package main

import (
	"bytes"
	"fmt"
	"io"
	"os"
	"os/exec"
	"regexp"
	"runtime"
	"strings"
	"sync/atomic"
	"time"

	"github.com/charmbracelet/bubbles/textinput"
	"github.com/charmbracelet/bubbles/viewport"
	tea "github.com/charmbracelet/bubbletea"
	"github.com/charmbracelet/lipgloss"
	"github.com/creack/pty"
	"golang.org/x/sys/unix"
)

const (
	binaryName = "main"
	browserCfg = "filesystem/home/admin/.config/browser/browser.cfg"
	maxLines   = 1500
)

var compileSources = []string{
	"main.cpp",
	"driver_registry.cpp",
	"kernel/kernel.c",
	"filesystem/proc/cpuinfo.cpp",
	"filesystem/proc/gpuinfo.cpp",
	"uefi.cpp",
	"logo.cpp",
	"bootloader.cpp",
	"make_system_dir.cpp",
	"kernel/initramfs.cpp",
}
var launcherArgs = map[string][]string{
	"vim":      {"filesystem/home/admin/Desktop"},
	"nvim":     {"filesystem/home/admin/Desktop"},
	"code":     {"filesystem/home/admin/Desktop"},
	"opencode": nil,
	"claude":   nil,
}

var commandWords = []string{
	"--version", "browser", "browser --print", "calculator", "cat",
	"cat proc/cpuinfo", "cat proc/gpuinfo", "chsh -s $(which bash)",
	"chsh -s $(which zsh)", "claude", "clear", "code", "date", "df -h",
	"echo", "echo $SHELL", "exit", "free -h", "help", "iwctl", "ls", "nvim",
	"opencode", "pacman -Syu", "penguinfetch", "reboot uefi", "root locate",
	"shutdown", "shutdown now", "touch", "update", "vim", "which",
	"which penguin", "whoami",
}

var (
	colGold    = lipgloss.Color("220")
	colCyan    = lipgloss.Color("45")
	colMagenta = lipgloss.Color("200")
	colRed     = lipgloss.Color("196")
	colGreen   = lipgloss.Color("82")
	colDim     = lipgloss.Color("245")

	stGold    = lipgloss.NewStyle().Foreground(colGold)
	stCyan    = lipgloss.NewStyle().Foreground(colCyan)
	stMagenta = lipgloss.NewStyle().Foreground(colMagenta)
	stRed     = lipgloss.NewStyle().Foreground(colRed)
	stGreen   = lipgloss.NewStyle().Foreground(colGreen)
	stDim     = lipgloss.NewStyle().Foreground(colDim)
)

type outputMsg struct{ text string }
type shellStartedMsg struct{}
type shellExitMsg struct{ err error }
type bootTimerMsg struct{}
type tickMsg struct{}
type quitMsg struct{}
type execDoneMsg struct{ err error }

type model struct {
	prog   *tea.Program
	vp     viewport.Model
	input  textinput.Model
	width  int
	height int

	shell  *exec.Cmd
	master *os.File
	slave  *os.File
	stdin  io.WriteCloser
	usePTY bool
	dead   atomic.Bool

	lines        []string
	pending      string
	history      []string
	histIdx      int
	compIdx      int
	bootAnswered bool
	booted       bool
	exited       bool
	exec         bool
	tick         bool
	version      string
	platform     string
}

func newModel(version, platform string) *model {
	m := &model{version: version, platform: platform}
	m.vp = viewport.New(80, 24)
	m.vp.MouseWheelEnabled = true
	m.vp.MouseWheelDelta = 3
	m.vp.Style = lipgloss.NewStyle().Border(lipgloss.RoundedBorder()).BorderForeground(lipgloss.Color("237"))
	m.input = textinput.New()
	m.input.CharLimit = 256
	m.input.Prompt = m.prompt()
	m.input.Focus()
	return m
}

func main() {
	if err := ensureBinary(); err != nil {
		fmt.Fprintln(os.Stderr, "build failed:", err)
		os.Exit(1)
	}
	m := newModel(cppConst("VERSION"), runtime.GOOS)
	p := tea.NewProgram(m, tea.WithAltScreen(), tea.WithMouseCellMotion())
	m.prog = p
	if _, err := p.Run(); err != nil {
		fmt.Fprintln(os.Stderr, err)
		os.Exit(1)
	}
}

func ensureBinary() error {
	out := binaryName
	if runtime.GOOS == "windows" {
		out = "main.exe"
	}
	if info, err := os.Stat(out); err == nil {
		fresh := true
		for _, src := range compileSources {
			if s, err := os.Stat(src); err == nil && s.ModTime().After(info.ModTime()) {
				fresh = false
				break
			}
		}
		if fresh {
			return nil
		}
	}
	fmt.Println("Building C++ binary...")
	args := append([]string{"-o", out, "-std=c++17"}, compileSources...)
	cmd := exec.Command("g++", args...)
	cmd.Stdout = os.Stdout
	cmd.Stderr = os.Stderr
	return cmd.Run()
}

func cppConst(name string) string {
	src, err := os.ReadFile("main.cpp")
	if err != nil {
		return "?"
	}
	re := regexp.MustCompile(name + `\s*=\s*"([^"]+)"`)
	if m := re.FindSubmatch(src); m != nil {
		return string(m[1])
	}
	return "?"
}

func (m *model) Init() tea.Cmd {
	return tea.Batch(
		m.spawnShell,
		tea.Tick(8*time.Second, func(time.Time) tea.Msg { return bootTimerMsg{} }),
		tea.Tick(500*time.Millisecond, func(time.Time) tea.Msg { return tickMsg{} }),
	)
}

func (m *model) spawnShell() tea.Msg {
	cmd := exec.Command("./" + binaryName)
	if runtime.GOOS != "windows" {
		if master, slave, err := pty.Open(); err == nil {
			cmd.Stdin = slave
			cmd.Stdout = slave
			cmd.Stderr = slave
			m.master = master
			m.slave = slave
			m.usePTY = true
			setNoEcho(slave)
		}
	}
	if !m.usePTY {
		in, err := cmd.StdinPipe()
		if err != nil {
			return shellExitMsg{err: err}
		}
		m.stdin = in
		out, _ := cmd.StdoutPipe()
		errPipe, _ := cmd.StderrPipe()
		go m.readStream(out)
		go m.readStream(errPipe)
	}
	if err := cmd.Start(); err != nil {
		return shellExitMsg{err: err}
	}
	m.shell = cmd
	if m.usePTY {
		go m.readStream(m.master)
	}
	go func() {
		m.prog.Send(shellExitMsg{err: cmd.Wait()})
	}()
	return shellStartedMsg{}
}

func (m *model) readStream(r io.Reader) {
	buf := make([]byte, 4096)
	acc := make([]byte, 0, 512)
	for {
		n, err := r.Read(buf)
		if n > 0 {
			acc = append(acc, buf[:n]...)
			for {
				idx := bytes.IndexByte(acc, '\n')
				if idx < 0 {
					break
				}
				part := strings.ReplaceAll(string(acc[:idx]), "\r", "")
				acc = acc[idx+1:]
				if !m.dead.Load() {
					m.prog.Send(outputMsg{text: part})
				}
			}
			if len(acc) > 0 {
				if !m.dead.Load() {
					m.prog.Send(outputMsg{text: strings.ReplaceAll(string(acc), "\r", "")})
				}
				acc = acc[:0]
			}
		}
		if err != nil {
			if len(acc) > 0 && !m.dead.Load() {
				m.prog.Send(outputMsg{text: strings.ReplaceAll(string(acc), "\r", "")})
			}
			return
		}
	}
}

func setNoEcho(f *os.File) {
	t, err := unix.IoctlGetTermios(int(f.Fd()), unix.TCGETS)
	if err != nil {
		return
	}
	t.Lflag &^= unix.ECHO
	unix.IoctlSetTermios(int(f.Fd()), unix.TCSETS, t)
}

func (m *model) writeShell(line string) {
	if m.usePTY {
		if m.master != nil {
			m.master.Write([]byte(line + "\n"))
		}
	} else if m.stdin != nil {
		m.stdin.Write([]byte(line + "\n"))
	}
}

func (m *model) handleOutput(line string) {
	if m.pending != "" {
		line = m.pending + line
		m.pending = ""
	}
	if !m.bootAnswered && strings.Contains(line, "Enter Boot Option") {
		m.bootAnswered = true
		m.writeShell("1")
		m.appendLine(styleLine(line + "1"))
		return
	}
	if m.usePTY {
		if t := strings.TrimRight(line, " "); strings.HasSuffix(t, "-->") || strings.HasSuffix(t, "#]") {
			m.pending = line
			return
		}
	}
	if !m.booted && strings.Contains(line, "Welcome To Talon") {
		m.booted = true
	}
	m.appendLine(styleLine(line))
}

func (m *model) appendLine(s string) {
	m.lines = append(m.lines, s)
	if len(m.lines) > maxLines {
		m.lines = m.lines[len(m.lines)-maxLines:]
	}
	m.vp.SetContent(strings.Join(m.lines, "\n"))
	m.vp.GotoBottom()
}

func styleLine(line string) string {
	if strings.Contains(line, "\x1b[") {
		return line
	}
	switch {
	case strings.HasPrefix(line, "[KERNEL]"):
		return stGold.Render(line)
	case strings.HasPrefix(line, "[BOOT]"):
		return stCyan.Render(line)
	case strings.HasPrefix(line, "[CREATED]"):
		return stGreen.Render(line)
	case strings.HasPrefix(line, "[EXISTS]"):
		return stDim.Render(line)
	case strings.HasPrefix(line, "[FAILED]"):
		return stRed.Render(line)
	case strings.Contains(line, "Flash Bootloader"):
		return stGold.Bold(true).Render(line)
	case strings.Contains(line, "Welcome To Talon"):
		return stCyan.Bold(true).Render(line)
	case strings.HasPrefix(line, "Talon Linux v"):
		return stGold.Bold(true).Render(line)
	case line == "root@talon [Desktop] >> [%]":
		return stCyan.Render("root@talon") + " " + stMagenta.Render("[Desktop]") + " " +
			stGold.Render(">>") + " " + stGreen.Render("[%]")
	default:
		return line
	}
}

func (m *model) submit(raw string) tea.Cmd {
	line := strings.TrimSpace(raw)
	m.history = append(m.history, raw)
	m.histIdx = len(m.history)
	m.compIdx = 0
	m.input.SetValue("")
	if line == "" {
		return nil
	}
	if m.exited {
		m.appendLine(stRed.Render("system offline"))
		return nil
	}
	switch {
	case line == "help":
		m.renderInput(line)
		m.appendHelp()
		return nil
	case line == "clear":
		m.renderInput(line)
		m.lines = nil
		m.vp.SetContent("")
		return nil
	case line == "browser":
		m.renderInput(line)
		return m.launchBrowser()
	case isLauncher(line):
		name, args, _ := launcherFor(line)
		m.renderInput(line)
		return m.launch(name, args)
	case line == "exit" || line == "shutdown" || line == "shutdown now":
		m.renderInput(line)
		m.writeShell(line)
		return nil
	default:
		m.renderInput(line)
		m.writeShell(line)
		return nil
	}
}

func (m *model) renderInput(line string) {
	if m.pending != "" {
		line = m.pending + line
		m.pending = ""
	} else {
		line = "--> " + line
	}
	m.appendLine(styleLine(line))
}

func launcherFor(line string) (string, []string, bool) {
	words := strings.Fields(line)
	if len(words) == 0 {
		return "", nil, false
	}
	if args, ok := launcherArgs[words[0]]; ok {
		if len(words) > 1 {
			return words[0], words[1:], true
		}
		return words[0], args, true
	}
	return "", nil, false
}

func isLauncher(line string) bool {
	_, _, ok := launcherFor(line)
	return ok
}

func (m *model) launch(name string, args []string) tea.Cmd {
	if name == "code" {
		cmd := exec.Command(name, args...)
		if err := cmd.Start(); err != nil {
			m.appendLine(stRed.Render("Something Went Wrong"))
		} else {
			m.appendLine(stDim.Render("── code launched ──"))
		}
		return nil
	}
	m.exec = true
	quoted := make([]string, 0, len(args)+1)
	quoted = append(quoted, name)
	for _, a := range args {
		quoted = append(quoted, "'"+strings.ReplaceAll(a, "'", `'\''`)+"'")
	}
	cmd := exec.Command("bash", "-c", "printf '\\033[2J\\033[H'; "+strings.Join(quoted, " "))
	return tea.ExecProcess(cmd, func(err error) tea.Msg { return execDoneMsg{err: err} })
}

func (m *model) launchBrowser() tea.Cmd {
	b, err := os.ReadFile(browserCfg)
	if err != nil || strings.TrimSpace(string(b)) == "" {
		m.appendLine(stRed.Render("Add Your Browser in " + browserCfg))
		return nil
	}
	fields := strings.Fields(string(b))
	m.exec = true
	quoted := make([]string, 0, len(fields))
	for _, a := range fields {
		quoted = append(quoted, "'"+strings.ReplaceAll(a, "'", `'\''`)+"'")
	}
	cmd := exec.Command("bash", "-c", "printf '\\033[2J\\033[H'; "+strings.Join(quoted, " "))
	return tea.ExecProcess(cmd, func(err error) tea.Msg { return execDoneMsg{err: err} })
}

func (m *model) appendHelp() {
	m.appendLine(stCyan.Bold(true).Render("── Talon OS — commands ──"))
	groups := []struct{ title, cmds string }{
		{"System", "cat proc/cpuinfo · cat proc/gpuinfo · free -h · df -h · date · whoami · --version · update"},
		{"Files", "ls · cat <file> · touch <file> · echo <text> · clear"},
		{"Kernel", "penguinfetch · root locate <target> · which <bin> · reboot uefi · pacman -Syu"},
		{"Apps (fullscreen)", "vim · nvim · code · browser · opencode · claude"},
		{"Interactive", "calculator · iwctl"},
		{"Shell", "exit · shutdown · help"},
	}
	for _, g := range groups {
		m.appendLine(stGold.Render(g.title) + "  " + g.cmds)
	}
	m.appendLine("")
}

func (m *model) complete() {
	val := m.input.Value()
	matches := []string{}
	for _, c := range commandWords {
		if strings.HasPrefix(c, val) {
			matches = append(matches, c)
		}
	}
	if len(matches) == 0 {
		return
	}
	if m.compIdx >= len(matches) {
		m.compIdx = 0
	}
	m.input.SetValue(matches[m.compIdx])
	m.input.CursorEnd()
	m.compIdx++
}

func (m *model) histPrev() {
	if len(m.history) == 0 {
		return
	}
	if m.histIdx > 0 {
		m.histIdx--
	}
	m.input.SetValue(m.history[m.histIdx])
	m.input.CursorEnd()
}

func (m *model) histNext() {
	if len(m.history) == 0 {
		return
	}
	if m.histIdx < len(m.history)-1 {
		m.histIdx++
		m.input.SetValue(m.history[m.histIdx])
	} else {
		m.histIdx = len(m.history)
		m.input.SetValue("")
	}
	m.input.CursorEnd()
}

func (m *model) killShell() {
	m.dead.Store(true)
	if m.shell != nil && m.shell.Process != nil {
		m.shell.Process.Kill()
	}
	if m.master != nil {
		m.master.Close()
	}
	if m.slave != nil {
		m.slave.Close()
	}
	if m.stdin != nil {
		m.stdin.Close()
	}
}

func (m *model) Update(msg tea.Msg) (tea.Model, tea.Cmd) {
	switch msg := msg.(type) {
	case outputMsg:
		m.handleOutput(msg.text)
		return m, nil
	case shellStartedMsg:
		return m, nil
	case shellExitMsg:
		m.exited = true
		return m, tea.Tick(400*time.Millisecond, func(time.Time) tea.Msg { return quitMsg{} })
	case bootTimerMsg:
		if !m.bootAnswered && !m.exited {
			m.bootAnswered = true
			m.writeShell("1")
			m.renderInput("1")
		}
		return m, nil
	case tickMsg:
		m.tick = !m.tick
		return m, tea.Tick(500*time.Millisecond, func(time.Time) tea.Msg { return tickMsg{} })
	case execDoneMsg:
		m.exec = false
		m.appendLine(stDim.Render("── back to Talon ──"))
		return m, nil
	case quitMsg:
		m.killShell()
		return m, tea.Quit
	case tea.WindowSizeMsg:
		m.width = msg.Width
		m.height = msg.Height
		m.vp.Width = max(10, msg.Width-2)
		m.vp.Height = max(3, msg.Height-4)
		m.input.Width = max(10, msg.Width-lipgloss.Width(m.prompt()))
		if m.usePTY && m.master != nil {
			pty.Setsize(m.master, &pty.Winsize{Cols: uint16(msg.Width), Rows: uint16(msg.Height)})
		}
		return m, nil
	case tea.MouseMsg:
		var cmd tea.Cmd
		m.vp, cmd = m.vp.Update(msg)
		return m, cmd
	case tea.KeyMsg:
		switch msg.String() {
		case "ctrl+c":
			m.killShell()
			return m, tea.Quit
		case "tab":
			m.complete()
			return m, nil
		case "up":
			m.histPrev()
			return m, nil
		case "down":
			m.histNext()
			return m, nil
		case "enter":
			return m, m.submit(m.input.Value())
		}
		m.compIdx = 0
		var cmds []tea.Cmd
		var cmd tea.Cmd
		m.input, cmd = m.input.Update(msg)
		cmds = append(cmds, cmd)
		m.vp, cmd = m.vp.Update(msg)
		cmds = append(cmds, cmd)
		return m, tea.Batch(cmds...)
	}
	return m, nil
}

func (m *model) prompt() string {
	return stCyan.Render("root@talon") + " " +
		stMagenta.Render("[Desktop]") + " " +
		stGold.Render(">>") + " " +
		stGreen.Render("[%]") + " "
}

func (m *model) renderHeader() string {
	title := stGold.Bold(true).Render(" TALON OS SIMULATION ")
	ver := stCyan.Render(" v" + m.version)
	plat := stDim.Render(" " + m.platform + " ")
	left := title + ver
	fill := 0
	if w := m.width - lipgloss.Width(left) - lipgloss.Width(plat); w > 0 {
		fill = w
	}
	return left + strings.Repeat("─", fill) + plat
}

func (m *model) renderStatus() string {
	var dot string
	if m.exited {
		dot = stRed.Render("●")
	} else if m.tick {
		dot = stGreen.Render("●")
	} else {
		dot = stGreen.Render("○")
	}
	left := dot
	if !m.booted && !m.exited {
		left += " " + stDim.Render("booting…")
	} else if m.exited {
		left += " " + stDim.Render("system halted")
	}
	right := stDim.Render("Tab: complete · ↑/↓: history · Ctrl+C: quit")
	pad := m.width - lipgloss.Width(left) - lipgloss.Width(right)
	if pad < 1 {
		pad = 1
	}
	return left + strings.Repeat(" ", pad) + right
}

func (m *model) View() string {
	if m.width == 0 {
		return "starting…"
	}
	if m.exec {
		return ""
	}
	hdr := m.renderHeader()
	body := m.vp.View()
	if !m.usePTY && m.pending != "" {
		body += "\n" + stDim.Render(m.pending)
	}
	return lipgloss.JoinVertical(lipgloss.Top, hdr, body, m.input.View(), m.renderStatus())
}

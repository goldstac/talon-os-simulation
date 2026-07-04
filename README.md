# talon-os-simulation

A minimal, multi-language operating system simulation experiment in Go, C, and C++.

Components:
- `main.go` — Go program that prints "test"
- `kernel/kernel.c` — C program simulating a kernel, prints "hello im a kernel"
- `system/free-h.cpp` — C++ program simulating a system utility, prints "hello im free -h"

## Structure

```
├── main.go              # Top-level entry point (Go)
├── kernel/
│   └── kernel.c         # Kernel simulation (C)
└── system/
    └── free-h.cpp       # System utility simulation (C++)
```

## License

GNU Affero General Public License v3.0. See [LICENSE](LICENSE).

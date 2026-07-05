#ifndef dxe_driver
#define dxe_driver
// Base Initialization (The Dispatcher/Common Interface)
void drivers_init();

// Hardware & Bus Drivers
void cpu_driver();
void ram_driver();
void pcie_bus_driver();

// Storage Drivers
void storage_driver();
void nvme_driver();
void sata_driver();
void ahci_driver();
void ide_driver();

// Graphics & Display Drivers
void gpu_driver();
void displayport_driver();
void hdmi_driver();

// Network Drivers
void ethernet_driver();
void wifi_driver();

// Input & Peripheral Drivers
void keyboard_driver();
void mouse_driver();
void usb_driver();

// System & Specialty Drivers
void audio_driver();
void efi_driver();
#endif

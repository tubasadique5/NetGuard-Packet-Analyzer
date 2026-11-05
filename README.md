# NetGuard – Network Packet Analyzer

A C++17 command-line network traffic analyzer that reads PCAP captures, parses packet headers, identifies selected application domains from visible TLS SNI / HTTP Host information, applies filtering rules, and writes a filtered PCAP file.

> **Project status:** Educational packet-analysis project. It analyzes saved PCAP files; it is not yet a live network capture tool, a firewall, or a full intrusion-detection system.

## Features

- Read classic PCAP files (Ethernet captures).
- Parse Ethernet, IPv4/IPv6 and TCP/UDP packet metadata where supported by the parser.
- Inspect visible TLS Server Name Indication (SNI) and HTTP host information.
- Classify a set of popular services from domain names.
- Track flows using the network five-tuple.
- Apply source-IP, application, and domain blocking rules to the capture being processed.
- Process packets using a reader / load-balancer / fast-path multithreaded pipeline.
- Write packets that pass the configured rules to a new PCAP file.
- Print packet, byte, protocol, thread, and application statistics.

## Requirements

- C++17 compiler (GCC 9+, Clang 10+, or a recent Visual Studio 2022 toolchain)
- CMake 3.16+
- Threads support

## Build

### Windows (PowerShell)

```powershell
cmake -S . -B build
cmake --build build --config Release
```

The executable is typically created at `build\Release\netguard.exe` with Visual Studio, or `build\netguard.exe` with a single-configuration generator.

### Linux / macOS

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

The executable is `build/netguard`.

## Run

```bash
# Analyze a capture and write packets that pass the rules
./build/netguard input.pcap filtered.pcap

# Block an application
./build/netguard input.pcap filtered.pcap --block-app YouTube

# Block a source IP and a domain
./build/netguard input.pcap filtered.pcap --block-ip 192.168.1.50 --block-domain example.com

# Change worker counts
./build/netguard input.pcap filtered.pcap --lbs 2 --fps 2
```

On Windows, replace `./build/netguard` with the path to `netguard.exe`.

## Tests

Run the included domain-classification unit test after building:

```bash
ctest --test-dir build --output-on-failure
```

## Sample data

The repository includes `test_dpi.pcap` for local testing. Run:

```bash
./build/netguard test_dpi.pcap filtered.pcap
```

Do not upload private packet captures: they may contain IP addresses, DNS requests, hostnames, and other sensitive metadata.

## How it works

1. The PCAP reader loads packet records from the capture.
2. The packet parser extracts supported link/network/transport headers.
3. A five-tuple hash distributes packets across worker queues.
4. The inspection stage attempts to identify application domains from visible protocol metadata.
5. Configured rules decide whether a packet is forwarded to the output capture or dropped from that output.
6. The program prints a processing summary.

## Important limitations

- This version processes offline PCAP files; it does not capture directly from a network interface.
- TLS 1.3, Encrypted Client Hello (ECH), QUIC, fragmented traffic, and encrypted DNS can limit domain visibility. A missing domain does not mean traffic is safe.
- Domain-based classification is heuristic and should not be treated as authoritative attribution.
- The current `extractExtensions` helper is a placeholder; full generic TLS extension enumeration is not implemented.
- The `--block-*` options filter the generated output PCAP. They do not block traffic on your computer or router.
- Use only packet captures you own or are authorized to inspect.

## Planned improvements

- CSV export for packet and application statistics.
- More comprehensive IPv6 and fragmented-packet test coverage.
- Unit tests and fuzz tests for malformed packet inputs.
- Optional live capture using a supported capture library.
- Better domain matching and configurable classification rules.
- A web dashboard for traffic summaries and alerts.

## Author

**Tuba Sadique** – project customization and further development.

## How to Run the Project on Windows

Follow these steps to build and run the NetGuard Packet Analyzer.

### 1. Prerequisites

Install the following tools:

- **CMake** – [Download CMake](https://cmake.org/download/)
- **MSYS2** – [Download MSYS2](https://www.msys2.org/)
- **GCC Compiler** – Install the UCRT64 GCC compiler through MSYS2.

Open the **MSYS2 UCRT64** terminal and run:

```bash
pacman -S mingw-w64-ucrt-x86_64-gcc
```

Make sure this folder is added to your Windows PATH:

```text
C:\msys64\ucrt64\bin
```

### 2. Open the Project Folder

Open PowerShell in the project directory:

```powershell
cd path\to\Packet_analyzer-main
```

### 3. Configure the Project

Run:

```powershell
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER="C:/msys64/ucrt64/bin/g++.exe"
```

### 4. Build the Project

Run:

```powershell
cmake --build build
```

After successful compilation, the executable will be available inside the `build` folder.

### 5. Run the Packet Analyzer

Use a sample PCAP file:

```powershell
.\build\netguard.exe test_dpi.pcap filtered.pcap
```

Here:
- `test_dpi.pcap` – Input network packet capture file.
- `filtered.pcap` – Output file containing the forwarded packets.

### 6. Run with Blocking Rules

**Block an application (YouTube):**

```powershell
.\build\netguard.exe test_dpi.pcap youtube_blocked.pcap --block-app YouTube
```

**Block an IP address:**

```powershell
.\build\netguard.exe test_dpi.pcap blocked.pcap --block-ip 192.168.1.10
```

**Block a domain:**

```powershell
.\build\netguard.exe test_dpi.pcap blocked_domain.pcap --block-domain example.com
```

### 7. Run Tests

To run the project's classification test:

```powershell
ctest --test-dir build --output-on-failure
```

A successful test should display:

```text
100% tests passed
```

### Important Notes

- Run commands from the project root directory.
- Replace sample PCAP filenames with your own `.pcap` files.
- The project is a command-line application; it does not have a graphical interface.
- The output PCAP file is created in the current project directory.



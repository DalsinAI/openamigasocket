# Contributors

## Creator and maintainer

- **SacredTrees** ([@SacredTrees](https://github.com/SacredTrees)): created OpenSocket, designs it and maintains it.

## The AmigaChrome team

We are the AI agents who build OpenSocket alongside SacredTrees:

- **Agnus**, our coordinator, who keeps every thread moving.
- **Thufir**, **Kynes** and **Galen**, the earlier agents who started the work on SacredTrees's x86 cores. Thufir also wrote OpenSocket's first `bsdsocket.library`, when the project was ACNet (`DalsinAI/Amigachrome-ACNet`).
- **The Claude Code threads**, each one taking a piece of the work from design to release.

## Copyright holder

OpenSocket's own code (the libraries, the device, the Commodity and the tools) and documents are
Copyright (c) 2026 Dalsin Limited, released under the MIT licence (`LICENSE`).

## Third-party work in this repository

OpenSocketDirect's TCP/IP core and three of the network tools are other people's code. They keep their
own copyright and licence; `tools/actcptools/THIRD_PARTY.md` and the notice
at the top of each file carry the formal terms.

| Component | Where | Authors | Licence |
| --- | --- | --- | --- |
| `ping`, from 4.4BSD-Lite2 through the Amiga port for Roadshow | `tools/actcptools/src/ping.c` | Mike Muuss, the Regents of the University of California; Amiga port by Olaf Barthel | BSD (University of California, with the advertising clause); Olaf Barthel's changes public domain |
| `traceroute`, likewise | `tools/actcptools/src/traceroute.c` | Van Jacobson, the Regents of the University of California; Amiga port by Olaf Barthel | As above |
| `arp`, likewise | `tools/actcptools/src/arp.c` | Sun Microsystems, Inc., the Regents of the University of California; Amiga port by Olaf Barthel | As above |
| lwIP 2.2.1 (`STABLE-2_2_1_RELEASE`), unchanged | `guest/network/acnet/direct/lwip/` | Adam Dunkels, the Swedish Institute of Computer Science, and lwIP's contributors | BSD (3-clause), `lwip/COPYING` |

The other tools are our own and none is derived from another implementation.

## Used at build or run time, not included

- **libslirp**: the packet provider behind the SANA-II path, on the x86 and ARM64 side.
- **AmiSSL 5**: `httpget` opens it at run time for https.
- **AmigaOS NDK**: headers and libraries for the build, supplied separately and not redistributed.

Amiga, AmigaOS and other product names are trademarks of their respective
owners.

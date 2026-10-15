# OpenSocket

OpenSocket is a free (MIT) `bsdsocket.library` for AmigaOS 3.2.3: the classic Amiga socket interface, its core library, the card's driver, a Commodity and the standard network tools. It was called ACNet until 4 October 2026. The repository is `DalsinAI/openamigasocket` (it was `DalsinAI/Amigachrome-ACNet`; GitHub forwards the old address).

It has two backends under the one socket interface, and a card layer that chooses between them when the library starts:

- **OpenSocket Host:** socket calls travel through the OpenSocket card (Zorro II, Dalsin product 6) in an AmigaChrome machine to real sockets on the x86 or ARM64 cores.
- **OpenSocketDirect:** a TCP/IP stack of its own (lwIP) over the machine's own network card and its SANA-II driver, for real Amigas and PiStorm. Its place in the card layer is built; the stack itself is not yet, and until it is, socket calls on Direct answer `ENOSYS` at once.

The design is `docs/architecture/OPENSOCKET_DESIGN.md` in DalsinAI/amigachrome.

## Choosing the card

`bsdsocket.library` chooses its backend once, when it starts: at its first `OpenLibrary`, which ramlib makes after DOS, so the choice can read the disk. The same code runs from `LIBS:` or from a ROM (`guest/network/acnet/build.sh module`). In this order:

1. **OpenSocket's configuration** names a backend or a device: `ENV:OpenSocket/Interface`, or `ENVARC:OpenSocket/Interface` when ENV: has none. That backend is used.
2. **The OpenSocket card is fitted** (expansion.library's `FindConfigDev`, Dalsin, product 6): Host, through `LIBS:opensocket.library` and `DEVS:Networks/opensocket.device`, both loaded from the disk.
3. **A SANA-II interface is configured** in Roadshow's `DEVS:NetInterfaces/`: Direct, over the device that file names.
4. **None of these:** the library's init keeps nothing. From a ROM, ramlib then goes on to `LIBS:bsdsocket.library`, so another stack on the disk (Roadshow, say) is opened as before; with none there, `OpenLibrary` fails, as with no stack at all.

Once a backend is chosen, `OpenLibrary` does not fail for want of the network. A Host bridge that is down, or a missing `opensocket.library` or card driver, gives `ENETDOWN` from the socket calls.

The configuration files are lines of `keyword=value` (`keyword value` is read too); `#` or `;` starts a comment. No requester appears when ENV: or DEVS: has no such file.

| Keyword | In `ENV:OpenSocket/Interface` | In a `DEVS:NetInterfaces/` file |
| --- | --- | --- |
| `backend` | `host`, `direct`, or `none` (the library steps aside for another stack) | not read |
| `device`, `unit` | the SANA-II device and unit for Direct | read |
| `configure` | `dhcp` | read (`configure=dhcp`) |
| `address`, `netmask` | a fixed IPv4 address (`address=dhcp` means DHCP) | read |
| `gateway` | the default route | not read |
| `nameserver` | up to three | name servers come from `DEVS:Internet/name_resolution` (`nameserver` lines) when OpenSocket's file names none |

From Roadshow's files only these documented keywords are read; the rest are skipped, and none of Roadshow's code is used. With no address set, Direct uses DHCP.

### On a real Amiga with its own network card

Install the card's SANA-II driver in `DEVS:Networks/` as its maker says. Then either keep the Roadshow interface file you have (`DEVS:NetInterfaces/a2065` with `device=a2065.device` and `configure=dhcp`, say), or write `ENV:OpenSocket/Interface` (and `ENVARC:`):

```
backend=direct
device=a2065.device
unit=0
configure=dhcp
```

To keep using Roadshow, or any other stack, as before, put `backend=none` in `ENVARC:OpenSocket/Interface`, or leave the machine with no OpenSocket card and no interface file: OpenSocket's library then steps aside and the disk's `bsdsocket.library` answers.

## The parts

| File | Installed as | What it is |
| --- | --- | --- |
| `bsdsocket.library` | `LIBS:`, or a ROM | The published socket interface: the 46 classic vectors (none stubbed) and 27 Roadshow-era calls after them (BPF, routes, modern IPv4 lookups), and the card layer that chooses the backend. Its id string says OpenSocket. |
| `opensocket.library` | `LIBS:` | OpenSocket's core: status, interfaces, routes, sockets, the event log, Wi-Fi requests. Everything else opens it. Was `acnetwork.library`. |
| `opensocket.device` | `DEVS:Networks/` | The card's driver: private calls for sockets, and a SANA-II face for frames. Was `acnet.device`; programs that open it fall back to `acnet.device` on older installs. |
| `opensocketwifi.device` | `DEVS:` | Control of the Wi-Fi outside the Amiga, by permission. Was `acwifi.device`. |
| `OpenSocketControl` | `SYS:Tools/Commodities/` | The Commodity, in GadTools. Was ACNetControlGT; the ReAction ACNetControl is retired. |
| `OpenSocket` | `C:` | Status, online, offline. Was `acnetctl`. |
| ping, traceroute, arp, ifconfig, route, netstat, hostname, resolve | `SYS:Tools/OpenSocket/` | The standard network tools (ACTCPTools). Never put in `C:`, so another stack's commands are not overwritten. |

The network interface is `opensocket0` (was `acnet0`). The card's HostSocket block keeps its signature ("ACH1"), commands and product number, so the wire format is unchanged.

## Building

The NDK is not redistributed here. Set `STOVE` to an AmigaOS 3.x cross-build environment containing `m68k-amigaos-gcc` and NDK 3.2 (`$STOVE/ndk`, for the SANA-II and Roadshow headers).

```sh
STOVE=/path/to/os32 ./guest/network/acnet/build.sh ./build/opensocket
STOVE=/path/to/os32 ./tools/actcptools/build.sh ./build/opensocket
```

The first builds the libraries, both devices, `OpenSocketControl` and the qualification programs (acnettest, bsdqual and the probes); the second builds `OpenSocket` and the tools. The builds are reproducible: the same commit gives the same bytes.

`build.sh module` builds only `bsdsocket.library`, at `OPT=` (default `-Os`), for a ROM: `build-amiga/modules/bsdsocket.module`. Every build of the library fails if it keeps data or BSS or leaves anything undefined: there are no writable globals, so the same code runs from a ROM. The card layer's configuration parser has host tests (`guest/network/acnet/tests/host`), which CI runs.

The vector table is generated from the checked-in manifest `guest/network/acnet/abi/bsdsocket-v4.json`; no third-party socket-library SFD is read. Source folders still carry their ACNet-era names (`guest/network/acnet`, `acnetwork`, `acwifi`); they move with the repository rename.

## Status

The status log from 1 to 3 October is in `docs/history/STATUS.md`. The complete stack builds and passes its host-side tests; live qualification of the matched guest and host set on AmigaOS 3.2.3 comes next. Until then, treat it as alpha.

## AmigaChrome integration

The AmigaChrome runtime supplies the OpenSocket card, the HostSocket service on the x86 or ARM64 cores, the Network switch and the installer that puts this payload on an instance. Those live in DalsinAI/amigachrome with the machine they depend on.

## Licence and credit

OpenSocket is MIT-licensed: `LICENSE`, Copyright (c) 2026 Dalsin Limited. Anyone may use, change, fork and redistribute it, commercially or not. The one condition is the MIT one: the copyright and permission notice stays with every copy and every fork.

A request, not a condition: if you fork or ship OpenSocket, please say it is based on OpenSocket by Dalsin Limited.

Inside the project, a few files keep their own licences:
- `tools/actcptools/src/ping.c`, `traceroute.c` and `arp.c` derive from 4.4BSD-Lite2 (through Olaf Barthel's Amiga ports) and keep the University of California's licence and notices (`tools/actcptools/THIRD_PARTY.md`).
- lwIP, when OpenSocketDirect brings it in, keeps its own BSD licence and notice.

AmigaOS/NDK material is not included and remains subject to its own licensing terms.

## Contributors

OpenSocket is created and maintained by [SacredTrees](https://github.com/SacredTrees) with the AmigaChrome agent team, copyright Dalsin Limited. Everyone whose work it includes is credited in [`CONTRIBUTORS.md`](CONTRIBUTORS.md).

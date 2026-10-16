# ACNet: networking for AmigaOS 3.2.3 on AmigaChrome

> **Renamed 4 October 2026.** ACNet is now OpenSocket: `acnet.device` is `DEVS:Networks/opensocket.device`, `acnetwork.library` is `opensocket.library`, `acwifi.device` is `opensocketwifi.device`, ACNetControlGT is OpenSocketControl (the ReAction version is retired), `acnetctl` is `C:OpenSocket`, the tools are in `SYS:Tools/OpenSocket/` and the interface is `opensocket0`. The current design is `docs/architecture/OPENSOCKET_DESIGN.md` in DalsinAI/amigachrome. The entries below keep the names of their time.

**Status:** Complete-stack code built; consolidated live qualification pending · **Date:** 3 October 2026 · **Guest branch:** `feature/complete-stack-20261003` · **Host branch:** `net/complete-stack-20261003`
**Created by** AmigaChrome Team, in collaboration with Thufir Hawat.

This is the build design. It takes the 30 September designs (capsules ACNet BSDSocket CX Design and ACNet FastPath HostSocket ACNetDev SANA2) and fixes the shape we set on 1 October.

## 1. What we are building

| Part | What it is |
| --- | --- |
| `LIBS:bsdsocket.library` | Classic Amiga BSD-socket compatibility facade. The core v4 ABI now has all 46 application vectors implemented; optional Roadshow-shaped compatibility lives behind the reserved guard tail. |
| `LIBS:acnetwork.library` | Native AmigaChrome networking boundary below `bsdsocket.library`. Native AC software may call it directly for status, interfaces, routes, neighbours, DNS, sockets and control without inheriting legacy BSD policy. |
| ACNet, the card | A Zorro II card in the autoconfig chain: AmigaChrome Team, product 6, 64 KB. ShowConfig and SysInfo list it. It is fitted while the instance's Network switch is on. |
| `DEVS:acnet.device` | The one driver that touches the card's HostSocket block. It owns the lock and the interrupt and wakes waiting tasks. It gives the library private direct calls, with no I/O request per packet. |
| `DEVS:acwifi.device` | Control of the host's Wi-Fi through the same card: scan, status, join, leave, forget. |
| ACNetControl | ReAction Commodity with Status, Wi-Fi, Connections, Diagnostics and Log pages. Live status/connection views, Refresh, soft online/offline, DNS lookup, TCP connect test and support-report save are implemented. To be replaced by ACNetControlGT once that has the same pages (AmigaOS 3.x programs are GadTools or MUI). |
| ACNetControlGT | The Commodity in GadTools (AmigaOS 2.04 to 3.2), on the shared core (`control/acnetcontrol_core.c`). First-light pages for now. |
| ACTCPTools | Native OS 3.2.3 command suite: `hostname`, `resolve`, `ping`, `traceroute`, `arp`, `ifconfig`, `route`, `netstat`, `acnetctl`. |
| The host service | Part of the instance's bridge on Linux. It owns the real sockets, DNS and the Wi-Fi requests. |

```
 classic Amiga app               AmigaChrome-native app / ACNetControl
        |                                     |
 bsdsocket.library                       acnetwork.library
        |                                     ^
        +------------> acnetwork.library -----+
                              |
                         acnet.device
                              |
                 ACNet Zorro II card / HostSocket
                              |
                    instance bridge on Linux
                    sockets, DNS, state/control

 acwifi.device is the permission-gated Wi-Fi control path on the same card.
 packet software may also use acnet.device's SANA-II face; BPF taps that virtual Ethernet independently.
```

HostSocket backs the fast application-socket path, while the SANA-II/BPF path uses an unprivileged user-mode Ethernet provider. Applications do not pay the packet-stack cost merely to use BSD sockets. Linux performs the low-level host TCP/IP work for the socket provider; ACNet owns the Amiga ABI, waits, errors, policy and compatibility behaviour.

## 2. Decisions (We, 1 October 2026)

1. **The library first.** OS 3.2.3 programs reach the network through ACNet's `bsdsocket.library`. A SANA-II card is not on its path.
2. **Off until switched on.** Each instance has a Network switch in Cradle's hardware panel. The library still opens with the switch off, and reports the network down (ENETDOWN).
3. **Internet and LAN, not this PC.** 127.0.0.1 is the Amiga's own loopback. The PC's own addresses are refused, which keeps out Cradle, other instances' bridges and other services on the PC.
4. **Two drivers, one card.** `acnet.device` and `acwifi.device` both talk to ACNet, which shows in autoconfig as a card in its own right (AmigaChrome Team product 6, the number main reserved for networking). We added this on 1 October. Its top half is kept for the later packet rings. Product 8 stays reserved.
5. **One Commodity**, ACNetControl, written in ReAction, with a Wi-Fi tab. ReAction ships with OS 3.2.3 and NDK 3.2 has its headers; MUI would be a third-party install. This merges the 30 September design's two Commodities. ACNetControlGT (4 October) shows the same pages in GadTools for machines without ReAction; both draw from `control/acnetcontrol_core.c` and register the same broker, so only one runs.
6. **Host Wi-Fi control is a separate per-instance permission, off by default.** Joining a network changes the whole PC's Wi-Fi.
7. **No paid licence and no weak spots.** The library must pass bsdsocktest and a matrix of real programs before release.

## 3. bsdsocket.library

- **Interface.** ACNet owns a checked-in classic Amiga BSD socket ABI manifest: 46 application vectors followed by 10 reserved growth slots. The vector table is generated from that manifest, not from a third-party stack SFD. As of 3 October all 46 core application vectors are implemented; the core has no remaining `ENOSYS` stubs.
- **Per opener.** Every `OpenLibrary()` returns a base of its own, following the classic Amiga socket model. Each base holds its own descriptors, errno, signals, h_errno and name buffers. Closing it closes its sockets.
- **Waiting.**
  - Blocking calls, WaitSelect and name lookups sleep on Exec signals. The device's interrupt wakes them; nothing polls.
  - Ctrl-C (or the opener's break mask) interrupts a wait with EINTR.
  - WaitSelect takes the caller's signal mask and a timeout (timer.device).
- **Name and network database lookups** run on the host, asynchronously where required, so a slow resolver never freezes the machine. `gethostbyname`, `gethostbyaddr`, `getservbyname`, `getservbyport`, `getnetbyname` and `getnetbyaddr` are implemented. Protocols come from a small built-in table; `gethostid()` is derived from the guest-facing ACNet IPv4 interface.
- **Moving sockets between tasks.** ObtainSocket, ReleaseSocket, ReleaseCopyOfSocket and Dup2Socket work. Servers started inetd-style depend on them.
- **Interfaces and routes.** Native state comes from `acnetwork.library`. `bsdsocket.library` contains an optional Roadshow-shaped compatibility adapter for read-only interface, route/ARP, DNS and statistics queries. Mutable Roadshow administration remains guarded rather than pretending Linux-owned state lives inside an Amiga TCP/IP stack.
- **Version:** 4.x, preserving the classic Amiga `bsdsocket.library` application ABI. The id string says ACNet.

## 4. acnet.device

- It finds the ACNet card (AmigaChrome Team $DA15, or 2011 for older ROMs, product 6) through expansion.library. It checks the HostSocket block's signature ("ACH1") and version, then adds one INT2 server. With no card it opens anyway and reports the network absent.
- **Private calls** (vectors after the standard device ones):
  - `ACN_Call`: one command. It writes the arguments, copies the transmit buffer, starts the command and copies the answer, all under Forbid().
  - `ACN_AddWaiter` and `ACN_RemWaiter`: a task and its signal, woken on each event.
  - `ACN_State`: present, and online.
- The library opens it once per opener and calls those vectors directly.
- **SANA-II is implemented.** `BeginIO` provides a packet-oriented Ethernet facade for software that bypasses `bsdsocket.library`, including reads/writes, online/offline, station/configuration queries, multicast/broadcast, event requests, type tracking/statistics and NSD device query. It remains independent of the application socket path.

## 5. acwifi.device

- The same card and the same discipline as acnet.device, using command range $40-$4F.
- **Calls:** Scan, Networks (SSID, signal, security, known), Status (SSID, BSSID, signal, channel, security, link rate), Join (a known network), Leave, Forget. A scan returns a ticket and the answer follows with an event.
- **Host side:** the bridge asks NetworkManager (`nmcli`). Passphrases never enter or stay in the Amiga.
- **Permission:** refused (EPERM) unless the instance's Wi-Fi control permission is on.

## 6. ACNetControl

- A real `commodities.library` broker using stock OS 3.2.3 ReAction. Exchange Show/Hide and the Ctrl-Alt-N hotkey expose the window; Kill quits only the control app.
- **Status:** live card/online state, hostname, IPv4, mask, gateway, DNS, host link, interface, open-socket count, byte counters, connects and refusals.
- **Connections:** read-only live socket inventory for the instance.
- **Controls:** Refresh and soft Go online/Go offline call the native ACNetwork control path.
- **Diagnostics:** editable DNS lookup and TCP host:port connection tests go through the public `bsdsocket.library` ABI; support snapshots save to `RAM:ACNetReport.txt`.
- **Wi-Fi:** backed by the built `acwifi.device`/NetworkManager path; scan/status and permission-gated known-profile join/leave/forget use asynchronous ACNet tickets.
- **Log:** backed by the bounded native ACNet event ring; the page reads sequenced host events rather than placeholder text.
- The stock-ReAction autoinit bug was fixed by strongly defining the class bases; the current build is warning-free.
- Instance-23 carries the current test copy under `SYS:Tools/ACNetwork Tools/ACNetControl`.

## 7. The host service

- It runs in the instance bridge (`hostsocket.py`, beside `native_bridge.py`). Host sockets are non-blocking. The service never blocks the runtime longer than one non-blocking call.
- **Handles** are small integers per instance. Host descriptors are never exposed.
- **Events:** the guest's POLL names the sockets it waits on. A watcher thread posts one event, input record 11, when any of them, or a pending DNS answer, becomes ready. The runtime holds INT2 while the event is pending and IRQ_ENABLE is set.
- **Policy:**
  - Off unless the instance's switch is on; it is read again on every HELLO and SOCKET.
  - TCP and UDP are IPv4-only in v1. General raw sockets remain refused. Two constrained compatibility personalities are implemented: classic ICMP Echo is virtualised over Linux unprivileged datagram-ICMP sockets, and traceroute raw-UDP/IP probes are translated through Linux UDP plus the error queue. Other raw protocols remain refused.
  - Connects and datagrams to 127.0.0.0/8, 0.0.0.0/8, multicast or the PC's own addresses are refused (ECONNREFUSED or ENETUNREACH).
  - Guest loopback works: a connect to 127.0.0.1:P reaches the same instance's own listener on P.
  - Wi-Fi changes require the separate Cradle Host Wi-Fi control permission and operate on known NetworkManager profiles without passing passwords through the guest ABI.
  - The packet provider is user-mode libslirp with IPv4/IPv6 capability; BPF taps only this virtual segment and does not obtain host-NIC promiscuous access.
- **Errno, levels and options** use the Amiga's (BSD) numbers on the board and are mapped to Linux's in the service. Human-readable errno/h_errno strings are provided by the guest library.

## 8. The board

ACNet is a 64 KB Zorro II card: AmigaChrome Team $DA15 (or 2011, as ACBridge), product 6, serial 1. It comes after ACStorage in the chain. It is fitted when the runtime starts with `-n`, and the bridge fits or pulls it from the next reset with input record 12, as the Network switch changes. Turning the switch off takes the network down at once; the card goes at the next reboot.

HostSocket's block starts at the card's base:
- registers at $0000 to $00FF;
- a 16,128-byte transmit buffer at $0100;
- a 16,384-byte receive buffer at $4000;
- $8000 to $FFFF remains reserved for a future higher-throughput packet-ring transport; the current SANA-II/BPF implementation uses the bounded HostSocket packet commands.

The registers and commands are in `vendor/amigachrome-guest/common/protocol/achostsocket.h`. A command is synchronous: writing COMMAND runs it, and RESULT, ERRNO and RXLEN are set when the write returns. EVENT plus IRQ_ENABLE gives INT2. The runtime and the bridge talk over the existing request pipe as board 3, and over the input pipe with record 11.

## 9. Cradle

- The hardware panel gains a **Network** switch and a **Host Wi-Fi control** switch. Both are off.
- Switching Network on fits the card from the next reboot and, for AmigaOS 3.x, installs `LIBS:bsdsocket.library`, `LIBS:acnetwork.library`, `DEVS:acnet.device`, `DEVS:acwifi.device`, ACNetControl and the native ACNetwork Tools suite from a hash-verified vendored payload. Existing third-party `bsdsocket.library` installations are detected as conflicts rather than silently overwritten.
- The installer is transactional, updates ACFS metadata/protection records, rolls back partial writes, refuses path/symlink escape and only updates a stopped OS3 guest. AROS keeps its own stack.

## 10. Quality bar

- **Host:** the complete-stack HostSocket suite is 35/35, including sockets, DNS, ICMP/traceroute, native state/logging, Wi-Fi permission/control, SANA-II packet-provider commands and classic-BPF filtering/capture/injection.
- **Cradle/install:** installer + hardware-model + HostSocket combined gate is 56/56; the transactional ACNet guest installer is 6/6 including foreign-stack refusal and injected rollback.
- **Board:** board_test cases cover the card block's registers, buffers and INT2; the packet path adds bounded user-mode Ethernet rather than host raw-NIC access.
- **Guest:** the complete stack cross-build is warning-clean with 46/46 core BSD vectors and 27 implemented Roadshow compatibility-tail vectors. Earlier socket/tools functionality is live-qualified on Instance-6; the newly completed modern/BPF/SANA-II/Wi-Fi pieces still require the consolidated live campaign.
- **External compatibility:** the Roadshow NDK's AmigaOS libpcap 0.8.1 source cross-builds against ACNet BPF into a 140 KiB m68k archive.
- **Conformance:** bsdsocktest (tbdye, 142 tests). The target is everything attempted, no unexpected failures, and every limit written down.
- **Programs:** an FTP client, a browser, an IRC client, SimpleMail, and a long download.
- **Never:** busy polling, a wait that can't be broken, or a library open that can hang.

## 11. Limits in version 1

- The classic AmigaOS 3.2/Roadshow socket ABI is IPv4. The target NDK has no public `AF_INET6`/`sockaddr_in6` ABI, so ACNet does not invent one. The SANA-II/libslirp packet path already carries IPv6 traffic and reports IPv6 capability.
- General unrestricted raw sockets remain unavailable; only the constrained ICMP Echo and traceroute personalities are virtualised.
- Mutable Roadshow administration (route/interface/DNS mutation) remains guarded because Cradle/Linux owns host network configuration. Read-only state is implemented.
- Wi-Fi join/forget is deliberately limited to known NetworkManager profiles; no password crosses the Amiga ABI.
- Roadshow mbuf/global-data/server-private/IP-filter internals remain safe guards rather than dependencies of ACNet core.
- The supplied tcpdump 3.8.1 source archive has an unrelated missing `rpc/pmap_prot.h`; its real AmigaOS libpcap dependency already builds against ACNet BPF.
- AROS 68k keeps its own stack. ACNet is AmigaOS 3.2.3 first.

## 12. Relationship to earlier designs

- **Kept:** the 30 September ACNet designs' interface, their test gates and their security boundary. Their two Commodities are merged into one.
- **Superseded:**
  - the 27 September ACNet v0.1 UDP-only stack;
  - the statement in the 27 September Network/Wi-Fi design that AmigaChrome would not supply bsdsocket.library;
  - that design's bridge-by-default rule (now off by default).
- **Superseded by the integrated card:** the earlier separate ACWifiNet/product-8 packet prototype supplied useful groundwork, but Wi-Fi control and SANA-II packet access now live on the integrated product-6 ACNet architecture.

## 13. Remaining work to drop-in release

1. Commit/merge the matching guest `feature/complete-stack-20261003` and host/runtime `net/complete-stack-20261003` branches.
2. Deploy the matching HostSocket/runtime and hash-verified guest payload together; do not qualify a new guest library against an older provider protocol.
3. Run the consolidated Instance-6 gate: core BSD qualification, modern compatibility probe, BPF probe, SANA-II probe, Wi-Fi device probe, ACTCPTools, Roadshow read-only probe and ACNetControl live telemetry/control.
4. Qualify the one-switch Cradle installer on a disposable OS3 volume, including upgrade, foreign-stack conflict and ACFS metadata preservation.
5. Run the real-application matrix: FTP, browser, IRC, SimpleMail, long transfer and representative Roadshow/AmiTCP software.
6. Run the broader bsdsock conformance suite and record every deliberate compatibility boundary.
7. Package tcpdump separately if desired; its current supplied source bundle is missing an RPC printer header, while the ACNet BPF/libpcap layer itself is built.

## 14. Open decisions / later lanes

- Whether Wi-Fi may ever join an unknown host profile; version 1 deliberately carries no passphrase through the guest.
- Whether mutable route/interface/DNS administration should ever be exposed from the guest rather than remaining host-owned.
- Whether ACNet should define a future native IPv6 socket ABI for new software; this would be an ACNet extension, not a fabricated Roadshow ABI.
- Whether the reserved upper card aperture should become a higher-throughput packet ring after the command-based SANA-II path is qualified.
- Whether tcpdump itself should be packaged after repairing the incomplete upstream/NDK source bundle.

#!/bin/sh
# OpenSocket (formerly ACNet, renamed 4 Oct 2026) for AmigaOS 3.2.3:
# opensocket.device (DEVS:Networks/), opensocket.library (built by
# ../acnetwork/build.sh) and the bsdsocket.library face (bare: no startup code
# or C library), plus the test programs (libnix), with the os32 stove (bebbo's
# m68k-amigaos-gcc, NDK 3.2). The library vector table is generated from its own classic
# Amiga BSD socket ABI manifest; no third-party socket SFD is required.
# bsdqual links common/os3/acnet_stack.c: this libnix ignores __stack, so it
# swaps to a stack of its own.
#   network/acnet/build.sh [OUT_DIR]     (default build/guest/os32/acnet)
set -eu
HERE=$(cd "$(dirname "$0")" && pwd)
GUEST=$(cd "$HERE/../.." && pwd)
if [ -z "${STOVE:-}" ]; then
  echo "Set STOVE to an AmigaOS 3.x cross-build environment containing m68k-amigaos-gcc and standard NDK headers." >&2
  exit 2
fi
CC="$STOVE/prefix/bin/m68k-amigaos-gcc"
ABI="$HERE/abi/bsdsocket-v4.json"
GUARD="$HERE/compat/roadshow/guard-v4.json"
OUT=${1:-$GUEST/build/guest/os32/acnet}
mkdir -p "$OUT"
BARE="-m68020 -O2 -fno-delete-null-pointer-checks -include sys/types.h -fomit-frame-pointer -fno-toplevel-reorder -fno-builtin -Wall -Wno-pointer-sign -nostartfiles -nostdlib"
INC="-I$GUEST/common/protocol -I$HERE/include -I$GUEST/network/acnetwork/include"
COMPAT_INC="-I$STOVE/ndk/SANA+RoadshowTCP-IP/include"

"$CC" $BARE $INC $COMPAT_INC -o "$OUT/opensocket.device" "$HERE/device/acnet_device.c" "$GUEST/common/os3/string.c" -lgcc
echo "$OUT/opensocket.device ($(wc -c < "$OUT/opensocket.device") bytes)"

STOVE="$STOVE" "$GUEST/network/acnetwork/build.sh" "$OUT"

L="$HERE/library"
# lib_base.c first: its start() must be the library's first code.
LIB="$L/lib_base.c $L/lib_fd.c $L/lib_errno.c $L/lib_tags.c $L/lib_strings.c $L/lib_select.c $L/lib_conn.c $L/lib_io.c $L/lib_opt.c $L/lib_names.c $L/lib_inet.c $HERE/compat/roadshow/routes.c $HERE/compat/roadshow/state.c $HERE/compat/roadshow/modern.c $HERE/compat/roadshow/bpf.c"
python3 "$HERE/library/gen_vectors.py" "$ABI" "$OUT/bsdsocket_vectors.c" $LIB --guard "$GUARD"
"$CC" $BARE $INC $COMPAT_INC -I"$HERE/library" -I"$OUT" -o "$OUT/bsdsocket.library" \
    $LIB "$HERE/library/provider_acnetwork.c" "$OUT/bsdsocket_vectors.c" "$GUEST/common/os3/string.c" -lgcc
echo "$OUT/bsdsocket.library ($(wc -c < "$OUT/bsdsocket.library") bytes)"

if [ -f "$HERE/tests/acnettest.c" ]; then
    "$CC" -m68020 -O2 -fno-delete-null-pointer-checks -Wall -noixemul -o "$OUT/acnettest" "$HERE/tests/acnettest.c"
    echo "$OUT/acnettest ($(wc -c < "$OUT/acnettest") bytes)"
fi

if [ -f "$HERE/tests/bsdqual.c" ]; then
    "$CC" -m68020 -O2 -fno-delete-null-pointer-checks -Wall -Wno-pointer-sign -noixemul -I"$GUEST/common/os3" -o "$OUT/bsdqual" \
        "$HERE/tests/bsdqual.c" "$GUEST/common/os3/acnet_stack.c"
    echo "$OUT/bsdqual ($(wc -c < "$OUT/bsdqual") bytes)"
fi

if [ -f "$HERE/tests/roadshow_readonly_probe.c" ]; then
    "$CC" -m68020 -O2 -fno-delete-null-pointer-checks -Wall -Wno-pointer-sign -noixemul \
        -o "$OUT/roadshow-readonly-probe" "$HERE/tests/roadshow_readonly_probe.c"
    echo "$OUT/roadshow-readonly-probe ($(wc -c < "$OUT/roadshow-readonly-probe") bytes)"
fi

if [ -f "$HERE/tests/modern_compat_probe.c" ]; then
    "$CC" -m68020 -O2 -fno-delete-null-pointer-checks -Wall -Wno-pointer-sign -noixemul \
        -o "$OUT/modern-compat-probe" "$HERE/tests/modern_compat_probe.c"
    echo "$OUT/modern-compat-probe ($(wc -c < "$OUT/modern-compat-probe") bytes)"
fi

if [ -f "$HERE/tests/bpfprobe.c" ]; then
    "$CC" -m68020 -O2 -fno-delete-null-pointer-checks -Wall -Wno-pointer-sign -noixemul $COMPAT_INC \
        -o "$OUT/bpfprobe" "$HERE/tests/bpfprobe.c"
    echo "$OUT/bpfprobe ($(wc -c < "$OUT/bpfprobe") bytes)"
fi

if [ -f "$HERE/tests/sana2probe.c" ]; then
    "$CC" -m68020 -O2 -fno-delete-null-pointer-checks -Wall -Wno-pointer-sign -noixemul $INC $COMPAT_INC \
        -o "$OUT/sana2probe" "$HERE/tests/sana2probe.c"
    echo "$OUT/sana2probe ($(wc -c < "$OUT/sana2probe") bytes)"
fi

# OpenSocketControl: the Commodity, in GadTools on the shared core (AmigaOS
# 3.x programs are GadTools or MUI). control/acnetcontrol.c, the retired
# ReAction version, is no longer built; it stays as the reference for the
# pages OpenSocketControl still lacks (live control, diagnostics, log, Wi-Fi).
# -fno-common: the program's own library bases must win over libnix's
# auto-open stubs.
# No -lamiga here: the compiler already links amiga.lib after libnix (for
# NewList and HotKey). Named on the command line, amiga.lib came first, and
# its sprintf (RawDoFmt's: %u and %d read 16 bits, no %llu) replaced libnix's.
# Each address byte then printed as two numbers, so 192.168.x.x showed as
# 0.192.0.168 (12 Oct 2026). The link map is checked for that below.
if [ -f "$HERE/control/acnetcontrol_gt.c" ]; then
    "$CC" -m68000 -O2 -fno-delete-null-pointer-checks -fno-common -Wall -Wno-pointer-sign -noixemul -I"$HERE/include" \
        -I"$GUEST/network/acnetwork/include" -I"$GUEST/network/acwifi/include" \
        -Wl,-Map="$OUT/OpenSocketControl.map" \
        -o "$OUT/OpenSocketControl" "$HERE/control/acnetcontrol_gt.c" "$HERE/control/acnetcontrol_core.c"
    if grep -Eq 'libamiga\.a\((v?s?printf)\.o\)' "$OUT/OpenSocketControl.map"; then
        echo "OpenSocketControl: printf or sprintf came from amiga.lib, whose %u is 16 bits" >&2
        exit 1
    fi
    echo "$OUT/OpenSocketControl ($(wc -c < "$OUT/OpenSocketControl") bytes)"
fi

if [ -f "$GUEST/network/acwifi/build.sh" ]; then
    sh "$GUEST/network/acwifi/build.sh" "$OUT"
fi

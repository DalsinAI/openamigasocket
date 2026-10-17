#!/bin/sh
set -eu

HERE=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
STOVE=${STOVE:-}
if [ -z "$STOVE" ]; then
    echo "Set STOVE to the AmigaChrome OS 3.2 cross-build stove." >&2
    exit 2
fi

CC="$STOVE/prefix/bin/m68k-amigaos-gcc"
OUT=${1:-"$HERE/build"}
CFLAGS="-m68040 -O2 -fno-delete-null-pointer-checks -Wall -Wno-pointer-sign -noixemul -I$HERE/../../guest/network/acnetwork/include"
mkdir -p "$OUT"

build_one() {
    name=$1
    "$CC" $CFLAGS -I"$HERE/src" "$HERE/src/$name.c" -o "$OUT/$name"
    echo "$OUT/$name ($(wc -c < "$OUT/$name") bytes)"
}

build_one hostname
build_one resolve
build_one ping
build_one traceroute
build_one arp
build_one ifconfig
build_one route
build_one netstat
# The newer tools share src/ostool.c and the stack helper (libnix ignores __stack).
OSTOOL="$HERE/src/ostool.c $HERE/../../guest/common/os3/acnet_stack.c"
build_net() {
    name=$1
    shift
    "$CC" $CFLAGS -I"$HERE/src" -I"$HERE/../../guest/common/os3" "$@" "$HERE/src/$name.c" $OSTOOL -o "$OUT/$name"
    echo "$OUT/$name ($(wc -c < "$OUT/$name") bytes)"
}

build_net host
build_net whois
build_net finger
build_net sntp
build_net tftp
build_net nc
build_net telnet
build_net ftp
# httpget: https:// when AMISSL names the AmiSSL 5 SDK's include folder.
if [ -n "${AMISSL:-}" ]; then
    build_net httpget -DHAVE_AMISSL -I"$AMISSL"
else
    build_net httpget
fi

# C:OpenSocket (was acnetctl): status, online, offline
"$CC" $CFLAGS -I"$HERE/src" "$HERE/src/opensocket.c" -o "$OUT/OpenSocket"
echo "$OUT/OpenSocket ($(wc -c < "$OUT/OpenSocket") bytes)"

echo "ACTCPTools build complete."
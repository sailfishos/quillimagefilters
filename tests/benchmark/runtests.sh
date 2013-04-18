#!/bin/sh

# --- env. preparations
export DISPLAY=:0
/usr/sbin/dsmetool -n -1 -t "/usr/bin/dbus-daemon --session"
export DBUS_SESSION_BUS_ADDRESS=unix:path=/tmp/session_bus_socket 

OUTDIR=/tmp/quillimagefilter-benchmark 

rm -rf "${OUTDIR}"
mkdir -p "${OUTDIR}"
touch "${OUTDIR}/dummy.log"

chmod +x /usr/lib/quillimagefilter-benchmark/benchmark

#/usr/lib/quillimagefilter-benchmark/benchmark
#/usr/share/quillimagefilter-benchmark/mem-monitor-smaps -i 0.01 -p benchmark

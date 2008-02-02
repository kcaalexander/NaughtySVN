#!/bin/sh

[ "`id -ur`" != "0" ] && echo "this script needs to be ran as root" && exit 1 || true

if which checkinstall > /dev/null
then
    echo "Found checkinstall, ok"
else
    echo "You need checkinstall to use this script. Please install checkinstall."
    exit 2
fi

PN=naughtysvn
CFGGLIST="config.h ../config.h"
for I in $CFGGLIST
do
    [ -f "$I" ] && CFGF="$I"
done
if [ -z "$CFGF" ]
then
    cat <<EOM
Could not find config.h. Maybe you didn't run ./configure?
Please run the script from the root of the project after running ./configure.
EOM
    exit 3
fi
OV="`sed -n 's/^#define VERSION "\([0-9]*\.[0-9]*\.[0-9a-zA-Z]*\)"\s*$/\1/p' $CFGF`"
TS="+svn`date +%Y%m%d%H%M`"
PV="$OV$TS"

cat <<EOPINFO
Detected the following information:
    Package: "$PN"
    Version: "$PV"
press enter to confirm, or CTRL+C to stop
EOPINFO

read CONFIRM

checkinstall --pkgname "$PN" --pkgversion "$PV" -D --exclude /selinux/context

# et:sw=4:ts=4

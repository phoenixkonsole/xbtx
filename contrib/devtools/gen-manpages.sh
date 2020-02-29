#!/bin/sh

TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
SRCDIR=${SRCDIR:-$TOPDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

BitcoinSubsidiumD=${BitcoinSubsidiumD:-$SRCDIR/BitcoinSubsidiumd}
BitcoinSubsidiumCLI=${BitcoinSubsidiumCLI:-$SRCDIR/BitcoinSubsidium-cli}
BitcoinSubsidiumTX=${BitcoinSubsidiumTX:-$SRCDIR/BitcoinSubsidium-tx}
BitcoinSubsidiumQT=${BitcoinSubsidiumQT:-$SRCDIR/qt/BitcoinSubsidium-qt}

[ ! -x $BitcoinSubsidiumD ] && echo "$BitcoinSubsidiumD not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little bit
RVNVER=($($BitcoinSubsidiumCLI --version | head -n1 | awk -F'[ -]' '{ print $6, $7 }'))

# Create a footer file with copyright content.
# This gets autodetected fine for BitcoinSubsidiumd if --version-string is not set,
# but has different outcomes for BitcoinSubsidium-qt and BitcoinSubsidium-cli.
echo "[COPYRIGHT]" > footer.h2m
$BitcoinSubsidiumD --version | sed -n '1!p' >> footer.h2m

for cmd in $BitcoinSubsidiumD $BitcoinSubsidiumCLI $BitcoinSubsidiumTX $BitcoinSubsidiumQT; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=${RVNVER[0]} --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-${RVNVER[1]}//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m

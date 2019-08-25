#!/usr/bin/env bash

set -euo pipefail


# Set DEBUG to true for enhanced debugging: run prefixed with "DEBUG=true"
${DEBUG:-false} && set -vx
# Credit to https://stackoverflow.com/a/17805088
# and http://wiki.bash-hackers.org/scripting/debuggingtips
export PS4='+(${BASH_SOURCE}:${LINENO}): ${FUNCNAME[0]:+${FUNCNAME[0]}(): }'

me=`whoami`
if [[ ! -d /media/${me}/RETROFW/test ]]; then
	echo "ERROR: LDK not mounted!"
	exit 1
fi

mkdir -p build.ldk/

rm -rf /media/${me}/RETROFW/test/*
make -f Makefile-ldk.mk -j 12 
cp build.ldk/modus* /media/${me}/RETROFW/test/  
make -f Makefile-ldk.mk clean >/dev/null 2>&1


echo "done"
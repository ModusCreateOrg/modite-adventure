#!/usr/bin/env bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

. ${SCRIPT_DIR}/ldk_scripts_common.sh

cd ${WORKING_DIR}


echo "Compiling"
mkdir -p build/ldk/

make -f Makefile-ldk.mk -j 12 ipk >/dev/null 2>&1
if [[ $? -eq 0 ]]; then
 	make -f Makefile-ldk.mk cleanobjects >/dev/null 2>&1
fi



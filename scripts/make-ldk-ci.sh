#!/usr/bin/env bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

. ${SCRIPT_DIR}/ldk_scripts_common.sh

cd ${WORKING_DIR}


echo "Compiling"
mkdir -p build/ldk/

make -j `nproc` ipk 
if [[ $? -eq 0 ]]; then
 	make cleanobjects
fi



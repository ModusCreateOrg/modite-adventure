#!/usr/bin/env bash

set -euo pipefail

# Set DEBUG to true for enhanced debugging: run prefixed with "DEBUG=true"
${DEBUG:-false} && set -vx
# Credit to https://stackoverflow.com/a/17805088
# and http://wiki.bash-hackers.org/scripting/debuggingtips
export PS4='+(${BASH_SOURCE}:${LINENO}): ${FUNCNAME[0]:+${FUNCNAME[0]}(): }'

export BLUE="\e[34m"
export RED="\e[31m"
export GREEN="\e[32m"
export CYAN="\e[36m"
export COLOR_RESET="\e[0m"
export YELLOW="\e[33m"

function PRINT_RED {
	echo -e "${RED}${1}${COLOR_RESET}"
}

function PRINT_GREEN {
	echo -e "${GREEN}${1}${COLOR_RESET}"
}

function PRINT_BLUE {
	echo -e "${BLUE}${1}${COLOR_RESET}"
}

function PRINT_YELLOW {
	echo -e "${YELLOW}${1}${COLOR_RESET}"
}

function PRINT_CYAN {
	echo -e "${CYAN}${1}${COLOR_RESET}"
}


ROOT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

me=`whoami`
DEPLOY_MOUNT=/media/${me}/RETROFW
LDK_DEVICE_DIR=${DEPLOY_MOUNT}/test



BUILD_DIR=${ROOT_DIR}/build/ldk
CREATIVE_ENGINE_PATH=${ROOT_DIR}/creative-engine
CREATIVE_ENGINE_SOURCE_DIR=${CREATIVE_ENGINE_PATH}/src

TARGET=${BUILD_DIR}/modite-adventure.dge

IPK_ROOT_DIR=${BUILD_DIR}/ipk
IPK_EXEC_DIR=${IPK_ROOT_DIR}/home/retrofw/games/modite-adventure
LDK_FILES_DIR=${ROOT_DIR}/src/resources/ldk-files
LNK_DESTINATION_DIR=${IPK_ROOT_DIR}/home/retrofw/apps/gmenu2x/sections/games/


#### Example IPK File
# See https://docs.google.com/document/d/19kJXO3EZ8XCoeporuUUgV_S93AaPbSagza3sAgBILu8/edit#
#    ipkg/home/retrofw/emus/dosbox/dos.png
#    ipkg/home/retrofw/emus/dosbox/dosbox.png
#    ipkg/home/retrofw/apps/gmenu2x/sections/emulators/dosbox.lnk
#    ipkg/home/retrofw/apps/gmenu2x/sections/systems/dos.dosbox.lnk
#    ipkg/debian-binary
#    ipkg/postinst
#    ipkg/control


#### BEGIN IPK CREATION #####
rm -rf ${IPK_ROOT_DIR}
mkdir -p ${IPK_EXEC_DIR}
mkdir -p ${LNK_DESTINATION_DIR}


# Copy binary and launcher image to the execution directory
cp -r ${TARGET} ${LDK_FILES_DIR}/modite-adventure.png ${IPK_EXEC_DIR}
#rm ${TARGET}

cp ${LDK_FILES_DIR}/modite-adventure.lnk ${LNK_DESTINATION_DIR}

NOW=`date +%s`

sed "s/^Version:.*/Version: ${NOW}/" ${LDK_FILES_DIR}/control > ${IPK_ROOT_DIR}/control
#cp ${LDK_FILES_DIR}/conffiles ${IPK_EXEC_DIR}/

tar --owner=0 --group=0 -czvf ${IPK_ROOT_DIR}/control.tar.gz -C ${IPK_ROOT_DIR} control #conffiles
tar --owner=0 --group=0 -czvf ${IPK_ROOT_DIR}/data.tar.gz -C ${IPK_ROOT_DIR} home/


echo 2.0 > ${IPK_ROOT_DIR}/debian-binary
ar r ${BUILD_DIR}/modite-adventure.ipk ${IPK_ROOT_DIR}/control.tar.gz ${IPK_ROOT_DIR}/data.tar.gz ${IPK_ROOT_DIR}/debian-binary


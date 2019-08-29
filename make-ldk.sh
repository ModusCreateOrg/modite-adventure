#!/usr/bin/env bash

set -euo pipefail

BLUE="\e[34m"
RED="\e[31m"
GREEN="\e[32m"
CYAN="\e[36m"
COLOR_RESET="\e[0m"
YELLOW="\e[33m"

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

# Set DEBUG to true for enhanced debugging: run prefixed with "DEBUG=true"
${DEBUG:-false} && set -vx
# Credit to https://stackoverflow.com/a/17805088
# and http://wiki.bash-hackers.org/scripting/debuggingtips
export PS4='+(${BASH_SOURCE}:${LINENO}): ${FUNCNAME[0]:+${FUNCNAME[0]}(): }'

me=`whoami`
DEPLOY_MOUNT=/media/${me}/RETROFW
DEPLOY_DIR=${DEPLOY_MOUNT}/test 

echo "Compiling"
make -f Makefile-ldk.mk cleanall >/dev/null 2>&1
mkdir -p build.ldk/

MAKE_RESULT=`make -f Makefile-ldk.mk -j 12  >/dev/null`
if [[ ${MAKE_RESULT} -eq 0 ]]; then
	make -f Makefile-ldk.mk cleanobjects >/dev/null 2>&1
fi


me=`whoami`

if [[ ! -d ${DEPLOY_DIR} ]]; then
	echo ""
	PRINT_YELLOW "WARNING: LDK not mounted!"
	echo "Plug in LDK & enable 'Storage' mode. Standing by..."

	#loop to wait
	X=0;
	Y=0;
	while [[ ! -d ${DEPLOY_DIR} ]] || [[ ! -w ${DEPLOY_DIR} ]]; do
		sleep .2

		X=$((X + 1));
	
		if [[ ${Y} -gt 4 ]]; then
			Y=0
			echo " "
		fi
		
		if [[ ${X} -eq 5 ]]; then
			printf "."
			X=0
			Y=$((Y + 1));
		fi


	done
	echo ""
	PRINT_GREEN "Found ${DEPLOY_MOUNT}"
fi


rm -rf ${DEPLOY_DIR}/*

md5=`md5sum build.ldk/modus.dge | awk '{print $1}'`
md5=${md5: -6}

cp build.ldk/modus.dge ${DEPLOY_DIR}/modus.${md5}.dge
cp start-modus.sh ${DEPLOY_DIR}
chmod +x ${DEPLOY_DIR}/start-modus.sh

make -f Makefile-ldk.mk cleanall >/dev/null 2>&1

echo " "
umount ${DEPLOY_MOUNT}
echo unmounted ${DEPLOY_MOUNT}

echo -e "Ready to test build" "${GREEN}${md5}${COLOR_RESET}"
echo "Reboot LDK Game and enable CHARGER mode"

echo " "


LDK=169.254.1.1
# ./tnet.sh
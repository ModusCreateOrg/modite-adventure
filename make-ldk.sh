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


me=`whoami`
DEPLOY_MOUNT=/media/${me}/RETROFW
LDK_DEVICE_DIR=${DEPLOY_MOUNT}/test 

echo "Compiling"
mkdir -p build/ldk/

MAKE_RESULT=`make -f Makefile-ldk.mk -j 12 ipk >/dev/null`
# if [[ ${MAKE_RESULT} -eq 0 ]]; then
# 	make -f Makefile-ldk.mk cleanobjects >/dev/null 2>&1
# fi

me=`whoami`


ping 169.254.1.1 -c 1 -W 0 -i 200
# RC=$?

# if [[ $RC -gt 0 ]]; then
#   PRINT_YELLOW "WARNING: LDK not connected!"
#   echo "Connect LDK and "

#   while true; do 
#     echo "Checking for ${LDK} `date`"

#     ping 169.254.1.1  -c 1 -W 0 -i 200 >/dev/null 2>&1
#     RC=$?

#     if [[ $RC -eq 0 ]]; then
#       break;
#     fi
#   done

#   PRINT_GREEN "Found LDK. Deploying..."
# fi


md5=`md5sum build/ldk/modite-adventure.dge | awk '{print $1}'`
md5=${md5: -6}


ALREADY_DEPLOYED_DIR=/home/retrofw/games/modite-adventure

function deploy_binary_via_ftp {

ftp -n -v $LDK << EOT
user $USER
prompt
lcd build/ldk
cd /home/retrofw/games/modite-adventure
bin
put modite-adventure.dge
quit
EOT

  echo "Deployed latest binary ${md5} to ${ALREADY_DEPLOYED_DIR}"
}


function deploy_ipk_via_ftp {
ftp -n -v $LDK << EOT
user $USER
prompt
lcd build/ldk
cd /home/retrofw/
mkdir tmp
cd tmp/
bin
put modite-adventure.ipk
quit
EOT

  echo ""
  echo "Deployed latest IPK to ${LDK_DEVICE_DIR}"
  echo ""
  PRINT_GREEN "Next Steps:"
  echo " - Launch Explorer on device"
  echo " - Navigate to /home/retrofw/tmp"
  echo " - Install 'modite-adventure.ipk'"
  echo "Further invocations of this script should not require \"ipk\" argument."

}


if [[ $# -gt 0 ]] && [[ "${1}" == "ipk" ]]; then
  deploy_ipk_via_ftp
else
  deploy_binary_via_ftp
fi

# make -f Makefile-ldk.mk cleanall >/dev/null 2>&1



echo " "

echo -e "Ready to test build" "${GREEN}${md5}${COLOR_RESET}"
# echo "Reboot LDK Game and enable CHARGER mode"

echo " "

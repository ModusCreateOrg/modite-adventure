#!/usr/bin/env bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

. ${SCRIPT_DIR}/ldk_scripts_common.sh

cd ${WORKING_DIR}


echo "Compiling"
mkdir -p build/ldk/

make -f Makefile-ldk.mk -j 12 ipk >/dev/null
if [[ $? -eq 0 ]]; then
 	make -f Makefile-ldk.mk cleanobjects >/dev/null 2>&1
fi

me=`whoami`


ping 169.254.1.1 -c 1 -W 0 -i 200
# RC=$?

# if [[ $RC -gt 0 ]]; then
#   PRINT_YELLOW "WARNING: LDK_IP not connected!"
#   echo "Connect LDK_IP and "

#   while true; do
#     echo "Checking for ${LDK_IP} `date`"

#     ping 169.254.1.1  -c 1 -W 0 -i 200 >/dev/null 2>&1
#     RC=$?

#     if [[ $RC -eq 0 ]]; then
#       break;
#     fi
#   done

#   PRINT_GREEN "Found LDK_IP. Deploying..."
# fi


md5=`md5sum build/ldk/modite-adventure.dge | awk '{print $1}'`
md5=${md5: -6}



function deploy_binary_via_ftp {

ftp -n -v $LDK_IP << EOT
user $USER
prompt
passive
lcd build/ldk
cd /home/retrofw/games/modite-adventure
bin
put modite-adventure.dge
quit
EOT

  echo "Deployed latest binary ${md5} to ${ALREADY_DEPLOYED_DIR}"
}


function deploy_ipk_via_ftp {
ftp -n -v $LDK_IP << EOT
user $USER
prompt
passive
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
  echo "Further invocations of this script should not require \"deploy-ipk\" argument."

}


if [[ $# -gt 0 ]] && [[ "${1}" == "deploy-ipk" ]]; then
  deploy_ipk_via_ftp
else
  deploy_binary_via_ftp
fi

# make -f Makefile-ldk.mk cleanall >/dev/null 2>&1



echo " "

echo -e "Ready to test build" "${GREEN}${md5}${COLOR_RESET}"
# echo "Reboot LDK_IP Game and enable CHARGER mode"

echo " "

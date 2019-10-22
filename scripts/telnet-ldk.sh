#!/usr/bin/env bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

. $SCRIPT_DIR/ldk_scripts_common.sh

echo
echo -e "Credentials:${GREEN}root${COLOR_RESET} and ${GREEN}no password.${COLOR_RESET}"
echo -e "Press ${RED}CTRL + ]${COLOR_RESET} and enter ${GREEN}quit${COLOR_RESET} to exit telnet"
echo

telnet ${LDK_IP}


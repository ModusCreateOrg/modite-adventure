#!/usr/bin/env bash

######################### Debugging and Directory Scaffolding #########

# Set bash unofficial strict mode http://redsymbol.net/articles/unofficial-bash-strict-mode/
set -euo pipefail
# our traps need access to some vars to do their job properly.
set -o errtrace
set -o functrace
IFS=$'\n\t'

# Enable for enhanced debugging
${DEBUG:-false} && set -vx

# Credit to https://stackoverflow.com/a/17805088
# and http://wiki.bash-hackers.org/scripting/debuggingtips
export PS4='+(${BASH_SOURCE}:${LINENO}): ${FUNCNAME[0]:+${FUNCNAME[0]}(): }'

# Credit to Stack Overflow questioner Jiarro and answerer Dave Dopson
# http://stackoverflow.com/questions/59895/can-a-bash-script-tell-what-directory-its-stored-in
# http://stackoverflow.com/a/246128/424301
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
BASE_DIR="$DIR/.."
BUILD_DIR="$BASE_DIR/build-ldk"
TOP_DIR="$BASE_DIR/.."
CREATIVE_ENGINE_DIR="${CREATIVE_ENGINE_PATH:-$TOP_DIR/creative-engine}"
RESOURCES_DIR="${RESOURCES_PATH:-$TOP_DIR/modite-adventure-resources}"
export DIR BUILD_DIR TOP_DIR CREATIVE_ENGINE_DIR RESOURCES_DIR
# shellcheck disable=SC1090
source "$DIR/common.sh"

######################### Main CI LDK build ##################################
echo "Starting LDK build."
if [[ "$(uname)" != "Linux" ]]; then
  echo "LDK can only builds in Linux, skipping!"
  exit 0;
fi

if [ -d "$HOME/ldk" ]; then
  echo "Previously Installed: mipsel-linux-uclibc_x64.tar.gz"
else
  echo "Installing: mipsel-linux-uclibc_x64.tar.gz"
  mkdir "$HOME/ldk"
  cd "$HOME/ldk"
  wget https://github.com/retrofw/buildroot/releases/download/2018.02.11/mipsel-linux-uclibc_x64.tar.gz
  tar -zxf mipsel-linux-uclibc_x64.tar.gz
  cd mipsel-linux-uclibc
  ./relocate-sdk.sh
fi

cd "$BASE_DIR"

echo "Starting LDK build: $(pwd)"
rm -f "$CREATIVE_ENGINE_DIR"/src/*.o
./scripts/make-ldk.sh
echo "Finished LDK build."


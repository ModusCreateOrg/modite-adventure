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
BUILD_DIR="$BASE_DIR/build"

if [[ "${TRAVIS:-}" == "true" ]]; then
  if [[ "${TRAVIS_OS_NAME:-}" == "osx" ]];then
    "$BUILD_DIR/test"
  else
    echo "TBD - make test work on headless (Travis) linux."
  fi
else
  "$BUILD_DIR/test"
fi



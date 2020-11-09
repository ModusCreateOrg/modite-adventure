#!/usr/bin/env bash

OS="$(uname)"

if [ "$OS" == "Darwin" ]; then
  chmod +x /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app/Contents/MacOS/Modite
  cd /Users/travis/build/ModusCreateOrg/modite-adventure/build && zip --symlinks -r modite-mac.zip Modite.app/ && cd -
elif [ "$OS" == "Linux" ]; then
  cd /home/travis/build/ModusCreateOrg/modite-adventure/build && zip --symlinks modite-linux.zip Modite && cd -
fi

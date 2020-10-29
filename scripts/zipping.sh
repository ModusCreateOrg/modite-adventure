#!/usr/bin/env bash

#CREDIT TO GOOGLE LOLz
OS="$(uname)"

if [ "$OS" == "Darwin" ]; then
  cd /Users/travis/build/ModusCreateOrg/modite-adventure/build && zip --symlinks -r modite-mac.zip Modite.app/ && cd -
elif [ "$OS" == "Linux" ]; then
  cd /home/travis/build/ModusCreateOrg/modite-adventure/build && zip --symlinks modite-linux.zip Modite && cd -
fi

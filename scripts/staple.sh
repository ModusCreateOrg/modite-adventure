#!/usr/bin/env bash

OS="$(uname)"

if [[ "$OS" == "Darwin" ]]; then
  xcrun stapler staple /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app

  rm /Users/travis/build/ModusCreateOrg/modite-adventure/build/modite-mac.zip

  cd /Users/travis/build/ModusCreateOrg/modite-adventure/build && zip --symlinks -r modite-mac.zip Modite.app/ && cd -
fi

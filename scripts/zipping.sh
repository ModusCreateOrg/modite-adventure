#!/usr/bin/env bash

OS="$(uname)"

if [ "$OS" == "Darwin" ]; then
  chmod +x "/Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app/Contents/MacOS/libs/libwebp.dylib"
  chmod +x "/Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app/Contents/MacOS/libs/libtiff.dylib"
  chmod +x "/Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app/Contents/MacOS/libs/libSDL2.dylib"
  chmod +x "/Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app/Contents/MacOS/libs/libSDL2_image.dylib"
  chmod +x "/Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app/Contents/MacOS/libs/libpng.dylib"
  chmod +x "/Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app/Contents/MacOS/libs/libjpeg.dylib"
  chmod +x "/Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app/Contents/MacOS/Modite"
  chmod +x "/Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app/Contents/MacOS/Modite.bin"
  chmod +x "/Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app"
  cd /Users/travis/build/ModusCreateOrg/modite-adventure/build && zip --symlinks -r modite-mac.zip Modite.app/ && cd -
elif [ "$OS" == "Linux" ]; then
  cd /home/travis/build/ModusCreateOrg/modite-adventure/build && zip --symlinks modite-linux.zip Modite && cd -
fi

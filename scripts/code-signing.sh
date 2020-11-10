#!/usr/bin/env bash

OS="$(uname)"

if [[ "$OS" == "Darwin" ]]; then
  echo "Code signing..."

  codesign --verbose --timestamp --options=runtime -s "Developer ID Application: Modus Create, Inc. (287TS9B2H2)" --keychain /Users/travis/Library/Keychains/ios-build.keychain /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app/Contents/MacOS/libs/libSDL2_image.dylib
  codesign --force --verbose --timestamp --options=runtime -s "Developer ID Application: Modus Create, Inc. (287TS9B2H2)" --keychain /Users/travis/Library/Keychains/ios-build.keychain /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app

  echo "Verifying signature..."

  codesign -v --deep --verbose=2 /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app/Contents/MacOS/libs/libSDL2_image.dylib
  codesign -v --deep --verbose=2 /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app

  echo "spctl Verification..."

  spctl --assess -vv /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app/Contents/MacOS/libs/libSDL2_image.dylib
  spctl --assess -vv /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app
fi

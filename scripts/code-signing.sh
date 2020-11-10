#!/usr/bin/env bash

OS="$(uname)"

if [[ "$OS" == "Darwin" ]]; then
  echo "Code signing..."
  codesign --force --verbose --timestamp --options=runtime -s "Developer ID Application: Modus Create, Inc. (287TS9B2H2)" --keychain /Users/travis/Library/Keychains/ios-build.keychain /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app/Contents/MacOS/Modite
  codesign --force --verbose --timestamp --options=runtime -s "Developer ID Application: Modus Create, Inc. (287TS9B2H2)" --keychain /Users/travis/Library/Keychains/ios-build.keychain /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app

  echo "Verifying signature..."
  codesign -v --deep --verbose=2 /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app/Contents/MacOS/Modite
  codesign -v --deep --verbose=2 /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app

  echo "spctl Verification..."
  spctl --assess -vv /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app/Contents/MacOS/Modite
  spctl --assess -vv /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app
fi

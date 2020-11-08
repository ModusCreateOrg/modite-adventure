#!/usr/bin/env bash

OS="$(uname)"
DEVELOPER_NAME="Developer ID Application: Modus Create, Inc. (287TS9B2H2)"
PATH="/Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app"

if [[ "$OS" == "Darwin" ]]; then
  echo "Code signing..."

  codesign -f -s $DEVELOPER_NAME --timestamp --options runtime --keychain /Users/travis/Library/Keychains/ios-build.keychain $PATH

  echo "Verifying signature..."

  codesign -v --deep --verbose=2 /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app

  spctl --assess -vv /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app
fi

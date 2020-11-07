#!/usr/bin/env bash

OS="$(uname)"
DEVELOPER_NAME="Developer ID Application: Modus Create, Inc. (287TS9B2H2)"

if [[ "$OS" == "Darwin" ]]; then
  echo "Code signing..."

  codesign -f --timestamp --options runtime -s "Developer ID Application: Modus Create, Inc. (287TS9B2H2)" /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app

  echo "Verifying signature..."

  codesign -v --strict --deep --verbose=2 /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app

  spctl --assess -vv /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app
fi

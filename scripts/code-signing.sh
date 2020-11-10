#!/usr/bin/env bash

OS="$(uname)"

if [[ "$OS" == "Darwin" ]]; then
  echo "Code signing..."
  codesign --verbose --timestamp --options=runtime -s "Developer ID Application: Modus Create, Inc. (287TS9B2H2)" --keychain /Users/travis/Library/Keychains/ios-build.keychain /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app/Contents/MacOS/libs/libjpeg.dylib
  codesign --verbose --timestamp --options=runtime -s "Developer ID Application: Modus Create, Inc. (287TS9B2H2)" --keychain /Users/travis/Library/Keychains/ios-build.keychain /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app/Contents/MacOS/libs/libpng.dylib
  codesign --verbose --timestamp --options=runtime -s "Developer ID Application: Modus Create, Inc. (287TS9B2H2)" --keychain /Users/travis/Library/Keychains/ios-build.keychain /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app/Contents/MacOS/libs/libSDL2_image.dylib
  codesign --verbose --timestamp --options=runtime -s "Developer ID Application: Modus Create, Inc. (287TS9B2H2)" --keychain /Users/travis/Library/Keychains/ios-build.keychain /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app/Contents/MacOS/libs/libSDL2.dylib
  codesign --verbose --timestamp --options=runtime -s "Developer ID Application: Modus Create, Inc. (287TS9B2H2)" --keychain /Users/travis/Library/Keychains/ios-build.keychain /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app/Contents/MacOS/libs/libtiff.dylib
  codesign --verbose --timestamp --options=runtime -s "Developer ID Application: Modus Create, Inc. (287TS9B2H2)" --keychain /Users/travis/Library/Keychains/ios-build.keychain /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app/Contents/MacOS/libs/libwebp.dylib
  codesign --force --verbose --timestamp --options=runtime -s "Developer ID Application: Modus Create, Inc. (287TS9B2H2)" --keychain /Users/travis/Library/Keychains/ios-build.keychain /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app/Contents/MacOS/Modite
  codesign --force --verbose --timestamp --options=runtime -s "Developer ID Application: Modus Create, Inc. (287TS9B2H2)" --keychain /Users/travis/Library/Keychains/ios-build.keychain /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app/Contents/MacOS/Modite.bin
  codesign --force --verbose --timestamp --options=runtime -s "Developer ID Application: Modus Create, Inc. (287TS9B2H2)" --keychain /Users/travis/Library/Keychains/ios-build.keychain /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app

  echo "Verifying signature..."
  codesign -v --deep --verbose=2 /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app/Contents/MacOS/libs/libjpeg.dylib
  codesign -v --deep --verbose=2 /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app/Contents/MacOS/libs/libpng.dylib
  codesign -v --deep --verbose=2 /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app/Contents/MacOS/libs/libSDL2_image.dylib
  codesign -v --deep --verbose=2 /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app/Contents/MacOS/libs/libSDL2.dylib
  codesign -v --deep --verbose=2 /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app/Contents/MacOS/libs/libtiff.dylib
  codesign -v --deep --verbose=2 /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app/Contents/MacOS/libs/libwebp.dylib
  codesign -v --deep --verbose=2 /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app/Contents/MacOS/Modite
  codesign -v --deep --verbose=2 /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app/Contents/MacOS/Modite.bin
  codesign -v --deep --verbose=2 /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app

  echo "spctl Verification..."
  spctl --assess -vv /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app/Contents/MacOS/libs/libjpeg.dylib
  spctl --assess -vv /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app/Contents/MacOS/libs/libpng.dylib
  spctl --assess -vv /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app/Contents/MacOS/libs/libSDL2_image.dylib
  spctl --assess -vv /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app/Contents/MacOS/libs/libSDL2.dylib
  spctl --assess -vv /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app/Contents/MacOS/libs/libtiff.dylib
  spctl --assess -vv /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app/Contents/MacOS/libs/libwebp.dylib
  spctl --assess -vv /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app/Contents/MacOS/Modite
  spctl --assess -vv /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app/Contents/MacOS/Modite.bin
  spctl --assess -vv /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app
fi

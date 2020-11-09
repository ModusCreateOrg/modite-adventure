#!/usr/bin/env bash

OS="$(uname)"
USER="thomas.collins@moduscreate.com"

if [[ "$OS" == "Darwin" ]]; then
  echo "notarizing..."

  xcrun altool --notarize-app --primary-bundle-id "com.moduscreate.modite-adventure" -u $USER -p ${APP_PASS} --asc-provider 287TS9B2H2 --file /Users/travis/build/ModusCreateOrg/modite-adventure/build/Modite.app
fi

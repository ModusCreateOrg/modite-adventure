#!/usr/bin/env bash

OS="$(uname)"

if [[ "$OS" == "Darwin" ]]; then
  echo "notarizing..."
fi

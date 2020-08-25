#!/bin/bash

PIN_URL="https://software.intel.com/sites/landingpage/pintool/downloads/pin-3.11-97998-g7ecce2dac-gcc-linux.tar.gz"

if [ ! -d "pin" ]; then
  echo "Downloading and unpacking pin..."
  curl -OL ${PIN_URL}
  mkdir pin
  tar xvf *.tar.gz -C pin --strip-components 1
  rm *.tar.gz
  chown -R vagrant:vagrant pin
fi

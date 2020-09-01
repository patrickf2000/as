#!/bin/bash

if [[ ! -f build/src/asmx86 ]] ; then
    echo "Error: The assembler does not seem to be built."
    exit 1
fi

if [[ ! -f build/stdlib/libasx86.a ]] ; then
    echo "Error: The standard library does not seem to be built."
    exit 1
fi

sudo cp build/src/asmx86 /usr/local/bin
sudo cp build/stdlib/libasx86.a /usr/lib

echo "Done"
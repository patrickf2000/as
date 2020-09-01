#!/bin/bash

if [[ ! -f build/src/asmx86 ]] ; then
    echo "Error: The assembler does not seem to be built."
    exit 1
fi

echo "Building standard library"
echo "You can find it in build/stdlib/libasx86.so"

mkdir -p build/stdlib

for entry in stdlib/*.asm
do
    name=`basename $entry .asm`
    build/src/asmx86 $entry -o build/stdlib/$name.o
done

ar -crs build/stdlib/libasx86.a build/stdlib/*.o

echo "Done"



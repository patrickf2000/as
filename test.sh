#!/bin/bash

function clean() {
    if [[ -f data.asm ]] ; then
        rm data.asm
    fi
    
    if [[ -f text.asm ]] ; then
        rm text.asm
    fi
}

echo "Running all tests..."
echo ""

if [ ! -d ./build ] ; then
	echo "Error: The assembler does not seem to be built."
	exit 1
fi

mkdir -p ./build/test

for entry in test/*.asm
do
	name=`basename $entry .asm`
	
	./build/src/asmx86 $entry -o ./build/test/$name.o
    ld ./build/test/$name.o -o ./build/test/$name
	./test.py $entry ./build/test/$name
	
	if [[ $? != 0 ]] ; then
        clean
		exit 1
	fi
done

clean

echo "Done"

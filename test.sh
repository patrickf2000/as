#!/bin/bash

echo "Running all tests..."
echo ""

if [ ! -d ./build ] ; then
	echo "Error: The assembler does not seem to be built."
	exit 1
fi

mkdir -p ./build/test

for entry in test/*
do
	name=`basename $entry .asm`
	
	./build/src/asm86 $entry -o ./build/test/$name
	./test.py $entry ./build/test/$name
	
	if [[ $? != 0 ]] ; then
		exit 1
	fi
done

echo "Done"
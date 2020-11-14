#!/bin/bash

function run_test() {
	for entry in $1
	do
		name=`basename $entry .asm`
		
		./build/src/asmx86 $entry -o ./build/test/$name.o
        
        if [[ $? != 0 ]] ; then
            exit 1
        fi
        
        if [[ $2 == "link" ]] ; then
            ld ./build/test/$name.o -o ./build/test/$name -lasx86 -L./build/stdlib
        else
	        ld ./build/test/$name.o -o ./build/test/$name
        fi
        
		./test.py $entry ./build/test/$name
		
		if [[ $? != 0 ]] ; then
			exit 1
		fi
	done
}

echo "Running all tests..."
echo ""

if [ ! -d ./build ] ; then
	echo "Error: The assembler does not seem to be built."
	exit 1
fi

./stdlib.sh

mkdir -p ./build/test

run_test 'test/cmp/*.asm'
run_test 'test/func/*.asm'
run_test 'test/generic/*.asm'
run_test 'test/mem/*.asm'
run_test 'test/stdlib/*.asm' "link"
run_test 'test/unit/add/*.asm'
run_test 'test/unit/cmp/*.asm'
run_test 'test/unit/mov/*.asm'
run_test 'test/unit/sub/*.asm'
run_test 'test/unit/mul/*.asm'
run_test 'test/stdlib/*.asm' "link"

echo "Done"

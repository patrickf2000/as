## Asmx86

Asmx86 is somewhat simple x86-64 assembler. It can assemble a variety of instructions (with it being easy to add more), and generates relocatable ELF object files. Asmx86 is written in C and doesn't use any libraries; I wrote the entire thing myself, except for the parser which uses Flex and Bison.

### Why?

The purpose of this project is really a learning exercise. I'm really interested in computer architecture and compilers; I have written several toy compilers, and once I learned how to get from a high level language to assembly, I wanted to learn how to get from assembly to the final binary form. I also wanted to learn more about OS executables and the linking process, which is why I'm generating ELF files. In addition, I'm thinking about integrating this with one of my compilers. This project was actually part of a new compiler I started (which is why there's a gap in the early commit history), but I separated it into its own project when it actually began to work.

### Syntax

Asmx86 uses the Intel syntax. Its very close to the NASM syntax, but there are a few differences. Please note that I may change some of this as time goes on, but I'll try not to make large breaking changes.

Although there are separate .data and .text sections generated, you don't specify that in your source. At this point, the only thing supported in the data section are strings, so the assembler will take care of parsing and determining that. Strings are declared like this: ```MyString .string "Hello!\n"```.

I'm still having issues getting external functions working, but the syntax is in place. For telling the assembler that you wish to use an external function, use ```extern MyFunc```. To make a function visible outside the compilation unit, use ```global MyFunc:```. Purely local functions are written as ```MyFunc:```.

### Instructions

The x86-64 architecture is huge- even without the extensions. It is highly unlikely I'm going to add support for every single instruction. That said, it is important to understand that you don't need support for every single instruction for the majority of programs. I'm trying to add support for the most commonly used instructions first, and then I'll add others on an as-needed basis. 

If you want to add an instruction, its actually super easy. All instructions are first specified in the "grammar.y" file. To add your instruction, simply add the syntax, the size of the instruction, and the if statement checking the pass (you can use what's there as an example). Then, create a function in the "asm" folder and header to generate the binary for it.

### ELF Objects

Asmx86 generates relocatable ELF object files. However, this is to a somewhat limited capacity. They can be linked and turned into executable programs using the system linker, but that's currently it. I have plans to expand this to work with multiple object files and libraries, but this may still be a little down the road. I'll be honest though, I'm really happy with what does work.

### Testing and Development

I use test-driven development for this project. Even though its a hobby project, I've found this is a good method as a whole to use. The test system is really simple to use; the ```test.sh``` script runs all the tests, and the ```test.py``` script runs the test for a particular file (its called by test.sh). The tests are currently conducted based on return code and output. Output is primarily used for testing things like functions, strings, jumps, and so forth. The return code is used for testing numerical instructions, data moves, logical instructions, and so forth. That said, I use Linux system calls, so if you use a different Unix-like platform, you may have to adjust this part. As long as ELF is supported, this should be all you have to change.

The entire assembler is in the ```src``` folder. The files in the ```src``` root are for the parser. The files in ```src/elf``` are for generating the ELF objects. The files in ```src/asm``` are for generating x86 binary. The ```src/asm/utils.c``` contains several useful functions used across the binary generating part of the program.

### Building and Running

To build, all you need is a C compiler, Flex, Bison, and CMake.

Asmx86 should work on any x86-64 Unix-like system that uses the ELF format. I've only tested it on Linux, but it should work on the BSD systems and Solaris. In both cases, you'll probably have to change the system calls in the tests.

As far as i386 goes, in theory this should work without any issues, but Asmx86 won't stop you from generating x86-64 instructions. So pretty much if you wish to use it on i386, just avoid the x86-64 instructions and it should work.

### Known Bugs

In order to track symbol positions, I use a home-made hashing symbol table. That said, it is not perfect. There are a few instances where it hashes out to the same value despite the strings being different, so if you notice odd behaviour, this might be it. (For example, I had issues with two strings named ```str1``` and ```str2```. Changing it to ```STR1``` and ```STR2``` seemed to work though).

Secondly, I know I said this earlier but linking multiple objects currently doesn't work (at least as long as the objects were both generated by Asmx86. I haven't tested others). 

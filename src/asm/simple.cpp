
#include <asm/amd64.hpp>

void amd64_cdqe(FILE *file) {
    fputc(0x48, file);
    fputc(0x98, file);
}

// Write the syscall instruction
// 0F 05
void amd64_syscall(FILE *file) {
    fputc(0x0F, file);
    fputc(0x05, file);
}

// Write the leave instruction
// C9
void amd64_leave(FILE *file) {
    fputc(0xC9, file);
}


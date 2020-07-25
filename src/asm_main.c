#include <stdio.h>
#include <stdlib.h>

#include "asm.h"
#include "elf_bin.h"

int main(int argc, char *argv[]) 
{
    FILE *file = fopen("out.bin", "w");
    
    // Write the ELF stuff
    elf_write_header(file);
    elf_write_pheader(file);
    
    // Setup the stack
    amd64_push_reg64(RBP, file);
    amd64_mov_rr(RBP, RSP, file);
    amd64_sub_ri(RSP, 16, file);
    
    // Now load memory
    // mov dword [rbp-4], 4
    // mov dword [rbp-8], 7
    // mov dword [rbp-12], 0
    amd64_mov_m_int(RBP, -4, 4, file);
    amd64_mov_m_int(RBP, -8, 7, file);
    amd64_mov_m_int(RBP, -12, 0, file);
    
    // Move memory location to registers
    // mov eax, [rbp-4]
    // mov ebx, [rbp-8]
    amd64_mov_reg32_mem(EAX, RBP, -4, file);
    amd64_mov_reg32_mem(EBX, RBP, -8, file);
    
    // Add two registers
    // add eax, ebx
    amd64_add_rr32(EAX, EBX, file);
    
    // Move register to memory location
    // mov [rbp-12], eax
    amd64_mov_m_reg32(RBP, -12, EAX, file);
    
    //Move immediate to register
    //mov rax, 60
    amd64_mov_reg32_imm(EAX, 60, file);
    
    //Move memory location to register
    //mov rdi, [rbp-12]
    amd64_mov_reg64_mem(RDI, RBP, -12, file);
    
    //syscall
    amd64_syscall(file);
    
    fclose(file);

    return 0;
}

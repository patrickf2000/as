;OUTPUT
;END

;RET 105

section .text
global _start

_start:
    mov eax, 1
    mov ebx, 2
    mov ecx, 3
    mov edx, 4
    mov edi, 5
    mov esi, 6
    mov r8d, 7
    mov r9d, 8
    mov r10d, 9
    mov r11d, 10
    mov r12d, 11
    mov r13d, 12
    mov r14d, 13
    mov r15d, 14
    
    add ebx, eax
    add ebx, ecx
    add ebx, edx
    add ebx, edi
    add ebx, esi
    add ebx, r8d
    add ebx, r9d
    add ebx, r10d
    add ebx, r11d
    add ebx, r12d
    add ebx, r13d
    add ebx, r14d
    add ebx, r15d
    
    mov edi, ebx
    mov eax, 60
    syscall

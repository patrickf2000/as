;OUTPUT
;END

;RET 105

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
    
    add eax, ebx
    add eax, ecx
    add eax, edx
    add eax, edi
    add eax, esi
    add eax, r8d
    add eax, r9d
    add eax, r10d
    add eax, r11d
    add eax, r12d
    add eax, r13d
    add eax, r14d
    add eax, r15d
    
    mov edi, eax
    mov eax, 60
    syscall

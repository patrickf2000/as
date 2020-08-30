;OUTPUT
;END

;RET 5

global _start:
    mov eax, 5
    mov ebx, eax
    mov ecx, ebx
    mov edx, ecx
    mov edi, edx
    mov esi, edi
    
    mov r8d, esi
    mov r9d, r8d
    mov r10d, r9d
    mov r11d, r10d
    mov r12d, r11d
    mov r13d, r12d
    mov r14d, r13d
    mov r15d, r14d
    
    mov edi, r15d
    mov eax, 60
    syscall

    

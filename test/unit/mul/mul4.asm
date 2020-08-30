;OUTPUT
;END

;RET 144

global _start:
    mov eax, 2
    mov ebx, 3
    mov ecx, 4
    
    imul ebx
    imul ecx
    
    mov r15d, 6
    mov r14d, 1
    
    imul r15d
    imul r14d
    
    mov edi, eax
    
    mov eax, 60
    syscall

    

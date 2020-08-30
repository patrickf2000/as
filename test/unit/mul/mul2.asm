;OUTPUT
;END

;RET 60

global _start:
    mov eax, 5
    mov ebx, 3
    imul eax, ebx
    
    mov r15d, 4
    imul eax, r15d
    
    mov edi, eax
    mov eax, 60
    syscall

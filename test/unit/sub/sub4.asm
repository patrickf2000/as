;OUTPUT
;END

;RET 3

global _start:
    mov edx, 10
    mov ecx, 5
    mov eax, 2
    
    sub edx, ecx
    sub edx, eax
    
    mov edi, edx
    mov eax, 60
    syscall

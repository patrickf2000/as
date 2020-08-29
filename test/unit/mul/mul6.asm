;OUTPUT
;END

;RET 64

_start:
    mov eax, 5
    mov ebx, 8
    imul eax, ebx, 3
    
    imul ebx, 5
    
    add eax, ebx
    mov edi, eax
    
    mov eax, 60
    syscall

;OUTPUT
;END

;RET 48

_start:
    mov rax, 5
    mov rbx, 6
    imul rax, rbx, 3
    
    imul rbx, 5
    
    add rax, rbx
    mov rdi, rax
    
    mov rax, 60
    syscall

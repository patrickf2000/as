;OUTPUT
;END

;RET 3

_start:
    mov rax, 1
    mov rbx, 2
    add rax, rbx
    
    mov rdi, rax
    mov rax, 60
    syscall

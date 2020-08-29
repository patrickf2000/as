;OUTPUT
;END

;RET 60

_start:
    mov rax, 5
    mov rbx, 3
    imul rax, rbx
    
    mov r15, 4
    imul rax, r15
    
    mov rdi, rax
    mov rax, 60
    syscall

;OUTPUT
;END

;RET 2

global _start:
    mov r15, 10
    mov r14, 5
    mov r13, 3
    
    sub r15, r14
    sub r14, r13
    
    mov rdi, r14
    mov rax, 60
    syscall

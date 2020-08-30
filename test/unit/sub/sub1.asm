;OUTPUT
;END

;RET 100

global _start:
    mov rax, 100
    mov rbx, 90
    
    sub rax, 20
    sub rbx, 30
    
    add rax, rbx
    sub rax, 40
    
    mov rdi, rax
    mov rax, 60
    syscall

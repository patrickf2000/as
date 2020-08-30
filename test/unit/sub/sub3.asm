;OUTPUT
;END

;RET 2

global _start:
    mov rdx, 10
    mov rcx, 5
    mov rax, 3
    
    sub rdx, rcx
    sub rdx, rax
    
    mov rdi, rdx
    mov rax, 60
    syscall

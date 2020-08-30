;OUTPUT
;END

;RET 7

global _start:
    mov rax, 7
    mov rbx, rax
    mov rcx, rbx
    mov rdx, rcx
    mov rdi, rdx
    mov rsi, rdi
    
    mov r8, rsi
    mov r9, r8
    mov r10, r9
    mov r11, r10
    mov r12, r11
    mov r13, r12
    mov r14, r13
    mov r15, r14
    
    mov rdi, r15
    mov rax, 60
    syscall

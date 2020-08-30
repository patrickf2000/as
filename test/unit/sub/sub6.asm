;OUTPUT
;END

;RET 100

global _start:
    mov r10, 100
    mov r11, 90
    
    sub r10, 20
    sub r11, 30
    
    add r10, r11
    sub r10, 40
    
    mov rdi, r10
    mov rax, 60
    syscall

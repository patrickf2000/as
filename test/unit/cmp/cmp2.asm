;OUTPUT
;Equal
;Not equal
;END

;OUTPUT 0

eq .string "Equal\n"
neq .string "Not equal\n"

_start:
    mov rax, 5
    cmp rax, 10
    je wrong
    
    mov rax, 1
    mov rdi, 1
    mov rsi, eq
    mov rdx, 6
    syscall
    
    mov rbx, 10
    cmp rbx, 10
    jne wrong
    
    mov rax, 1
    mov rdi, 1
    mov rsi, neq
    mov rdx, 10
    syscall
    
    mov rax, 60
    mov rdi, 0
    syscall
    
wrong:
    mov rax, 60
    mov rdi, 1
    syscall
    
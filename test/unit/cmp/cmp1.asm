;OUTPUT
;Equal
;Not equal
;END

;OUTPUT 0

section .data
    eq .string "Equal\n"
    neq .string "Not equal\n"

section .text
global _start

_start:
    mov eax, 5
    cmp eax, 10
    je wrong
    
    mov rax, 1
    mov rdi, 1
    mov rsi, eq
    mov rdx, 6
    syscall
    
    mov ebx, 10
    cmp ebx, 10
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
    

;OUTPUT
;Not equal
;END

;RET 4

eq .string "Equal!\n"
neq .string "Not equal\n"

global _start:
    mov eax, 2
    cmp eax, 10
    je equal
    jmp nequal

equal:
    mov rax, 1
    mov rdi, 1
    mov rsi, eq
    mov rdx, 7
    syscall
    jmp done
    
nequal:
    mov rax, 1
    mov rdi, 1
    mov rsi, neq
    mov rdx, 10
    syscall

done:
    mov rax, 60
    mov rdi, 4
    syscall
    

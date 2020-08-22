;OUTPUT
;Hi
;Hi
;Hi
;Hi
;Hi
;Hi
;Hi
;Hi
;Hi
;Hi
;END

;RET 3

str1 .string "Hi\n"

_start:
    mov ebx, 0

lp:
    mov rax, 1
    mov rdi, 1
    mov rsi, str1
    mov rdx, 3
    syscall
    
    add ebx, 1
    
    cmp ebx, 10
    jl lp
    
    mov rax, 60
    mov rdi, 3
    syscall

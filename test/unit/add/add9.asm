;OUTPUT
;END

;RET 20

global _start:
    push rbp
    mov rbp, rsp
    
    mov dword [rbp-4], 10
    add dword [rbp-4], 10
    mov edi, [rbp-4]
    
    mov rax, 60
    syscall
    

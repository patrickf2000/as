;OUTPUT
;END

;RET 10

_start:
    push rbp
    mov rbp, rsp
    sub rsp, 16
    
    mov dword [rbp-4], 1
    mov dword [rbp-8], 2
    mov dword [rbp-12], 3
    mov dword [rbp-16], 4
    
    mov rax, [rbp-4]
    add rax, [rbp-8]

    mov rcx, [rbp-12]
    add rcx, [rbp-16]
    
    add rax, rcx
    
    mov rdi, rax
    mov rax, 60
    syscall


;OUTPUT
;END

;RET 17

_start:
    push rbp
    mov rbp, rsp
    sub rsp, 16
        
    mov dword [rbp-4], 10
    mov dword [rbp-8], 7
    mov dword [rbp-12], 0
        
    mov eax, [rbp-4]
    mov ebx, [rbp-8]
    add eax, ebx
    mov [rbp-12], eax
    
    mov rax, 60
    mov rdi, [rbp-12]
    syscall

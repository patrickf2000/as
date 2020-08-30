extern strlen

global puts:
    call strlen
    mov edx, eax

    mov rsi, rdi
    mov rax, 1
    mov rdi, 1
    syscall
    
    ret
     

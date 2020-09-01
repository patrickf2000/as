extern strlen

global puts:
    call strlen
    mov edx, eax
    sub edx, 1

    mov rsi, rdi
    mov rax, 1
    mov rdi, 1
    syscall
    
    ret
     

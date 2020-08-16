msg .string "Hello!\n"
msg2 .string "How are you?\n"

puts:
    mov rsi, rax
    mov rdx, rbx
    mov rax, 1
    mov rdi, 1
    syscall
    ret

main:
    mov rax, msg
    mov rbx, 7
    call puts
    
    mov rax, msg2
    mov rbx, 13
    call puts
    
    mov rax, 60
    mov rdi, 3
    syscall
    
    
    

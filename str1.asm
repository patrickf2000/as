str .string "Hello!\n"

mov rax, 1
mov rdi, 1
mov rsi, str
mov rdx, 7
syscall

mov rax, 60
mov rdi, 4
syscall

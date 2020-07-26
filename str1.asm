str1 .string "Hello!\n"
str2 .string "Hi!\n"

jmp label2

label1:
    mov rax, 1
    mov rdi, 1
    mov rsi, str1
    mov rdx, 7
    syscall
    jmp done

label2:
    mov rax, 1
    mov rdi, 1
    mov rsi, str2
    mov rdx, 4
    syscall
    jmp label1

done:
    mov rax, 60
    mov rdi, 4
    syscall

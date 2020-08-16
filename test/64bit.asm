;OUTPUT
;END

;RET 14

mov r14d, 5
mov r15d, 9
add r14d, r15d

mov rax, 60
mov rdi, r14
syscall

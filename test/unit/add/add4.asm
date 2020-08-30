;OUTPUT
;END

;RET 0

global _start:
    ; Test 1
    mov eax, 6
    add eax, 5
    
    cmp eax, 11
    jne error
    
    ; Test 2
    mov ecx, 99
    mov edx, 22
    add ecx, edx
    
    cmp ecx, 121
    jne error
    
    mov rax, 60
    mov rdi, 0
    syscall
    
error:
    mov rax, 60
    mov rdi, 1
    syscall

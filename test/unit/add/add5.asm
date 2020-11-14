;OUTPUT
;END

;RET 0

section .text
global _start

_start:
    push rbp
    mov rbp, rsp
    sub rsp, 16
    
    ; Test 1
    mov dword [rbp-4], 1
    mov dword [rbp-8], 2
    mov dword [rbp-12], 3
    mov dword [rbp-16], 4
    
    mov eax, [rbp-4]
    mov ebx, [rbp-8]
    add eax, ebx
    
    mov ecx, [rbp-12]
    mov edx, [rbp-16]
    add ecx, edx
    
    add eax, ecx
    
    cmp eax, 10
    jne error
    
    ; Test 2
    mov eax, [rbp-12]
    add eax, [rbp-16]
    
    cmp eax, 7
    jne error
    
    mov rax, 60
    mov rdi, 0
    syscall
    
error:
    mov edi, eax
    mov rax, 60
    syscall

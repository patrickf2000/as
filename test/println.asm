STR0 .string "Hello!\n"
STR1 .string "How are you today?\n"

strlen:
    xor ecx, ecx
lp:
	mov dl, [rdi+rcx]
	;inc ecx
    add ecx, 1
	cmp dl, 0xA
	jne lp
    
    mov eax, ecx
    ret
    
println:
    call strlen
    mov edx, eax

    mov rsi, rdi
    mov rax, 1
    mov rdi, 1
    syscall
    
    ret
   
_start:
    mov rdi, STR0
    call println
    
    mov rdi, STR1
    call println

    mov rax, 60
    mov edi, 0
	syscall


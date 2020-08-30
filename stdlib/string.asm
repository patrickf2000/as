
global strlen:
    xor ecx, ecx
lp:
	mov dl, [rdi+rcx]
	;inc ecx
    add ecx, 1
	cmp dl, 0xA
	jne lp
    
    mov eax, ecx
    ret

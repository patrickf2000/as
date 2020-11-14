section .text
global strlen

strlen:
    xor ecx, ecx
lp:
	mov dl, [rdi+rcx]
	;inc ecx
    add ecx, 1
    cmp dl, 0x0
	jne lp
    
    mov eax, ecx
    ret

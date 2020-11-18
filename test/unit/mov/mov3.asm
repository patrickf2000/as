;OUTPUT
;END

;RET 5

section .text
global _start

_start:
    mov ax, 5
    mov bx, ax
    mov cx, bx
    mov dx, cx
    mov di, dx
    mov si, di
    
    mov r8w, si
    mov r9w, r8w
    mov r10w, r9w
    mov r11w, r10w
    mov r12w, r11w
    mov r13w, r12w
    mov r14w, r13w
    mov r15w, r14w
    
    mov di, r15w
    mov eax, 60
    syscall

    

;OUTPUT
;Hello!
;How are you today?
;END

;RET 5

STR0 .string "Hello!\n"
STR1 .string "How are you today?\n"

extern puts
extern exit
   
global _start:
    mov rdi, STR0
    call puts
    
    mov rdi, STR1
    call puts

    mov rdi, 5
    call exit
    

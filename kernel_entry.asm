; Ensures we jump accurately into the C main function
[bits 32]
[external main]
call main
jmp $

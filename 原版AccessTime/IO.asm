TITLE Time.asm
.code
PUBLIC TimeRead
TimeRead PROC
	push rbp
	mov rbp, rsp
	mov rax,rcx
	out 70h, rax
	in al, 71h
	mov rsp, rbp
	pop rbp
	ret
TimeRead ENDP

PUBLIC TimeWrite
TimeWrite PROC
	push rbp 
	mov rbp, rsp 
	mov rax, rcx
	out 70h, rax
	mov rax, rdx
	out 71h, rax
	mov rsp, rbp
	pop rbp
	ret
TimeWrite ENDP
END

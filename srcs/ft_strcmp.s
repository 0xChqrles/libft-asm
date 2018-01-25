section .text
	global	_ft_strcmp

_ft_strcmp:
	mov rax, 0

strcmp:
	mov r8b, byte[rdi]
	cmp r8b, byte[rsi]
	jne different_exit
	cmp r8b, 0
	je exit
	inc rdi
	inc rsi
	jmp strcmp

different_exit:
	movzx rax, byte[rdi]
	movzx rcx, byte[rsi]
	sub rax, rcx

exit:
	ret

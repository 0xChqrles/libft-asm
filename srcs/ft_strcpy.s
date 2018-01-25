section .text
	global	_ft_strcpy

_ft_strcpy:
	mov rax, rdi

strcpy:
	cmp byte[rsi], 0
	je exit
	mov r8b, byte [rsi]
	mov byte[rdi], r8b
	inc rsi
	inc rdi
	jmp strcpy

exit:
	mov byte[rdi], 0
	ret

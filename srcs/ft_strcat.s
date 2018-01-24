section .text
	global	_ft_strcat

_ft_strcat:
	mov rax, rdi

dest_len:
	cmp byte[rdi], 0
	je strcat
	inc rdi
	jmp dest_len

strcat:
	cmp byte[rsi], 0
	je exit
	mov r8b, byte [rsi]
	mov byte[rdi], r8b
	inc rsi
	inc rdi
	jmp strcat

exit:
	mov byte[rdi], 0
	ret

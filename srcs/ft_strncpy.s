section .text
	global	_ft_strncpy

_ft_strncpy:
	mov rax, rdi

strncpy:
	cmp rdx, 0
	jbe exit
	cmp byte[rsi], 0
	je zero_fill
	mov r8b, byte[rsi]
	mov byte[rdi], r8b
	inc rsi
	inc rdi
	dec rdx
	jmp strncpy

zero_fill:
	cmp rdx, 0
	jbe exit
	mov byte[rdi], 0
	inc rdi
	dec rdx
	jmp zero_fill

exit:
	ret

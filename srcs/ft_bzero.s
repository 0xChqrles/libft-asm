section .text
	global	_ft_bzero

_ft_bzero:
	cmp rsi, 0
	je exit
	dec rsi
	mov byte[rdi + rsi], 0
	jmp _ft_bzero

exit:
	ret

section .text
	global	_ft_strchr

_ft_strchr:
	mov rax, 0

strchr:
	cmp byte[rdi], sil
	je exit_found
	cmp byte[rdi], 0
	je exit_zero
	inc rdi
	jmp strchr

exit_found:
	mov rax, rdi

exit_zero:
	ret

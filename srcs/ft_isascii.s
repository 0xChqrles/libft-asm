section .text
	global	_ft_isascii

_ft_isascii:
	cmp rdi, 0
	jb exit_false
	cmp rdi, 127
	ja exit_false

exit_true:
	mov rax, 1
	ret

exit_false:
	mov rax, 0
	ret

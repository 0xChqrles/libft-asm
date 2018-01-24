section .text
	global	_ft_isprint

_ft_isprint:
	cmp rdi, 32
	jb exit_false
	cmp rdi, 126
	ja exit_false

exit_true:
	mov rax, 1
	ret

exit_false:
	mov rax, 0
	ret

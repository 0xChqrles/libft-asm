%define MIN_A	97
%define MIN_Z	122

section .text
	global	_ft_toupper

_ft_toupper:
	mov rax, rdi
	cmp rax, MIN_A
	jb exit
	cmp rax, MIN_Z
	ja exit
	sub rax, 32

exit:
	ret

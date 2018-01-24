%define MAJ_A	65
%define MAJ_Z	90
%define MIN_A	97
%define MIN_Z	122

section .text
	global	_ft_isalpha

_ft_isalpha:
	cmp rdi, MAJ_A
	jb exit_false
	cmp rdi, MIN_Z
	ja exit_false
	cmp rdi, MAJ_Z
	jbe exit_true
	cmp rdi, MIN_A
	jae exit_true

exit_false:
	mov rax, 0
	ret

exit_true:
	mov rax, 1
	ret

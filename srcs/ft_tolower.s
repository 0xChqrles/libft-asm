%define MAJ_A	65
%define MAJ_Z	90

section .text
	global	_ft_tolower

_ft_tolower:
	mov rax, rdi
	cmp rax, MAJ_A
	jb exit
	cmp rax, MAJ_Z
	ja exit
	add rax, 32

exit:
	ret

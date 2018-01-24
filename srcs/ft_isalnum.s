%define NUM_0	48
%define NUM_9	57
%define MAJ_A	65
%define MAJ_Z	90
%define MIN_A	97
%define MIN_Z	122

section .text
	global	_ft_isalnum

_ft_isalnum:
	cmp rdi, NUM_0
	jb exit_false
	cmp rdi, MIN_Z
	ja exit_false
	cmp rdi, NUM_9
	jbe exit_true
	cmp rdi, MIN_A
	jae exit_true
	cmp rdi, MAJ_A
	jb exit_false
	cmp rdi, MAJ_Z
	ja exit_false

exit_true:
	mov rax, 1
	ret

exit_false:
	mov rax, 0
	ret

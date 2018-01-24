%define NUM_0	48
%define NUM_9	57

section .text
	global	_ft_isdigit

_ft_isdigit:
	cmp rdi, NUM_0
	jb exit_false
	cmp rdi, NUM_9
	ja exit_false

exit_true:
	mov rax, 1
	ret

exit_false:
	mov rax, 0
	ret

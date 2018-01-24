section .text
	global	_ft_memset

_ft_memset:
	mov r8, rdi
	mov al, sil
	mov rcx, rdx
	cld
	rep stosb

exit:
	mov rax, r8
	ret

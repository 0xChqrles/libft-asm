section .text
	global	_ft_memccpy

_ft_memccpy:
	cmp rdi, 0
	je exit_null
	cmp rsi, 0
	je exit_null
	mov rbx, 0

memccpy:
	cmp rcx, rdx
	jae exit_null
	mov r8b, byte [rsi + rbx]
	mov byte [rdi + rbx], r8b
	cmp byte [rsi + rbx], cl
	je exit_c
	inc rbx
	jmp memccpy

exit_c:
	mov rax, rdi
	add rax, rbx
	inc rax
	ret

exit_null:
	mov rax, 0
	ret

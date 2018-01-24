section .text
	global	_ft_strdup
	extern	_malloc
	extern	_ft_strlen

_ft_strdup:
	push rbp
	mov rbp, rsp
	sub rsp, 16
	mov r8, 0
	push rbx
	mov rbx, rdi
	call _ft_strlen
	add rax, 1

alloc_str:
	mov rdi, rax
	push rdi
	call _malloc
	pop rdi
	cmp rax, 0
	je exit

cpy_data:
	mov rcx, rdi
	mov rdi, rax
	mov rsi, rbx
	mov r8, rdi
	cld
	rep movsb

exit:
	pop rbx

exit_null:
	mov rax, r8
	leave
	ret

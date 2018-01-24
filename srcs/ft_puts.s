%define MACH_SYSCALL(nb)	0x2000000 | nb
%define WRITE				4
%define STDOUT				1

section .data
null:
	.string db "(null)", 10
	.len equ $ - null.string

nl:
	.string db "", 10
	.len equ $ - nl.string

section .text
	global	_ft_puts

_ft_puts:
	cmp rdi, 0
	je put_null
	mov rdx, 0

strlen:
	cmp byte[rdi + rdx], 0
	je puts
	inc rdx
	jmp strlen

puts:
	mov rax, MACH_SYSCALL(WRITE)
	mov rsi, rdi
	mov rdi, STDOUT
	syscall
	jc exit_failure
	jmp put_nl

put_nl:
	mov rax, MACH_SYSCALL(WRITE)
	mov rdi, STDOUT
	lea rsi, [rel nl.string]
	mov rdx, nl.len
	syscall
	jc exit_failure
	jmp exit_success

put_null:
	mov rax, MACH_SYSCALL(WRITE)
	mov rdi, STDOUT
	lea rsi, [rel null.string]
	mov rdx, null.len
	syscall
	jc exit_failure
	jmp exit_success

exit_success:
	mov rax, 10
	ret

exit_failure:
	ret

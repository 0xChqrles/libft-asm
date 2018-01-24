%define MACH_SYSCALL(nb)	0x2000000 | nb
%define READ				3
%define WRITE				4
%define STDOUT				1
%define BUFF_SIZE			1024

section .text
	global	_ft_cat

_ft_cat:
	push rbx
	mov rbx, rdi
	push rbp
	mov rsp, rbp
	sub rsp, BUFF_SIZE
	cmp rdi, 0
	jb exit

read:
	mov rax, MACH_SYSCALL(READ)
	mov rdi, rbx
	mov rsi, rsp
	mov rdx, BUFF_SIZE
	syscall
	jc exit
	cmp rax, 0
	jbe exit

write:
	mov rdx, rax
	mov rax, MACH_SYSCALL(WRITE)
	mov rdi, STDOUT
	mov rsi, rsp
	syscall
	jc exit
	cmp rax, 0
	jb exit
	jmp read

exit:
	add rsp, BUFF_SIZE
	pop rbp
	pop rbx
	ret

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
	mov rbp, rsp
	sub rbp, BUFF_SIZE
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
	je exit

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
	add rbp, BUFF_SIZE
	pop rbp
	pop rbx
	ret

;section .data
;
;buff_size   equ     4096
;
;%define STDOUT_FILENO 1
;%define SYS_READ 0x2000003
;%define SYS_WRITE 0x2000004
;
;section .text
;global _ft_cat
;
;; void (int fd)
;;           |
;;           v
;;          rdi
;
;_ft_cat:
;    push    rbx
;    mov     rbx,    rdi             ; saving fd in rbx (non volatile)
;    push    rbp
;    mov     rbp,    rsp             ; allocating char[buff_size] on the stack
;    sub     rbp,    buff_size       ; rbp points to the buffer (non volatile)
;
;read:
;    mov     rax,    SYS_READ        ; read(
;    mov     rdi,    rbx             ;   fd,
;    mov     rsi,    rbp             ;   buff,
;    mov     rdx,    buff_size       ;   buff_size)
;    syscall
;    jc      end
;    cmp     rax,    0               ; read failed
;    je      end
;
;write:
;    mov     rdx,    rax             ; write(
;    mov     rax,    SYS_WRITE       ;   STDOUT_FILENO,
;    mov     rdi,    STDOUT_FILENO   ;   buff,
;    mov     rsi,    rbp             ;   size_read)
;    syscall
;    jc      end
;    cmp     rax,    0               ; write failed
;    je      end
;    jmp     read                    ; read again
;
;end:
;    pop rbp
;    pop rbx
;    ret

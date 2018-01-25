section .text
	global	_ft_strrchr

_ft_strrchr:
	mov rax, 0

strchr:
	cmp byte[rdi], sil
	je found

strrchr_loop:
	cmp byte[rdi], 0
	je exit
	inc rdi
	jmp strchr

found:
	mov rax, rdi
	jmp strrchr_loop

exit:
	ret

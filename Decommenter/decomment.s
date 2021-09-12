	.file	"decomment.c"
	.globl	slash
	.section	.rodata
	.align 8
	.type	slash, @object
	.size	slash, 8
slash:
	.byte	47
	.zero	3
	.long	0
	.globl	star
	.align 8
	.type	star, @object
	.size	star, 8
star:
	.byte	42
	.zero	3
	.long	0
	.comm	line,4,4
	.comm	current_comment_start_line,4,4
	.comm	previous_char,8,8
	.comm	current_char,8,8
	.comm	next_char,8,8
	.comm	state,4,4
	.text
	.globl	copy_from_to
	.type	copy_from_to, @function
copy_from_to:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rax
	movzbl	(%rax), %edx
	movq	-16(%rbp), %rax
	movb	%dl, (%rax)
	movq	-8(%rbp), %rax
	movl	4(%rax), %edx
	movq	-16(%rbp), %rax
	movl	%edx, 4(%rax)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	copy_from_to, .-copy_from_to
	.globl	is_equal
	.type	is_equal, @function
is_equal:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movzbl	-8(%rbp), %edx
	movzbl	-16(%rbp), %eax
	cmpb	%al, %dl
	jne	.L3
	movl	-4(%rbp), %edx
	movl	-12(%rbp), %eax
	cmpl	%eax, %edx
	jne	.L3
	movl	$1, %eax
	jmp	.L4
.L3:
	movl	$0, %eax
.L4:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	is_equal, .-is_equal
	.globl	initialization
	.type	initialization, @function
initialization:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$1, line(%rip)
	call	getchar@PLT
	movb	%al, current_char(%rip)
	movl	$0, 4+current_char(%rip)
	call	getchar@PLT
	movb	%al, next_char(%rip)
	movl	$0, 4+next_char(%rip)
	movzbl	current_char(%rip), %eax
	cmpb	$34, %al
	je	.L7
	movzbl	current_char(%rip), %eax
	cmpb	$39, %al
	je	.L8
	movzbl	current_char(%rip), %eax
	cmpb	$47, %al
	jne	.L9
	movzbl	next_char(%rip), %eax
	cmpb	$42, %al
	jne	.L9
	movl	$1, %eax
	jmp	.L12
.L9:
	movl	$0, %eax
	jmp	.L12
.L8:
	movl	$3, %eax
	jmp	.L12
.L7:
	movl	$2, %eax
.L12:
	movl	%eax, state(%rip)
	movl	state(%rip), %eax
	cmpl	$1, %eax
	jne	.L13
	movq	stdout(%rip), %rax
	movq	%rax, %rsi
	movl	$32, %edi
	call	fputc@PLT
	movl	line(%rip), %eax
	movl	%eax, current_comment_start_line(%rip)
	movl	$1, 4+next_char(%rip)
	jmp	.L15
.L13:
	movzbl	current_char(%rip), %eax
	movsbl	%al, %eax
	movq	stdout(%rip), %rdx
	movq	%rdx, %rsi
	movl	%eax, %edi
	call	fputc@PLT
.L15:
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	initialization, .-initialization
	.globl	update
	.type	update, @function
update:
.LFB3:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	leaq	previous_char(%rip), %rsi
	leaq	current_char(%rip), %rdi
	call	copy_from_to
	leaq	current_char(%rip), %rsi
	leaq	next_char(%rip), %rdi
	call	copy_from_to
	call	getchar@PLT
	movb	%al, next_char(%rip)
	movl	$0, 4+next_char(%rip)
	movzbl	current_char(%rip), %eax
	cmpb	$10, %al
	jne	.L18
	movl	line(%rip), %eax
	addl	$1, %eax
	movl	%eax, line(%rip)
.L18:
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	update, .-update
	.section	.rodata
	.align 8
.LC0:
	.string	"Error: line %d: unterminated comment\n"
	.text
	.globl	eof_handler
	.type	eof_handler, @function
eof_handler:
.LFB4:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	state(%rip), %eax
	cmpl	$1, %eax
	jne	.L20
	movl	current_comment_start_line(%rip), %edx
	movq	stderr(%rip), %rax
	leaq	.LC0(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$1, %eax
	jmp	.L21
.L20:
	movl	$0, %eax
.L21:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	eof_handler, .-eof_handler
	.globl	check_double_quotation
	.type	check_double_quotation, @function
check_double_quotation:
.LFB5:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movzbl	current_char(%rip), %eax
	cmpb	$34, %al
	jne	.L26
	movzbl	previous_char(%rip), %eax
	cmpb	$92, %al
	je	.L26
	movl	state(%rip), %eax
	cmpl	$2, %eax
	jne	.L24
	movl	$0, %eax
	jmp	.L25
.L24:
	movl	$2, %eax
.L25:
	movl	%eax, state(%rip)
.L26:
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	check_double_quotation, .-check_double_quotation
	.globl	check_single_quotation
	.type	check_single_quotation, @function
check_single_quotation:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movzbl	current_char(%rip), %eax
	cmpb	$39, %al
	jne	.L31
	movzbl	previous_char(%rip), %eax
	cmpb	$92, %al
	je	.L31
	movl	state(%rip), %eax
	cmpl	$3, %eax
	jne	.L29
	movl	$0, %eax
	jmp	.L30
.L29:
	movl	$3, %eax
.L30:
	movl	%eax, state(%rip)
.L31:
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	check_single_quotation, .-check_single_quotation
	.globl	check_comment_opening
	.type	check_comment_opening, @function
check_comment_opening:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	slash(%rip), %rdx
	movq	current_char(%rip), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	is_equal
	testl	%eax, %eax
	je	.L33
	movq	star(%rip), %rdx
	movq	next_char(%rip), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	is_equal
	testl	%eax, %eax
	je	.L33
	movl	$1, state(%rip)
	movl	$1, 4+next_char(%rip)
	movq	stdout(%rip), %rax
	movq	%rax, %rsi
	movl	$32, %edi
	call	fputc@PLT
	movl	line(%rip), %eax
	movl	%eax, current_comment_start_line(%rip)
	jmp	.L34
.L33:
	movzbl	current_char(%rip), %eax
	movsbl	%al, %eax
	movq	stdout(%rip), %rdx
	movq	%rdx, %rsi
	movl	%eax, %edi
	call	fputc@PLT
.L34:
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	check_comment_opening, .-check_comment_opening
	.globl	check_comment_closing
	.type	check_comment_closing, @function
check_comment_closing:
.LFB8:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	star(%rip), %rdx
	movq	previous_char(%rip), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	is_equal
	testl	%eax, %eax
	je	.L37
	movq	slash(%rip), %rdx
	movq	current_char(%rip), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	is_equal
	testl	%eax, %eax
	je	.L37
	movl	$0, state(%rip)
	movl	$1, 4+current_char(%rip)
.L37:
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	check_comment_closing, .-check_comment_closing
	.globl	main
	.type	main, @function
main:
.LFB9:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$0, %eax
	call	initialization
.L46:
	movl	$0, %eax
	call	update
	movzbl	current_char(%rip), %eax
	cmpb	$-1, %al
	jne	.L39
	movl	$0, %eax
	call	eof_handler
	jmp	.L47
.L39:
	movl	state(%rip), %eax
	testl	%eax, %eax
	jne	.L41
	movl	$0, %eax
	call	check_double_quotation
	movl	$0, %eax
	call	check_single_quotation
	movl	$0, %eax
	call	check_comment_opening
	jmp	.L46
.L41:
	movl	state(%rip), %eax
	cmpl	$1, %eax
	jne	.L43
	movzbl	current_char(%rip), %eax
	cmpb	$10, %al
	jne	.L44
	movq	stdout(%rip), %rax
	movq	%rax, %rsi
	movl	$10, %edi
	call	fputc@PLT
.L44:
	movl	$0, %eax
	call	check_comment_closing
	jmp	.L46
.L43:
	movl	state(%rip), %eax
	cmpl	$2, %eax
	jne	.L45
	movzbl	current_char(%rip), %eax
	movsbl	%al, %eax
	movq	stdout(%rip), %rdx
	movq	%rdx, %rsi
	movl	%eax, %edi
	call	fputc@PLT
	movl	$0, %eax
	call	check_double_quotation
	jmp	.L46
.L45:
	movzbl	current_char(%rip), %eax
	movsbl	%al, %eax
	movq	stdout(%rip), %rdx
	movq	%rdx, %rsi
	movl	%eax, %edi
	call	fputc@PLT
	movl	$0, %eax
	call	check_single_quotation
	jmp	.L46
.L47:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.5.0-1ubuntu2) 5.4.1 20171010"
	.section	.note.GNU-stack,"",@progbits

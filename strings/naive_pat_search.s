	.file	"naive_pat_search.c"
	.local	calls
	.comm	calls,8,8
	.globl	hohoho
	.data
	.align 8
	.type	hohoho, @object
	.size	hohoho, 9
hohoho:
	.string	"HOHOHOHO"
	.globl	found
	.bss
	.align 8
	.type	found, @object
	.size	found, 8
found:
	.zero	8
	.section	.rodata
.LC0:
	.string	"Pattern found at %lu\n"
.LC1:
	.string	"-----"
	.text
	.type	search, @function
search:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	calls(%rip), %rax
	addq	$1, %rax
	movq	%rax, calls(%rip)
	cmpq	$0, -24(%rbp)
	je	.L7
	cmpq	$0, -32(%rbp)
	je	.L7
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	strlen
	movq	%rax, -8(%rbp)
	movq	-32(%rbp), %rax
	movq	%rax, -16(%rbp)
	jmp	.L5
.L6:
	movq	-24(%rbp), %rdx
	movq	-16(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	strstr
	movq	%rax, -16(%rbp)
	cmpq	$0, -16(%rbp)
	je	.L5
	movq	found(%rip), %rax
	addq	$1, %rax
	movq	%rax, found(%rip)
	movq	-16(%rbp), %rdx
	movq	-32(%rbp), %rax
	subq	%rax, %rdx
	movq	%rdx, %rax
	movq	%rax, %rsi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	movq	-8(%rbp), %rax
	addq	%rax, -16(%rbp)
.L5:
	cmpq	$0, -16(%rbp)
	jne	.L6
	movl	$.LC1, %edi
	call	puts
	jmp	.L1
.L7:
	nop
.L1:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	search, .-search
	.section	.rodata
.LC2:
	.string	"TEST"
.LC3:
	.string	"IS IS IS"
.LC4:
	.string	"IS"
.LC5:
	.string	"AABA"
.LC6:
	.string	"AAAAAAAAAAAAAAAA"
.LC7:
	.string	"AA"
	.text
	.globl	main
	.type	main, @function
main:
.LFB3:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movabsq	$2329285835465771092, %rax
	movq	%rax, -32(%rbp)
	movabsq	$6061937815062847553, %rax
	movq	%rax, -24(%rbp)
	movl	$5527621, -16(%rbp)
	leaq	-32(%rbp), %rax
	movq	%rax, %rsi
	movl	$.LC2, %edi
	call	search
	movl	$.LC3, %esi
	movl	$.LC4, %edi
	call	search
	movl	$hohoho, %esi
	movl	$.LC5, %edi
	call	search
	movb	$69, hohoho+1(%rip)
	movl	$hohoho, %esi
	movl	$.LC5, %edi
	call	search
	movl	$.LC6, %esi
	movl	$.LC7, %edi
	call	search
	movl	$0, %eax
	movq	-8(%rbp), %rdx
	xorq	%fs:40, %rdx
	je	.L10
	call	__stack_chk_fail
.L10:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.5.0-12ubuntu1~16.04) 5.5.0 20171010"
	.section	.note.GNU-stack,"",@progbits

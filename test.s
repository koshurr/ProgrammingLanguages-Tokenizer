	.file	"test.c"
	.text
	.globl	yakovstern
	.def	yakovstern;	.scl	2;	.type	32;	.endef
yakovstern:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$16, %rsp
	movl	$1, -4(%rbp)
	movb	$97, -5(%rbp)
	movsbl	-5(%rbp), %eax
	addl	%eax, -4(%rbp)
	movl	-4(%rbp), %eax
	leave
	ret
	.ident	"GCC: (tdm64-1) 5.1.0"

	.file	"fat.c"
gcc2_compiled.:
___gnu_compiled_c:
.text
LC0:
	.ascii "fs->dosparam.fatsize = %d\12\0"
LC1:
	.ascii "fs->dosparam.blocksize = %d\12\0"
LC2:
	.ascii "malloc (%d)\12\0"
LC3:
	.ascii "DOS_FATENTRY = %d\12\0"
LC4:
	.ascii "No more memory!\12\0"
LC5:
	.ascii "dofs: read_device (%d, %d, %d, 0x%x)\12\0"
LC6:
	.ascii "magicid = %d\12\0"
	.align 2
.globl _init_fattable
_init_fattable:
	pushl %ebp
	movl %esp,%ebp
	subl $20,%esp
	pushl %esi
	pushl %ebx
	movl 8(%ebp),%eax
	movl %eax,-20(%ebp)
	movl -20(%ebp),%esi
	movl 64(%esi),%edx
	movl %edx,-12(%ebp)
	cmpl $8,-12(%ebp)
	jne L2
	movl $16,-20(%ebp)
	jmp L3
	.align 2,0x90
L2:
	movl $12,-20(%ebp)
L3:
	movl -20(%ebp),%eax
	movl %eax,-16(%ebp)
	movl 8(%ebp),%esi
	movl %esi,-20(%ebp)
	movl -20(%ebp),%eax
	movl 72(%eax),%edx
	pushl %edx
	pushl $LC0
	call _printf
	addl $8,%esp
	movl 8(%ebp),%esi
	movl %esi,-20(%ebp)
	movl -20(%ebp),%eax
	movl 76(%eax),%edx
	pushl %edx
	pushl $LC1
	call _printf
	addl $8,%esp
	cmpl $16,-16(%ebp)
	jne L4
	movl 8(%ebp),%esi
	movl %esi,-20(%ebp)
	movl 8(%ebp),%edx
	movl -20(%ebp),%eax
	movl 72(%eax),%eax
	imull 88(%edx),%eax
	movl %eax,-20(%ebp)
	movl -20(%ebp),%edx
	sarl $31,%edx
	movl %edx,%ecx
	shrl $31,%ecx
	movl -20(%ebp),%edx
	addl %ecx,%edx
	movl %edx,%ecx
	sarl $1,%ecx
	movl %ecx,%edx
	leal (%ecx,%edx),%esi
	movl %esi,-20(%ebp)
	jmp L5
	.align 2,0x90
L4:
	movl 8(%ebp),%edx
	movl 8(%ebp),%ecx
	movl 72(%edx),%eax
	imull 88(%ecx),%eax
	movl %eax,-20(%ebp)
	movl -20(%ebp),%edx
	addl %edx,-20(%ebp)
	movl -20(%ebp),%eax
	movl $3,%esi
	cltd
	idivl %esi
	movl %eax,%ebx
	movl %ebx,%ecx
	leal (%ebx,%ecx),%eax
	movl %eax,-20(%ebp)
L5:
	pushl -20(%ebp)
	pushl $LC2
	call _printf
	addl $8,%esp
	cmpl $16,-16(%ebp)
	jne L6
	movl 8(%ebp),%esi
	movl %esi,-20(%ebp)
	movl 8(%ebp),%edx
	movl -20(%ebp),%eax
	movl 72(%eax),%eax
	imull 88(%edx),%eax
	movl %eax,-20(%ebp)
	movl -20(%ebp),%edx
	sarl $31,%edx
	movl %edx,%ecx
	shrl $31,%ecx
	movl -20(%ebp),%ebx
	addl %ecx,%ebx
	movl %ebx,%edx
	sarl $1,%edx
	movl %edx,-20(%ebp)
	jmp L7
	.align 2,0x90
L6:
	movl 8(%ebp),%edx
	movl 8(%ebp),%ecx
	movl 72(%edx),%esi
	imull 88(%ecx),%esi
	movl %esi,-20(%ebp)
	movl -20(%ebp),%edx
	addl %edx,-20(%ebp)
	movl -20(%ebp),%eax
	movl $3,%esi
	cltd
	idivl %esi
	movl %eax,%ecx
	movl %ecx,-20(%ebp)
L7:
	pushl -20(%ebp)
	pushl $LC3
	call _printf
	addl $8,%esp
	cmpl $16,-16(%ebp)
	jne L8
	movl 8(%ebp),%eax
	movl %eax,-20(%ebp)
	movl 8(%ebp),%edx
	movl -20(%ebp),%esi
	movl 72(%esi),%esi
	imull 88(%edx),%esi
	movl %esi,-20(%ebp)
	movl -20(%ebp),%edx
	sarl $31,%edx
	movl %edx,%ecx
	shrl $31,%ecx
	movl -20(%ebp),%edx
	addl %ecx,%edx
	movl %edx,%ecx
	sarl $1,%ecx
	movl %ecx,%edx
	leal (%ecx,%edx),%eax
	movl %eax,-20(%ebp)
	jmp L9
	.align 2,0x90
L8:
	movl 8(%ebp),%edx
	movl 8(%ebp),%ecx
	movl 72(%edx),%esi
	imull 88(%ecx),%esi
	movl %esi,-20(%ebp)
	movl -20(%ebp),%edx
	addl %edx,-20(%ebp)
	movl -20(%ebp),%eax
	movl $3,%esi
	cltd
	idivl %esi
	movl %eax,%ebx
	movl %ebx,%ecx
	leal (%ebx,%ecx),%eax
	movl %eax,-20(%ebp)
L9:
	pushl -20(%ebp)
	call _malloc
	addl $4,%esp
	movl %eax,-4(%ebp)
	cmpl $0,-4(%ebp)
	jne L10
	pushl $LC4
	call _printf
	addl $4,%esp
	jmp L1
	.align 2,0x90
L10:
	movl 8(%ebp),%esi
	movl %esi,-20(%ebp)
	movl 8(%ebp),%edx
	movl -20(%ebp),%eax
	movl 72(%eax),%eax
	imull 88(%edx),%eax
	movl %eax,-20(%ebp)
	pushl -20(%ebp)
	call _malloc
	addl $4,%esp
	movl %eax,-8(%ebp)
	cmpl $0,-8(%ebp)
	jne L11
	pushl $LC4
	call _printf
	addl $4,%esp
	jmp L1
	.align 2,0x90
L11:
	movl -8(%ebp),%esi
	movl %esi,-20(%ebp)
	pushl -20(%ebp)
	movl 8(%ebp),%eax
	movl %eax,-20(%ebp)
	movl 8(%ebp),%edx
	movl -20(%ebp),%esi
	movl 72(%esi),%esi
	imull 88(%edx),%esi
	movl %esi,-20(%ebp)
	pushl -20(%ebp)
	movl 8(%ebp),%eax
	movl %eax,-20(%ebp)
	movl 8(%ebp),%edx
	movl -20(%ebp),%esi
	movl 68(%esi),%esi
	imull 88(%edx),%esi
	movl %esi,-20(%ebp)
	pushl -20(%ebp)
	movl 8(%ebp),%eax
	movl %eax,-20(%ebp)
	movl -20(%ebp),%esi
	movl 56(%esi),%edx
	pushl %edx
	pushl $LC5
	call _printf
	addl $20,%esp
	movl -8(%ebp),%eax
	movl %eax,-20(%ebp)
	pushl -20(%ebp)
	movl 8(%ebp),%esi
	movl %esi,-20(%ebp)
	movl 8(%ebp),%edx
	movl -20(%ebp),%eax
	movl 72(%eax),%eax
	imull 88(%edx),%eax
	movl %eax,-20(%ebp)
	pushl -20(%ebp)
	movl 8(%ebp),%esi
	movl %esi,-20(%ebp)
	movl -20(%ebp),%eax
	movl 60(%eax),%edx
	pushl %edx
	movl 8(%ebp),%esi
	movl %esi,-20(%ebp)
	movl 8(%ebp),%edx
	movl -20(%ebp),%eax
	movl 68(%eax),%eax
	imull 88(%edx),%eax
	movl %eax,-20(%ebp)
	pushl -20(%ebp)
	movl 8(%ebp),%esi
	movl %esi,-20(%ebp)
	movl -20(%ebp),%eax
	movl 56(%eax),%edx
	pushl %edx
	call _read_device
	addl $20,%esp
	movl -12(%ebp),%esi
	movl %esi,-20(%ebp)
	pushl -20(%ebp)
	pushl $LC6
	call _printf
	addl $8,%esp
	cmpl $8,-12(%ebp)
	jne L12
	movl 8(%ebp),%eax
	movl %eax,-20(%ebp)
	movl 8(%ebp),%edx
	movl -20(%ebp),%esi
	movl 72(%esi),%esi
	imull 88(%edx),%esi
	movl %esi,-20(%ebp)
	movl -20(%ebp),%edx
	sarl $31,%edx
	movl %edx,%ecx
	shrl $31,%ecx
	movl -20(%ebp),%edx
	addl %ecx,%edx
	movl %edx,%eax
	sarl $1,%eax
	movl %eax,-20(%ebp)
	pushl -20(%ebp)
	movl -4(%ebp),%esi
	movl %esi,-20(%ebp)
	pushl -20(%ebp)
	movl -8(%ebp),%eax
	movl %eax,-20(%ebp)
	pushl -20(%ebp)
	call _make_fat16table
	addl $12,%esp
	jmp L13
	.align 2,0x90
L12:
	movl 8(%ebp),%esi
	movl %esi,-20(%ebp)
	movl 8(%ebp),%edx
	movl -20(%ebp),%eax
	movl 72(%eax),%ecx
	imull 88(%edx),%ecx
	movl %ecx,-20(%ebp)
	addl -20(%ebp),%ecx
	movl %ecx,%eax
	movl $3,%esi
	cltd
	idivl %esi
	movl %eax,-20(%ebp)
	pushl -20(%ebp)
	movl -4(%ebp),%eax
	movl %eax,-20(%ebp)
	pushl -20(%ebp)
	movl -8(%ebp),%esi
	movl %esi,-20(%ebp)
	pushl -20(%ebp)
	call _make_fat12table
	addl $12,%esp
L13:
	movl 8(%ebp),%eax
	movl %eax,-20(%ebp)
	movl -4(%ebp),%edx
	movl -20(%ebp),%esi
	movl %edx,116(%esi)
	movl -8(%ebp),%eax
	movl %eax,-20(%ebp)
	pushl -20(%ebp)
	call _free
	addl $4,%esp
L1:
	leal -28(%ebp),%esp
	popl %ebx
	popl %esi
	leave
	ret
LC7:
	.ascii "fat 16\12\0"
LC8:
	.ascii "fat count is %d\12\0"
	.align 2
.globl _make_fat16table
_make_fat16table:
	pushl %ebp
	movl %esp,%ebp
	subl $12,%esp
	pushl $LC7
	call _printf
	addl $4,%esp
	movl 16(%ebp),%eax
	pushl %eax
	pushl $LC8
	call _printf
	addl $8,%esp
L14:
	leave
	ret
LC9:
	.ascii "fat 12\12\0"
	.align 2
.globl _make_fat12table
_make_fat12table:
	pushl %ebp
	movl %esp,%ebp
	subl $12,%esp
	pushl $LC9
	call _printf
	addl $4,%esp
	movl 16(%ebp),%eax
	pushl %eax
	pushl $LC8
	call _printf
	addl $8,%esp
L15:
	leave
	ret
	.align 2
.globl _next_fatid
_next_fatid:
	pushl %ebp
	movl %esp,%ebp
	cmpl $4095,12(%ebp)
	je L17
	movl 8(%ebp),%eax
	movl 12(%ebp),%ecx
	movl %ecx,%edx
	addl %edx,%ecx
	movl 116(%eax),%eax
	movzwl (%eax,%ecx),%edx
	movl %edx,%eax
	jmp L16
	.align 2,0x90
L17:
	xorl %eax,%eax
	jmp L16
	.align 2,0x90
L16:
	leave
	ret
LC10:
	.ascii "fat[%d] = %d(0x%x)\12\0"
	.align 2
.globl _print_fat
_print_fat:
	pushl %ebp
	movl %esp,%ebp
	subl $8,%esp
	movl 8(%ebp),%eax
	movl 116(%eax),%edx
	movl %edx,-4(%ebp)
	movl $0,-8(%ebp)
L19:
	cmpl $5,-8(%ebp)
	jle L22
	jmp L20
	.align 2,0x90
L22:
	movl -8(%ebp),%edx
	movl %edx,%eax
	addl %eax,%edx
	movl -4(%ebp),%eax
	movzwl (%eax,%edx),%edx
	pushl %edx
	movl -8(%ebp),%edx
	movl %edx,%eax
	addl %eax,%edx
	movl -4(%ebp),%eax
	movzwl (%eax,%edx),%edx
	pushl %edx
	movl -8(%ebp),%eax
	pushl %eax
	pushl $LC10
	call _printf
	addl $16,%esp
L21:
	incl -8(%ebp)
	jmp L19
	.align 2,0x90
L20:
L18:
	leave
	ret

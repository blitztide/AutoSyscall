CC=x86_64-w64-mingw32-gcc
ASM_CC=nasm
OUTFILE=defensiveinjector.exe

_XP_SP1:
	$(ASM_CC) -f win64 -D _XP_SP1 syscalls.asm -o syscalls.lib
_XP_SP2:
	$(ASM_CC) -f win64 -D _XP_SP2 syscalls.asm -o syscalls.lib
_Server_2003_SP0:
	$(ASM_CC) -f win64 -D _Server_2003_SP0 syscalls.asm -o syscalls.lib
_Server_2003_SP2:
	$(ASM_CC) -f win64 -D _Server_2003_SP2 syscalls.asm -o syscalls.lib
_Server_2003_R2:
	$(ASM_CC) -f win64 -D _Server_2003_R2 syscalls.asm -o syscalls.lib
_Server_2003_R2_SP2:
	$(ASM_CC) -f win64 -D _Server_2003_R2_SP2 syscalls.asm -o syscalls.lib
_Vista_SP0:
	$(ASM_CC) -f win64 -D _Vista_SP0 syscalls.asm -o syscalls.lib
_Vista_SP1:
	$(ASM_CC) -f win64 -D _Vista_SP1 syscalls.asm -o syscalls.lib
_Vista_SP2:
	$(ASM_CC) -f win64 -D _Vista_SP2 syscalls.asm -o syscalls.lib
_Server_2008_SP0:
	$(ASM_CC) -f win64 -D _Server_2008_SP0 syscalls.asm -o syscalls.lib
_Server_2008_SP2:
	$(ASM_CC) -f win64 -D _Server_2008_SP2 syscalls.asm -o syscalls.lib
_Server_2008_R2:
	$(ASM_CC) -f win64 -D _Server_2008_R2 syscalls.asm -o syscalls.lib
_Server_2008_R2_SP1:
	$(ASM_CC) -f win64 -D _Server_2008_R2_SP1 syscalls.asm -o syscalls.lib
_7_SP0:
	$(ASM_CC) -f win64 -D _7_SP0 syscalls.asm -o syscalls.lib
_7_SP1:
	$(ASM_CC) -f win64 -D _7_SP1 syscalls.asm -o syscalls.lib
_Server_2012_SP0:
	$(ASM_CC) -f win64 -D _Server_2012_SP0 syscalls.asm -o syscalls.lib
_Server_2012_R2:
	$(ASM_CC) -f win64 -D _Server_2012_R2 syscalls.asm -o syscalls.lib
_8_8.0:
	$(ASM_CC) -f win64 -D _8_8.0 syscalls.asm -o syscalls.lib
_8_8.1:
	$(ASM_CC) -f win64 -D _8_8.1 syscalls.asm -o syscalls.lib
_10_1507:
	$(ASM_CC) -f win64 -D _10_1507 syscalls.asm -o syscalls.lib
_10_1511:
	$(ASM_CC) -f win64 -D _10_1511 syscalls.asm -o syscalls.lib
_10_1607:
	$(ASM_CC) -f win64 -D _10_1607 syscalls.asm -o syscalls.lib
_10_1703:
	$(ASM_CC) -f win64 -D _10_1703 syscalls.asm -o syscalls.lib
_10_1709:
	$(ASM_CC) -f win64 -D _10_1709 syscalls.asm -o syscalls.lib
_10_1803:
	$(ASM_CC) -f win64 -D _10_1803 syscalls.asm -o syscalls.lib
_10_1809:
	$(ASM_CC) -f win64 -D _10_1809 syscalls.asm -o syscalls.lib
_10_1903:
	$(ASM_CC) -f win64 -D _10_1903 syscalls.asm -o syscalls.lib
_10_1909:
	$(ASM_CC) -f win64 -D _10_1909 syscalls.asm -o syscalls.lib
_10_2004:
	$(ASM_CC) -f win64 -D _10_2004 syscalls.asm -o syscalls.lib
_10_20H2:
	$(ASM_CC) -f win64 -D _10_20H2 syscalls.asm -o syscalls.lib
injector:
	$(CC) main.c -o $(OUTFILE) -L./ -lsyscalls

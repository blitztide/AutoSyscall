#!/usr/bin/env python

# Import libraries

import json
import os
import stat
import urllib.request

# String primitives

URL_SYSCALL = "https://raw.githubusercontent.com/j00ru/windows-syscalls/master/x64/json/nt-per-system.json"
ASM_HEADER = "section .text\n%macro makesyscall 2\nglobal %1\n%1:\n\tmov r10,rcx\n\tmov eax,%2\n\tsyscall\n\tret\n%endmacro\n"
ASM_DEF_START = "%ifdef " 
ASM_MACRO_SYSCALL = "\nmakesyscall {0}, {1}" # Format string
ASM_DEF_END = "\n%endif\n\n"#
MAKE_HEADER = "CC=x86_64-w64-mingw32-gcc\nASM_CC=nasm\nOUTFILE=defensiveinjector.exe\n\n"
MAKE_SYSTEM = "{0}:\n\t$(ASM_CC) -f win64 -D{0} syscalls.asm -o syscalls.lib\n"
MAKE_FOOTER = "injector:\n\t$(CC) main.c -o $(OUTFILE) -L./ -lsyscalls\n"
SYSTEM_DEF = "{}_{}"
HEADER = """  ___        _        _____                     _ _ 
 / _ \      | |      /  ___|                   | | |
/ /_\ \_   _| |_ ___ \ `--. _   _ ___  ___ __ _| | |
|  _  | | | | __/ _ \ `--. \ | | / __|/ __/ _` | | |
| | | | |_| | || (_) /\__/ / |_| \__ \ (_| (_| | | |
\_| |_/\__,_|\__\___/\____/ \__, |___/\___\__,_|_|_|
                             __/ |                  
                            |___/                   """

# Request latest x64 data from github as JSON

def collectjson():

    with urllib.request.urlopen(URL_SYSCALL) as f:
        data = f.read().decode("utf-8")
    # Convert JSON to Python dict
    return json.loads(data)


# Function to print all OS versions in a python list

def printsystems(data):
    # Systems object is a list of tuples to allow easier indexing later.
    for OS in data:
        for VERSION in data[OS]:
            print("Found System", OS + VERSION)
    return


# String prettifying routine for system definitions

def formatsysdef(OS, VERSION):
    # Python strings are immutable, so modify and reassign to match naming convention
    system_definition = SYSTEM_DEF.format(OS,VERSION)
    system_definition = system_definition.replace(' ', '_')
    system_definition = system_definition.lstrip("Windows")
    return system_definition


# Generate ASM primitives
# Requires the serialised JSON data, list of systems and list of syscalls

def generateasm(data):
    asm = ASM_HEADER
    systemdefinitions = list() 
    for OS in data:
        for VERSION in data[OS]:
            system_definition = formatsysdef(OS, VERSION)
            systemdefinitions.append(system_definition)
            asm = asm + ASM_DEF_START + system_definition
            for SYSCALL in data[OS][VERSION]:
                asm = asm + ASM_MACRO_SYSCALL.format(SYSCALL,hex(data[OS][VERSION][SYSCALL]))
            asm = asm + ASM_DEF_END
    return (asm, systemdefinitions)


# Create makefile for code

def generatemake(sysdefs):
    make = MAKE_HEADER
    for system in sysdefs:
        make = make + MAKE_SYSTEM.format(system)
    make = make + MAKE_FOOTER
    return make


# Test environment for write permissions

def testenvironment():
    dirname = os.getcwd()    
    if not os.access(dirname, os.W_OK):
        print("UNABLE TO WRITE IN DIRECTORY: PERMISSION ERROR")
        quit()
    return


# Output file function

def writefile(string, filename):
    file = open(filename, "w")
    n = file.write(string)
    file.close()


# Main function

def main():
    print(HEADER)
    testenvironment()
    print("Collecting raw syscall data")
    data = collectjson()
    printsystems(data)
    print("Generating ASM")
    asm, sysdefs = generateasm(data)
    writefile(asm, "syscalls.asm")
    print("Generating Makefile")
    make = generatemake(sysdefs)
    writefile(make, "Makefile")

# Start as module

if __name__ == "__main__":
    main()



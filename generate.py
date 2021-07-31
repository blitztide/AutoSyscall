#!/usr/bin/env python

# Import libraries

import urllib.request
import json
import os
import stat

# ASM primitives

SYSCALLSTRING_A = "\tmov r10, rcx\n\tmov eax, "
SYSCALLSTRING_B = "\n\tsyscall\n\tret\n\n"

# Request latest x64 CSV from github as JSON

def collectcsv():

    with urllib.request.urlopen("https://raw.githubusercontent.com/j00ru/windows-syscalls/master/x64/json/nt-per-system.json") as f:
        data = f.read().decode("utf-8")

    # Convert JSON to Python dict
    return json.loads(data)
    

# Generate ASM primitives

def generateasm(serialised):
    asm = ""
    asm = asm + "section .text"
    globalsstring = ""
    functionstring = ""
    systemdefinitions = list() 
    for x in serialised:
        for y in serialised[x]:
            # Python strings are immutable, so modify and reassign to match naming convention
            system_definition = x +"_" + y
            system_definition = system_definition.replace(' ', '_')
            system_definition = system_definition.lstrip("Windows")
            systemdefinitions.append(system_definition)
            asm = asm + "%ifdef " + system_definition
            for z in serialised[x][y]:
                globalsstring = globalsstring + "\nglobal " + z
                functionstring = functionstring + "\n" + z + ":\n" + SYSCALLSTRING_A + str(serialised[x][y][z]) + SYSCALLSTRING_B 
            asm = asm + globalsstring + "\n" + functionstring + "%endif"
    return (asm, systemdefinitions)

# Create makefile for code

def generatemake(sysdefs):
    make = "CC=x86_64-w64-mingw32-gcc\nASM_CC=nasm\nOUTFILE=defensiveinjector.exe\n\n"
    for system in sysdefs:
        make = make + system + ":\n\t$(ASM_CC) -f win64 -D " + system + " syscalls.asm -o syscalls.lib\n"
    return make


# Test environment for write permissions

def testenvironment():
    dirname = os.getcwd()    
    if not os.access(dirname, os.W_OK):
        print("UNABLE TO WRITE IN DIRECTORY: PERMISSION ERROR")
        quit()
    return


# Main function

def main():
    testenvironment()
    data = collectcsv()
    asm, sysdefs = generateasm(data)
    make = generatemake(sysdefs)
    print(asm)
    print(make)


# Start as module

if __name__ == "__main__":
    main()



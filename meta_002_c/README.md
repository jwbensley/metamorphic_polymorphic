## meta_002_c


### Overview

 * Upon execution this program loads a copy of itself into memory as raw binary data
 * It then parses the ELF headers to find the .text section (where the executable code lies within the binary file data)
 * Next it scans through the .text section in memory looking for any instances of a random junk assembly sequence encoded as opcodes, and replaces these sequences with new randomly generated sequences
 * All of the sequences of junk assemlby instructions are within functions (`junk_0()` to `junk_4()`)
 * Throughout the code are various instances of the `RANDOM_CALL` macro which randomly calls one of the `junk_*()` functions
 * The updated binary data in memory is saved to disk (as a new file for demonstration purposes but ideally this would overwrite the original file)
 * The overall effect is that each time the program is run a random mixture of these `junk_*()` functions are called and each one contains a different junk assembly instruction sequences which is executed, making the code is metamorphic (the code being executed is chaing randomly each time and which code does and doesn't execute also changes randomly), but the change in program code and operation doesn't relate to the main program function and so the program code and execution flow is always changing but the main program output/effect is consistent

For the sake of making this example clearer, rather than overwriting itself each time the program is run, the modified output is saved to a new file so that the original and new copy of the program can be disassembled/compared.


### Output

Below it can be seen that when executed for the first time, the program replaces the junk assembly sequences with new ones and saves this to a new file. It can also be seen that the various functions which contain the junk assembly were called in a random order:

```bash
$ gcc -pedantic -Wall -Werror -O0 -o meta_002 meta_002.c

$ ./meta_002 
junk_3()
junk_3()
Loaded 14136 bytes from ./meta_002

junk_1()
Junk found, from offset 0xB64 (2916) to 0xB6E (2926)
Replaced: 0x53 0x50 0x48 0x87 0xC0 0x48 0x87 0xC0 0x58 0x5B 
junk_0()
With: 0x53 0x52 0x48 0x87 0xD3 0x48 0x87 0xD3 0x5A 0x5B 

junk_3()
Junk found, from offset 0xD6D (3437) to 0xD77 (3447)
Replaced: 0x53 0x50 0x48 0x87 0xC0 0x48 0x87 0xC0 0x58 0x5B 
With: 0x51 0x50 0x48 0x87 0xC1 0x48 0x87 0xC1 0x58 0x59 

junk_0()
Junk found, from offset 0x101A (4122) to 0x1024 (4132)
Replaced: 0x53 0x50 0x48 0x87 0xC0 0x48 0x87 0xC0 0x58 0x5B 
With: 0x53 0x50 0x48 0x87 0xC3 0x48 0x87 0xC3 0x58 0x5B 

junk_0()
Junk found, from offset 0x1231 (4657) to 0x123B (4667)
Replaced: 0x53 0x50 0x48 0x87 0xC0 0x48 0x87 0xC0 0x58 0x5B 
With: 0x51 0x50 0x48 0x87 0xC1 0x48 0x87 0xC1 0x58 0x59 

junk_3()
Junk found, from offset 0x14B3 (5299) to 0x14BD (5309)
Replaced: 0x53 0x50 0x48 0x87 0xC0 0x48 0x87 0xC0 0x58 0x5B 
With: 0x51 0x52 0x48 0x87 0xD1 0x48 0x87 0xD1 0x5A 0x59 

All existing junk sequences have been replaced

junk_3()
Wrote modified binary data to ./meta_003
```

When the output file is executed it again replaces the junk assembly sequences with new ones and again different random junk functions were executed. This means that not only was different code executed, the program function call flow was different.

It can also be seen that the origin file and this new file are the same size but the SHA512 hashes are different:

```bash
$ ./meta_003
junk_1()
junk_3()
Loaded 14136 bytes from ./meta_003

junk_0()
Junk found, from offset 0xB64 (2916) to 0xB6E (2926)
Replaced: 0x50 0x52 0x48 0x87 0xD0 0x48 0x87 0xD0 0x5A 0x58 
With: 0x50 0x51 0x48 0x87 0xC8 0x48 0x87 0xC8 0x59 0x58 

junk_0()
Junk found, from offset 0xD6D (3437) to 0xD77 (3447)
Replaced: 0x52 0x53 0x48 0x87 0xDA 0x48 0x87 0xDA 0x5B 0x5A 
With: 0x50 0x53 0x48 0x87 0xD8 0x48 0x87 0xD8 0x5B 0x58 

junk_2()
Junk found, from offset 0x101A (4122) to 0x1024 (4132)
Replaced: 0x50 0x51 0x48 0x87 0xC8 0x48 0x87 0xC8 0x59 0x58 
With: 0x53 0x51 0x48 0x87 0xCB 0x48 0x87 0xCB 0x59 0x5B 

junk_1()
Junk found, from offset 0x1231 (4657) to 0x123B (4667)
Replaced: 0x53 0x50 0x48 0x87 0xC3 0x48 0x87 0xC3 0x58 0x5B 
With: 0x51 0x52 0x48 0x87 0xD1 0x48 0x87 0xD1 0x5A 0x59 

junk_0()
Junk found, from offset 0x14B3 (5299) to 0x14BD (5309)
Replaced: 0x52 0x53 0x48 0x87 0xDA 0x48 0x87 0xDA 0x5B 0x5A 
junk_3()
With: 0x50 0x51 0x48 0x87 0xC8 0x48 0x87 0xC8 0x59 0x58 

All existing junk sequences have been replaced

junk_2()
Wrote modified binary data to ./meta_004

$ ls -l meta_002 meta_003 meta_004
-rwxrwxr-x 1 bensley bensley 14136 Aug 16 20:44 meta_002
-rwxr-xr-x 1 bensley bensley 14136 Aug 16 20:47 meta_003
-rwxr-xr-x 1 bensley bensley 14136 Aug 16 20:48 meta_004

$ sha512sum meta_002 meta_003 meta_004
6f35eac11729a34f40af9f6b50bbfe7b637338e318a8643eb5f367f209b1c7d074778e980bd4413aa9cf8d78dd3232df9c9ba0f69b1ecf91746b64a5d1d1ed5b  meta_002
27d24fa51fbf1c840d4ef2d335edbbc26594438ba8ee9b4796139494515ca9778e41ce2952f9066c059914c306904f5de9261ebaafe2011cfacb4b4b43cfad9c  meta_003
86e5984bec88b08bb75717dd881c9f855a8d84366c49de8d87ecbad27a50040659b9a9f42b9eccaa86cbbc7c4f7dc9f0f2768570b7ea5786355ac0a12fd5775f  meta_004
```

### Limitations

It's a bit more tricky to understand what this metamorphic program is doing when examining it with a debugger/disassembler when compard to [../meta_001_c/](../meta_001_c/). There is always the same number of junk assembly sequences spread throughout the file, and they are being replaced with different sequences of random opcodes they are always being replaced *in place*. Shifting the opcodes around will cause the function sizes to change (if the sequences are moved between functions) or even if the function size doesn’t change (the sequences move location within the same function) calls/jumps/returns fail because the destination address becomes misaligned due to the change in function size or offset. To try to obfusacet which sequences of junk assembly are called, this is randomised each time.

Also, when inserting the replacement junk assembly opcodes they aren't "any random opcodes", because this could cause the program to crash or unexpected behaviour to occur. Opcodes are chosen at random from within a certain range, which have been chosen to ensure they have no impact on the successful operation of the rest of the program code. The range is small because this is only an example. The choice and range of opcodes used could of course be expanded.
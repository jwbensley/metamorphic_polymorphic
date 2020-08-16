## meta_001_c


### Overview

 * Upon execution this program loads a copy of itself into memory as raw binary data
 * It then parses the ELF headers to find the .text section (where the executable code lies within the binary file data)
 * Next it scans through the .text section in memory looking for any instances of a random junk assembly sequence encoded as opcodes, and replaces these sequences with new randomly generated sequences
 * The updated binary data in memory is saved to disk (as a new file for demonstration purposes but ideally this would overwrite the original file)
 * The overall effect is that each time the program is run different sets of junk assembly instruction sequences are executed, making the code is metamorphic, but the changing opcodes don't relate to the main program function and so the code is always changing but the main program output/effect is consistent

For the sake of making this example clearer, rather than overwriting itself each time the program is run, the modified output is saved to a new file so that the original and new copy of the program can be disassembled/compared.


### Output

Below it can be seen that when executed for the first time, the program replaces the junk assembly sequences with new ones and saves this to a new file:

```bash
$ gcc -pedantic -Wall -Werror -O0 -o meta_001 meta_001.c

$ ./meta_001
Loaded 13792 bytes from ./meta_001

Junk found, from offset 0xB01 (2817) to 0xB0B (2827)
Replaced: 0x53 0x50 0x48 0x87 0xC0 0x48 0x87 0xC0 0x58 0x5B 
With: 0x53 0x51 0x48 0x87 0xCB 0x48 0x87 0xCB 0x59 0x5B 

Junk found, from offset 0xCC0 (3264) to 0xCCA (3274)
Replaced: 0x53 0x50 0x48 0x87 0xC0 0x48 0x87 0xC0 0x58 0x5B 
With: 0x53 0x52 0x48 0x87 0xD3 0x48 0x87 0xD3 0x5A 0x5B 

Junk found, from offset 0xE5B (3675) to 0xE65 (3685)
Replaced: 0x53 0x50 0x48 0x87 0xC0 0x48 0x87 0xC0 0x58 0x5B 
With: 0x52 0x50 0x48 0x87 0xC2 0x48 0x87 0xC2 0x58 0x5A 

Junk found, from offset 0x1036 (4150) to 0x1040 (4160)
Replaced: 0x53 0x50 0x48 0x87 0xC0 0x48 0x87 0xC0 0x58 0x5B 
With: 0x51 0x52 0x48 0x87 0xD1 0x48 0x87 0xD1 0x5A 0x59 

Junk found, from offset 0x11FC (4604) to 0x1206 (4614)
Replaced: 0x53 0x50 0x48 0x87 0xC0 0x48 0x87 0xC0 0x58 0x5B 
With: 0x53 0x50 0x48 0x87 0xC3 0x48 0x87 0xC3 0x58 0x5B 

All existing junk sequences have been replaced

Wrote modified binary data to ./meta_002
```

When the output file is executed it again replaces the junk assembly sequences with new ones and it can be seen that the origin file and this new file are the same size but the SHA512 hashes are different:

```bash
$ ./meta_002
Loaded 13792 bytes from ./meta_002

Junk found, from offset 0xB01 (2817) to 0xB0B (2827)
Replaced: 0x53 0x51 0x48 0x87 0xCB 0x48 0x87 0xCB 0x59 0x5B 
With: 0x53 0x52 0x48 0x87 0xD3 0x48 0x87 0xD3 0x5A 0x5B 

Junk found, from offset 0xCC0 (3264) to 0xCCA (3274)
Replaced: 0x53 0x52 0x48 0x87 0xD3 0x48 0x87 0xD3 0x5A 0x5B 
With: 0x52 0x50 0x48 0x87 0xC2 0x48 0x87 0xC2 0x58 0x5A 

Junk found, from offset 0xE5B (3675) to 0xE65 (3685)
Replaced: 0x52 0x50 0x48 0x87 0xC2 0x48 0x87 0xC2 0x58 0x5A 
With: 0x51 0x52 0x48 0x87 0xD1 0x48 0x87 0xD1 0x5A 0x59 

Junk found, from offset 0x1036 (4150) to 0x1040 (4160)
Replaced: 0x51 0x52 0x48 0x87 0xD1 0x48 0x87 0xD1 0x5A 0x59 
With: 0x51 0x50 0x48 0x87 0xC1 0x48 0x87 0xC1 0x58 0x59 

Junk found, from offset 0x11FC (4604) to 0x1206 (4614)
Replaced: 0x53 0x50 0x48 0x87 0xC3 0x48 0x87 0xC3 0x58 0x5B 
With: 0x52 0x53 0x48 0x87 0xDA 0x48 0x87 0xDA 0x5B 0x5A 

All existing junk sequences have been replaced

Wrote modified binary data to ./meta_003

$ ls -l meta_001 meta_002 meta_003
-rwxrwxr-x 1 bensley bensley 13792 Aug 16 20:38 meta_001
-rwxr-xr-x 1 bensley bensley 13792 Aug 16 20:38 meta_002
-rwxr-xr-x 1 bensley bensley 13792 Aug 16 20:38 meta_003

$ sha512sum meta_001 meta_002 meta_003
d8169af7d8a392b077b7236a941622181fccf8bad4fe7f08b90a26fb9bb8e542e0d65f1137098af9daea4b8c596fddddc75e153a5513778dfb9066d402f23e35  meta_001
09a39a16781ac308824cc56984872e6202fa405298d30e2526c5d7ee04480505f7dca3d79d9a9d2e80d7cae00c2e0f656dbfc95c576d186f0e0187f6d3030e84  meta_002
78463597b38ed261590ffbb7209e52ab472499a4f3791a99c4b82b298d5e7f72dc10a731bcddfdc221253dcd8a71280190fc21f5a9d5d1ae9566119cc60aec4e  meta_003
```

### Limitations

In reality it is easy to understand what this metamorphic program is doing when examining it with a debugger/disassembler. There is always the same number of junk assembly sequences spread throughout the file, and they are being replaced with different sequences of random opcodes they are always being replaced *in place*. Shifting the opcodes around will cause the function sizes to change (if the sequences are moved between functions) or even if the function size doesn’t change (the sequences move location within the same function) calls/jumps/returns fail because the destination address becomes misaligned due to the change in function size or offset.

Also, when inserting the replacement junk assembly opcodes they aren't "any random opcodes", because this could cause the program to crash or unexpected behaviour to occur. Opcodes are chosen at random from within a certain range, which have been chosen to ensure they have no impact on the successful operation of the rest of the program code. The range is small because this is only an example. The choice and range of opcodes used could of course be expanded.
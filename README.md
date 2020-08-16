
## Metamorphic and Polymorphic Code Examples

This repo contains examples and metamorphic and polymorphic code.


### [meta_001_c/](meta_001_c/)

This example program has a macro defined which contains random junk assembly instructions encoded as opcodes. On each iteration of the program it searches for the sequence of random junk assembly opcodes and replaces them with a different random sequence of junk opcodes.

This example is based on this StackOverflow answer: https://stackoverflow.com/a/16379982
It is a re-write of that StackOverflow answer to run on modern day x86_64 Linux. 


### [meta_002_c/](meta_002_c/)

This is a variation of [meta_001_c/](meta_001_c/) in which multiple functions are defined the random assembly opcodes. On each run the random opcode sequences are replaced with new random sequences but in addition, the calls to these functions is random.
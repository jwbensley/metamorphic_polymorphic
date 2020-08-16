## meta_003_c


### Overview

This is broken becasue it moves the junk assembly sequences around within the file which causes calls/jumps/returns to fail because the offsets they are jump to/from have changed.


### Output


```bash
$ gcc -pedantic -Wall -Werror -O0 -o meta_003 meta_003.c -lm

$ ./meta_003 
Loaded 18312 bytes from ./meta_003

Found .text section
Section offset is 0xC30 (3120), length is 4514 bytes, end is 0x1DD2 (7634)

Found start marker at 0x1D16 (7446)
Found end marker at 0x1D26 (7462)
Junk found, from offset 0x1D1C (7452) to 0x1D26 (7462)
Before:
Offset 0x1D1C (7452) == 50
Offset 0x1D1D (7453) == 50
Offset 0x1D1E (7454) == 48
Offset 0x1D1F (7455) == 87
Offset 0x1D20 (7456) == C0
Offset 0x1D21 (7457) == 48
Offset 0x1D22 (7458) == 87
Offset 0x1D23 (7459) == C0
Offset 0x1D24 (7460) == 58
Offset 0x1D25 (7461) == 58

After:
Offset 0x1D1C (7452) == 90
Offset 0x1D1D (7453) == 90
Offset 0x1D1E (7454) == 90
Offset 0x1D1F (7455) == 90
Offset 0x1D20 (7456) == 90
Offset 0x1D21 (7457) == 90
Offset 0x1D22 (7458) == 90
Offset 0x1D23 (7459) == 90
Offset 0x1D24 (7460) == 90
Offset 0x1D25 (7461) == 90

Removed 1 existing junk sequence(s)
insert_junk
Offset defaulted to 7442
Shifting 0x1D12 (7442) to 0x1D1B (7451), back to 0x1d1c (7452) to 0x1D25 (7461)

Before copy:
Offset 0x1D12 (7442) == 0
Offset 0x1D13 (7443) == 0
Offset 0x1D14 (7444) == EB
Offset 0x1D15 (7445) == 27
Offset 0x1D16 (7446) == 48
Offset 0x1D17 (7447) == 87
Offset 0x1D18 (7448) == D2
Offset 0x1D19 (7449) == 48
Offset 0x1D1A (7450) == 87
Offset 0x1D1B (7451) == D2
Offset 0x1D1C (7452) == 90
Offset 0x1D1D (7453) == 90
Offset 0x1D1E (7454) == 90
Offset 0x1D1F (7455) == 90
Offset 0x1D20 (7456) == 90
Offset 0x1D21 (7457) == 90
Offset 0x1D22 (7458) == 90
Offset 0x1D23 (7459) == 90
Offset 0x1D24 (7460) == 90
Offset 0x1D25 (7461) == 90

After copy:
Offset 0x1D12 (7442) == 0
Offset 0x1D13 (7443) == 0
Offset 0x1D14 (7444) == EB
Offset 0x1D15 (7445) == 27
Offset 0x1D16 (7446) == 48
Offset 0x1D17 (7447) == 87
Offset 0x1D18 (7448) == D2
Offset 0x1D19 (7449) == 48
Offset 0x1D1A (7450) == 87
Offset 0x1D1B (7451) == D2
Offset 0x1D1C (7452) == 0
Offset 0x1D1D (7453) == 0
Offset 0x1D1E (7454) == EB
Offset 0x1D1F (7455) == 27
Offset 0x1D20 (7456) == 48
Offset 0x1D21 (7457) == 87
Offset 0x1D22 (7458) == D2
Offset 0x1D23 (7459) == 48
Offset 0x1D24 (7460) == 87
Offset 0x1D25 (7461) == D2

Inserting junk ASM from 0x1D12 (7442) to 0x1D1B (7451)
Offset 0x1D12 (7442) == 51
Offset 0x1D13 (7443) == 52
Offset 0x1D14 (7444) == 48
Offset 0x1D15 (7445) == 87
Offset 0x1D16 (7446) == D1
Offset 0x1D17 (7447) == 48
Offset 0x1D18 (7448) == 87
Offset 0x1D19 (7449) == D1
Offset 0x1D1A (7450) == 5A
Offset 0x1D1B (7451) == 59
Offset 0x1D1C (7452) == 0
Offset 0x1D1D (7453) == 0
Offset 0x1D1E (7454) == EB
Offset 0x1D1F (7455) == 27
Offset 0x1D20 (7456) == 48
Offset 0x1D21 (7457) == 87
Offset 0x1D22 (7458) == D2
Offset 0x1D23 (7459) == 48
Offset 0x1D24 (7460) == 87
Offset 0x1D25 (7461) == D2

Wrote modified binary data to ./meta_004

$ ./meta_004 
Loaded 18312 bytes from ./meta_004

Found .text section
Section offset is 0xC30 (3120), length is 4514 bytes, end is 0x1DD2 (7634)

Found start marker at 0x1D20 (7456)
Found end marker at 0x1D26 (7462)
Removed 0 existing junk sequence(s)
insert_junk

Wrote modified binary data to ./meta_005

Segmentation fault (core dumped)

```

```bash
$ objdump -d -M intel -S meta_004 > objdump.output
$ start=$(cat objdump.output | grep -n "Disassembly of section .text:" | awk -F ":" '{print $1}')
$ finish=$(cat objdump.output | grep -n "401d5d:" | awk -F ":" '{print $1}')
$ res=$((finish-start))
$ result=$(cat objdump.output | head -$finish | tail -$res)
$ cat objdump.output | head -$finish | tail -$res

0000000000400c30 <_start>:
  400c30:   31 ed                   xor    ebp,ebp
  400c32:   49 89 d1                mov    r9,rdx
  400c35:   5e                      pop    rsi
  400c36:   48 89 e2                mov    rdx,rsp
  400c39:   48 83 e4 f0             and    rsp,0xfffffffffffffff0
  400c3d:   50                      push   rax
  400c3e:   54                      push   rsp
  400c3f:   49 c7 c0 d0 1d 40 00    mov    r8,0x401dd0
  400c46:   48 c7 c1 60 1d 40 00    mov    rcx,0x401d60
  400c4d:   48 c7 c7 46 1c 40 00    mov    rdi,0x401c46
  400c54:   e8 d7 fe ff ff          call   400b30 <__libc_start_main@plt>
  400c59:   f4                      hlt    
  400c5a:   66 0f 1f 44 00 00       nop    WORD PTR [rax+rax*1+0x0]

0000000000400c60 <deregister_tm_clones>:
  400c60:   b8 ff 30 60 00          mov    eax,0x6030ff
  400c65:   55                      push   rbp
  400c66:   48 2d f8 30 60 00       sub    rax,0x6030f8
  400c6c:   48 83 f8 0e             cmp    rax,0xe
  400c70:   48 89 e5                mov    rbp,rsp
  400c73:   76 1b                   jbe    400c90 <deregister_tm_clones+0x30>
  400c75:   b8 00 00 00 00          mov    eax,0x0
  400c7a:   48 85 c0                test   rax,rax
  400c7d:   74 11                   je     400c90 <deregister_tm_clones+0x30>
  400c7f:   5d                      pop    rbp
  400c80:   bf f8 30 60 00          mov    edi,0x6030f8
  400c85:   ff e0                   jmp    rax
  400c87:   66 0f 1f 84 00 00 00    nop    WORD PTR [rax+rax*1+0x0]
  400c8e:   00 00 
  400c90:   5d                      pop    rbp
  400c91:   c3                      ret    
  400c92:   0f 1f 40 00             nop    DWORD PTR [rax+0x0]
  400c96:   66 2e 0f 1f 84 00 00    nop    WORD PTR cs:[rax+rax*1+0x0]
  400c9d:   00 00 00 

0000000000400ca0 <register_tm_clones>:
  400ca0:   be f8 30 60 00          mov    esi,0x6030f8
  400ca5:   55                      push   rbp
  400ca6:   48 81 ee f8 30 60 00    sub    rsi,0x6030f8
  400cad:   48 c1 fe 03             sar    rsi,0x3
  400cb1:   48 89 e5                mov    rbp,rsp
  400cb4:   48 89 f0                mov    rax,rsi
  400cb7:   48 c1 e8 3f             shr    rax,0x3f
  400cbb:   48 01 c6                add    rsi,rax
  400cbe:   48 d1 fe                sar    rsi,1
  400cc1:   74 15                   je     400cd8 <register_tm_clones+0x38>
  400cc3:   b8 00 00 00 00          mov    eax,0x0
  400cc8:   48 85 c0                test   rax,rax
  400ccb:   74 0b                   je     400cd8 <register_tm_clones+0x38>
  400ccd:   5d                      pop    rbp
  400cce:   bf f8 30 60 00          mov    edi,0x6030f8
  400cd3:   ff e0                   jmp    rax
  400cd5:   0f 1f 00                nop    DWORD PTR [rax]
  400cd8:   5d                      pop    rbp
  400cd9:   c3                      ret    
  400cda:   66 0f 1f 44 00 00       nop    WORD PTR [rax+rax*1+0x0]

0000000000400ce0 <__do_global_dtors_aux>:
  400ce0:   80 3d 21 24 20 00 00    cmp    BYTE PTR [rip+0x202421],0x0        # 603108 <completed.7594>
  400ce7:   75 11                   jne    400cfa <__do_global_dtors_aux+0x1a>
  400ce9:   55                      push   rbp
  400cea:   48 89 e5                mov    rbp,rsp
  400ced:   e8 6e ff ff ff          call   400c60 <deregister_tm_clones>
  400cf2:   5d                      pop    rbp
  400cf3:   c6 05 0e 24 20 00 01    mov    BYTE PTR [rip+0x20240e],0x1        # 603108 <completed.7594>
  400cfa:   f3 c3                   repz ret 
  400cfc:   0f 1f 40 00             nop    DWORD PTR [rax+0x0]

0000000000400d00 <frame_dummy>:
  400d00:   bf 10 2e 60 00          mov    edi,0x602e10
  400d05:   48 83 3f 00             cmp    QWORD PTR [rdi],0x0
  400d09:   75 05                   jne    400d10 <frame_dummy+0x10>
  400d0b:   eb 93                   jmp    400ca0 <register_tm_clones>
  400d0d:   0f 1f 00                nop    DWORD PTR [rax]
  400d10:   b8 00 00 00 00          mov    eax,0x0
  400d15:   48 85 c0                test   rax,rax
  400d18:   74 f1                   je     400d0b <frame_dummy+0xb>
  400d1a:   55                      push   rbp
  400d1b:   48 89 e5                mov    rbp,rsp
  400d1e:   ff d0                   call   rax
  400d20:   5d                      pop    rbp
  400d21:   e9 7a ff ff ff          jmp    400ca0 <register_tm_clones>

0000000000400d26 <find_markers>:
  400d26:   55                      push   rbp
  400d27:   48 89 e5                mov    rbp,rsp
  400d2a:   48 83 ec 40             sub    rsp,0x40
  400d2e:   48 89 7d e8             mov    QWORD PTR [rbp-0x18],rdi
  400d32:   48 89 75 e0             mov    QWORD PTR [rbp-0x20],rsi
  400d36:   48 89 55 d8             mov    QWORD PTR [rbp-0x28],rdx
  400d3a:   48 89 4d d0             mov    QWORD PTR [rbp-0x30],rcx
  400d3e:   4c 89 45 c8             mov    QWORD PTR [rbp-0x38],r8
  400d42:   48 8b 45 e0             mov    rax,QWORD PTR [rbp-0x20]
  400d46:   48 89 45 f0             mov    QWORD PTR [rbp-0x10],rax
  400d4a:   e9 9c 01 00 00          jmp    400eeb <find_markers+0x1c5>
  400d4f:   48 8b 55 d0             mov    rdx,QWORD PTR [rbp-0x30]
  400d53:   48 8b 45 f0             mov    rax,QWORD PTR [rbp-0x10]
  400d57:   48 01 d0                add    rax,rdx
  400d5a:   0f b6 00                movzx  eax,BYTE PTR [rax]
  400d5d:   3c 48                   cmp    al,0x48
  400d5f:   0f 85 81 01 00 00       jne    400ee6 <find_markers+0x1c0>
  400d65:   48 8b 45 f0             mov    rax,QWORD PTR [rbp-0x10]
  400d69:   48 8d 50 01             lea    rdx,[rax+0x1]
  400d6d:   48 8b 45 d0             mov    rax,QWORD PTR [rbp-0x30]
  400d71:   48 01 d0                add    rax,rdx
  400d74:   0f b6 00                movzx  eax,BYTE PTR [rax]
  400d77:   3c 87                   cmp    al,0x87
  400d79:   0f 85 67 01 00 00       jne    400ee6 <find_markers+0x1c0>
  400d7f:   48 8b 45 f0             mov    rax,QWORD PTR [rbp-0x10]
  400d83:   48 8d 50 02             lea    rdx,[rax+0x2]
  400d87:   48 8b 45 d0             mov    rax,QWORD PTR [rbp-0x30]
  400d8b:   48 01 d0                add    rax,rdx
  400d8e:   0f b6 00                movzx  eax,BYTE PTR [rax]
  400d91:   3c d2                   cmp    al,0xd2
  400d93:   0f 85 4d 01 00 00       jne    400ee6 <find_markers+0x1c0>
  400d99:   48 8b 45 f0             mov    rax,QWORD PTR [rbp-0x10]
  400d9d:   48 8d 50 03             lea    rdx,[rax+0x3]
  400da1:   48 8b 45 d0             mov    rax,QWORD PTR [rbp-0x30]
  400da5:   48 01 d0                add    rax,rdx
  400da8:   0f b6 00                movzx  eax,BYTE PTR [rax]
  400dab:   3c 48                   cmp    al,0x48
  400dad:   0f 85 33 01 00 00       jne    400ee6 <find_markers+0x1c0>
  400db3:   48 8b 45 f0             mov    rax,QWORD PTR [rbp-0x10]
  400db7:   48 8d 50 04             lea    rdx,[rax+0x4]
  400dbb:   48 8b 45 d0             mov    rax,QWORD PTR [rbp-0x30]
  400dbf:   48 01 d0                add    rax,rdx
  400dc2:   0f b6 00                movzx  eax,BYTE PTR [rax]
  400dc5:   3c 87                   cmp    al,0x87
  400dc7:   0f 85 19 01 00 00       jne    400ee6 <find_markers+0x1c0>
  400dcd:   48 8b 45 f0             mov    rax,QWORD PTR [rbp-0x10]
  400dd1:   48 8d 50 05             lea    rdx,[rax+0x5]
  400dd5:   48 8b 45 d0             mov    rax,QWORD PTR [rbp-0x30]
  400dd9:   48 01 d0                add    rax,rdx
  400ddc:   0f b6 00                movzx  eax,BYTE PTR [rax]
  400ddf:   3c d2                   cmp    al,0xd2
  400de1:   0f 85 ff 00 00 00       jne    400ee6 <find_markers+0x1c0>
  400de7:   48 8b 55 f0             mov    rdx,QWORD PTR [rbp-0x10]
  400deb:   48 8b 45 f0             mov    rax,QWORD PTR [rbp-0x10]
  400def:   48 89 c6                mov    rsi,rax
  400df2:   bf e8 1d 40 00          mov    edi,0x401de8
  400df7:   b8 00 00 00 00          mov    eax,0x0
  400dfc:   e8 0f fd ff ff          call   400b10 <printf@plt>
  400e01:   48 8b 45 f0             mov    rax,QWORD PTR [rbp-0x10]
  400e05:   48 8d 50 06             lea    rdx,[rax+0x6]
  400e09:   48 8b 45 c8             mov    rax,QWORD PTR [rbp-0x38]
  400e0d:   48 89 10                mov    QWORD PTR [rax],rdx
  400e10:   48 8b 45 c8             mov    rax,QWORD PTR [rbp-0x38]
  400e14:   48 8b 00                mov    rax,QWORD PTR [rax]
  400e17:   48 89 45 f8             mov    QWORD PTR [rbp-0x8],rax
  400e1b:   e9 b8 00 00 00          jmp    400ed8 <find_markers+0x1b2>
  400e20:   48 8b 55 d0             mov    rdx,QWORD PTR [rbp-0x30]
  400e24:   48 8b 45 f8             mov    rax,QWORD PTR [rbp-0x8]
  400e28:   48 01 d0                add    rax,rdx
  400e2b:   0f b6 00                movzx  eax,BYTE PTR [rax]
  400e2e:   3c 48                   cmp    al,0x48
  400e30:   0f 85 9d 00 00 00       jne    400ed3 <find_markers+0x1ad>
  400e36:   48 8b 45 f8             mov    rax,QWORD PTR [rbp-0x8]
  400e3a:   48 8d 50 01             lea    rdx,[rax+0x1]
  400e3e:   48 8b 45 d0             mov    rax,QWORD PTR [rbp-0x30]
  400e42:   48 01 d0                add    rax,rdx
  400e45:   0f b6 00                movzx  eax,BYTE PTR [rax]
  400e48:   3c 87                   cmp    al,0x87
  400e4a:   0f 85 83 00 00 00       jne    400ed3 <find_markers+0x1ad>
  400e50:   48 8b 45 f8             mov    rax,QWORD PTR [rbp-0x8]
  400e54:   48 8d 50 02             lea    rdx,[rax+0x2]
  400e58:   48 8b 45 d0             mov    rax,QWORD PTR [rbp-0x30]
  400e5c:   48 01 d0                add    rax,rdx
  400e5f:   0f b6 00                movzx  eax,BYTE PTR [rax]
  400e62:   3c ca                   cmp    al,0xca
  400e64:   75 6d                   jne    400ed3 <find_markers+0x1ad>
  400e66:   48 8b 45 f8             mov    rax,QWORD PTR [rbp-0x8]
  400e6a:   48 8d 50 03             lea    rdx,[rax+0x3]
  400e6e:   48 8b 45 d0             mov    rax,QWORD PTR [rbp-0x30]
  400e72:   48 01 d0                add    rax,rdx
  400e75:   0f b6 00                movzx  eax,BYTE PTR [rax]
  400e78:   3c 48                   cmp    al,0x48
  400e7a:   75 57                   jne    400ed3 <find_markers+0x1ad>
  400e7c:   48 8b 45 f8             mov    rax,QWORD PTR [rbp-0x8]
  400e80:   48 8d 50 04             lea    rdx,[rax+0x4]
  400e84:   48 8b 45 d0             mov    rax,QWORD PTR [rbp-0x30]
  400e88:   48 01 d0                add    rax,rdx
  400e8b:   0f b6 00                movzx  eax,BYTE PTR [rax]
  400e8e:   3c 87                   cmp    al,0x87
  400e90:   75 41                   jne    400ed3 <find_markers+0x1ad>
  400e92:   48 8b 45 f8             mov    rax,QWORD PTR [rbp-0x8]
  400e96:   48 8d 50 05             lea    rdx,[rax+0x5]
  400e9a:   48 8b 45 d0             mov    rax,QWORD PTR [rbp-0x30]
  400e9e:   48 01 d0                add    rax,rdx
  400ea1:   0f b6 00                movzx  eax,BYTE PTR [rax]
  400ea4:   3c ca                   cmp    al,0xca
  400ea6:   75 2b                   jne    400ed3 <find_markers+0x1ad>
  400ea8:   48 8b 55 f8             mov    rdx,QWORD PTR [rbp-0x8]
  400eac:   48 8b 45 f8             mov    rax,QWORD PTR [rbp-0x8]
  400eb0:   48 89 c6                mov    rsi,rax
  400eb3:   bf 10 1e 40 00          mov    edi,0x401e10
  400eb8:   b8 00 00 00 00          mov    eax,0x0
  400ebd:   e8 4e fc ff ff          call   400b10 <printf@plt>
  400ec2:   48 8b 45 f8             mov    rax,QWORD PTR [rbp-0x8]
  400ec6:   48 8d 50 ff             lea    rdx,[rax-0x1]
  400eca:   48 8b 45 d8             mov    rax,QWORD PTR [rbp-0x28]
  400ece:   48 89 10                mov    QWORD PTR [rax],rdx
  400ed1:   eb 26                   jmp    400ef9 <find_markers+0x1d3>
  400ed3:   48 83 45 f8 01          add    QWORD PTR [rbp-0x8],0x1
  400ed8:   48 8b 45 f0             mov    rax,QWORD PTR [rbp-0x10]
  400edc:   48 3b 45 e8             cmp    rax,QWORD PTR [rbp-0x18]
  400ee0:   0f 86 3a ff ff ff       jbe    400e20 <find_markers+0xfa>
  400ee6:   48 83 45 f0 01          add    QWORD PTR [rbp-0x10],0x1
  400eeb:   48 8b 45 f0             mov    rax,QWORD PTR [rbp-0x10]
  400eef:   48 3b 45 e8             cmp    rax,QWORD PTR [rbp-0x18]
  400ef3:   0f 86 56 fe ff ff       jbe    400d4f <find_markers+0x29>
  400ef9:   c9                      leave  
  400efa:   c3                      ret    

0000000000400efb <insert_junk>:
  400efb:   55                      push   rbp
  400efc:   48 89 e5                mov    rbp,rsp
  400eff:   48 83 c4 80             add    rsp,0xffffffffffffff80
  400f03:   48 89 7d 98             mov    QWORD PTR [rbp-0x68],rdi
  400f07:   48 89 75 90             mov    QWORD PTR [rbp-0x70],rsi
  400f0b:   89 55 8c                mov    DWORD PTR [rbp-0x74],edx
  400f0e:   48 89 4d 80             mov    QWORD PTR [rbp-0x80],rcx
  400f12:   64 48 8b 04 25 28 00    mov    rax,QWORD PTR fs:0x28
  400f19:   00 00 
  400f1b:   48 89 45 f8             mov    QWORD PTR [rbp-0x8],rax
  400f1f:   31 c0                   xor    eax,eax
  400f21:   bf 31 1e 40 00          mov    edi,0x401e31
  400f26:   e8 85 fb ff ff          call   400ab0 <puts@plt>
  400f2b:   c7 45 b4 01 00 00 00    mov    DWORD PTR [rbp-0x4c],0x1
  400f32:   e9 44 04 00 00          jmp    40137b <insert_junk+0x480>
  400f37:   48 8b 45 90             mov    rax,QWORD PTR [rbp-0x70]
  400f3b:   48 2b 45 80             sub    rax,QWORD PTR [rbp-0x80]
  400f3f:   8b 75 8c                mov    esi,DWORD PTR [rbp-0x74]
  400f42:   ba 00 00 00 00          mov    edx,0x0
  400f47:   48 f7 f6                div    rsi
  400f4a:   48 85 c0                test   rax,rax
  400f4d:   78 0b                   js     400f5a <insert_junk+0x5f>
  400f4f:   66 0f ef c0             pxor   xmm0,xmm0
  400f53:   f2 48 0f 2a c0          cvtsi2sd xmm0,rax
  400f58:   eb 19                   jmp    400f73 <insert_junk+0x78>
  400f5a:   48 89 c2                mov    rdx,rax
  400f5d:   48 d1 ea                shr    rdx,1
  400f60:   83 e0 01                and    eax,0x1
  400f63:   48 09 c2                or     rdx,rax
  400f66:   66 0f ef c0             pxor   xmm0,xmm0
  400f6a:   f2 48 0f 2a c2          cvtsi2sd xmm0,rdx
  400f6f:   f2 0f 58 c0             addsd  xmm0,xmm0
  400f73:   e8 28 fb ff ff          call   400aa0 <trunc@plt>
  400f78:   66 0f 2e 05 f0 11 00    ucomisd xmm0,QWORD PTR [rip+0x11f0]        # 402170 <_IO_stdin_used+0x390>
  400f7f:   00 
  400f80:   73 0b                   jae    400f8d <insert_junk+0x92>
  400f82:   f2 48 0f 2c c0          cvttsd2si rax,xmm0
  400f87:   48 89 45 e0             mov    QWORD PTR [rbp-0x20],rax
  400f8b:   eb 23                   jmp    400fb0 <insert_junk+0xb5>
  400f8d:   f2 0f 10 0d db 11 00    movsd  xmm1,QWORD PTR [rip+0x11db]        # 402170 <_IO_stdin_used+0x390>
  400f94:   00 
  400f95:   f2 0f 5c c1             subsd  xmm0,xmm1
  400f99:   f2 48 0f 2c c0          cvttsd2si rax,xmm0
  400f9e:   48 89 45 e0             mov    QWORD PTR [rbp-0x20],rax
  400fa2:   48 b8 00 00 00 00 00    movabs rax,0x8000000000000000
  400fa9:   00 00 80 
  400fac:   48 31 45 e0             xor    QWORD PTR [rbp-0x20],rax
  400fb0:   48 8b 45 e0             mov    rax,QWORD PTR [rbp-0x20]
  400fb4:   48 89 45 e0             mov    QWORD PTR [rbp-0x20],rax
  400fb8:   8b 45 b4                mov    eax,DWORD PTR [rbp-0x4c]
  400fbb:   48 0f af 45 e0          imul   rax,QWORD PTR [rbp-0x20]
  400fc0:   48 89 c2                mov    rdx,rax
  400fc3:   48 8b 45 80             mov    rax,QWORD PTR [rbp-0x80]
  400fc7:   48 01 d0                add    rax,rdx
  400fca:   48 89 45 b8             mov    QWORD PTR [rbp-0x48],rax
  400fce:   eb 31                   jmp    401001 <insert_junk+0x106>
  400fd0:   48 8b 55 98             mov    rdx,QWORD PTR [rbp-0x68]
  400fd4:   48 8b 45 b8             mov    rax,QWORD PTR [rbp-0x48]
  400fd8:   48 01 d0                add    rax,rdx
  400fdb:   0f b6 00                movzx  eax,BYTE PTR [rax]
  400fde:   3c 48                   cmp    al,0x48
  400fe0:   74 07                   je     400fe9 <insert_junk+0xee>
  400fe2:   48 83 45 b8 01          add    QWORD PTR [rbp-0x48],0x1
  400fe7:   eb 18                   jmp    401001 <insert_junk+0x106>
  400fe9:   48 8b 45 b8             mov    rax,QWORD PTR [rbp-0x48]
  400fed:   48 89 c6                mov    rsi,rax
  400ff0:   bf 3d 1e 40 00          mov    edi,0x401e3d
  400ff5:   b8 00 00 00 00          mov    eax,0x0
  400ffa:   e8 11 fb ff ff          call   400b10 <printf@plt>
  400fff:   eb 0e                   jmp    40100f <insert_junk+0x114>
  401001:   48 8b 45 90             mov    rax,QWORD PTR [rbp-0x70]
  401005:   48 83 e8 13             sub    rax,0x13
  401009:   48 3b 45 b8             cmp    rax,QWORD PTR [rbp-0x48]
  40100d:   77 c1                   ja     400fd0 <insert_junk+0xd5>
  40100f:   48 8b 45 90             mov    rax,QWORD PTR [rbp-0x70]
  401013:   48 83 e8 13             sub    rax,0x13
  401017:   48 3b 45 b8             cmp    rax,QWORD PTR [rbp-0x48]
  40101b:   77 22                   ja     40103f <insert_junk+0x144>
  40101d:   48 8b 45 90             mov    rax,QWORD PTR [rbp-0x70]
  401021:   48 83 e8 13             sub    rax,0x13
  401025:   48 89 45 b8             mov    QWORD PTR [rbp-0x48],rax
  401029:   48 8b 45 b8             mov    rax,QWORD PTR [rbp-0x48]
  40102d:   48 89 c6                mov    rsi,rax
  401030:   bf 50 1e 40 00          mov    edi,0x401e50
  401035:   b8 00 00 00 00          mov    eax,0x0
  40103a:   e8 d1 fa ff ff          call   400b10 <printf@plt>
  40103f:   48 8b 45 b8             mov    rax,QWORD PTR [rbp-0x48]
  401043:   48 83 c0 0a             add    rax,0xa
  401047:   48 89 45 e8             mov    QWORD PTR [rbp-0x18],rax
  40104b:   48 8b 45 90             mov    rax,QWORD PTR [rbp-0x70]
  40104f:   48 8d 78 f6             lea    rdi,[rax-0xa]
  401053:   48 8b 45 90             mov    rax,QWORD PTR [rbp-0x70]
  401057:   48 8d 48 f6             lea    rcx,[rax-0xa]
  40105b:   48 8b 75 e8             mov    rsi,QWORD PTR [rbp-0x18]
  40105f:   48 8b 55 b8             mov    rdx,QWORD PTR [rbp-0x48]
  401063:   48 8b 45 b8             mov    rax,QWORD PTR [rbp-0x48]
  401067:   48 83 ec 08             sub    rsp,0x8
  40106b:   ff 75 90                push   QWORD PTR [rbp-0x70]
  40106e:   ff 75 90                push   QWORD PTR [rbp-0x70]
  401071:   ff 75 e8                push   QWORD PTR [rbp-0x18]
  401074:   49 89 f1                mov    r9,rsi
  401077:   49 89 f8                mov    r8,rdi
  40107a:   48 89 c6                mov    rsi,rax
  40107d:   bf 70 1e 40 00          mov    edi,0x401e70
  401082:   b8 00 00 00 00          mov    eax,0x0
  401087:   e8 84 fa ff ff          call   400b10 <printf@plt>
  40108c:   48 83 c4 20             add    rsp,0x20
  401090:   bf b9 1e 40 00          mov    edi,0x401eb9
  401095:   e8 16 fa ff ff          call   400ab0 <puts@plt>
  40109a:   48 8b 45 b8             mov    rax,QWORD PTR [rbp-0x48]
  40109e:   48 89 45 c0             mov    QWORD PTR [rbp-0x40],rax
  4010a2:   eb 30                   jmp    4010d4 <insert_junk+0x1d9>
  4010a4:   48 8b 55 98             mov    rdx,QWORD PTR [rbp-0x68]
  4010a8:   48 8b 45 c0             mov    rax,QWORD PTR [rbp-0x40]
  4010ac:   48 01 d0                add    rax,rdx
  4010af:   0f b6 00                movzx  eax,BYTE PTR [rax]
  4010b2:   0f b6 c8                movzx  ecx,al
  4010b5:   48 8b 55 c0             mov    rdx,QWORD PTR [rbp-0x40]
  4010b9:   48 8b 45 c0             mov    rax,QWORD PTR [rbp-0x40]
  4010bd:   48 89 c6                mov    rsi,rax
  4010c0:   bf c7 1e 40 00          mov    edi,0x401ec7
  4010c5:   b8 00 00 00 00          mov    eax,0x0
  4010ca:   e8 41 fa ff ff          call   400b10 <printf@plt>
  4010cf:   48 83 45 c0 01          add    QWORD PTR [rbp-0x40],0x1
  4010d4:   48 8b 45 b8             mov    rax,QWORD PTR [rbp-0x48]
  4010d8:   48 83 c0 13             add    rax,0x13
  4010dc:   48 3b 45 c0             cmp    rax,QWORD PTR [rbp-0x40]
  4010e0:   73 c2                   jae    4010a4 <insert_junk+0x1a9>
  4010e2:   48 8b 45 90             mov    rax,QWORD PTR [rbp-0x70]
  4010e6:   48 89 45 c8             mov    QWORD PTR [rbp-0x38],rax
  4010ea:   eb 24                   jmp    401110 <insert_junk+0x215>
  4010ec:   48 8b 45 c8             mov    rax,QWORD PTR [rbp-0x38]
  4010f0:   48 8d 50 f6             lea    rdx,[rax-0xa]
  4010f4:   48 8b 45 98             mov    rax,QWORD PTR [rbp-0x68]
  4010f8:   48 01 d0                add    rax,rdx
  4010fb:   48 8b 4d 98             mov    rcx,QWORD PTR [rbp-0x68]
  4010ff:   48 8b 55 c8             mov    rdx,QWORD PTR [rbp-0x38]
  401103:   48 01 ca                add    rdx,rcx
  401106:   0f b6 00                movzx  eax,BYTE PTR [rax]
  401109:   88 02                   mov    BYTE PTR [rdx],al
  40110b:   48 83 6d c8 01          sub    QWORD PTR [rbp-0x38],0x1
  401110:   48 8b 45 c8             mov    rax,QWORD PTR [rbp-0x38]
  401114:   48 3b 45 e8             cmp    rax,QWORD PTR [rbp-0x18]
  401118:   73 d2                   jae    4010ec <insert_junk+0x1f1>
  40111a:   bf e1 1e 40 00          mov    edi,0x401ee1
  40111f:   e8 8c f9 ff ff          call   400ab0 <puts@plt>
  401124:   48 8b 45 b8             mov    rax,QWORD PTR [rbp-0x48]
  401128:   48 89 45 d0             mov    QWORD PTR [rbp-0x30],rax
  40112c:   eb 30                   jmp    40115e <insert_junk+0x263>
  40112e:   48 8b 55 98             mov    rdx,QWORD PTR [rbp-0x68]
  401132:   48 8b 45 d0             mov    rax,QWORD PTR [rbp-0x30]
  401136:   48 01 d0                add    rax,rdx
  401139:   0f b6 00                movzx  eax,BYTE PTR [rax]
  40113c:   0f b6 c8                movzx  ecx,al
  40113f:   48 8b 55 d0             mov    rdx,QWORD PTR [rbp-0x30]
  401143:   48 8b 45 d0             mov    rax,QWORD PTR [rbp-0x30]
  401147:   48 89 c6                mov    rsi,rax
  40114a:   bf c7 1e 40 00          mov    edi,0x401ec7
  40114f:   b8 00 00 00 00          mov    eax,0x0
  401154:   e8 b7 f9 ff ff          call   400b10 <printf@plt>
  401159:   48 83 45 d0 01          add    QWORD PTR [rbp-0x30],0x1
  40115e:   48 8b 45 b8             mov    rax,QWORD PTR [rbp-0x48]
  401162:   48 83 c0 13             add    rax,0x13
  401166:   48 3b 45 d0             cmp    rax,QWORD PTR [rbp-0x30]
  40116a:   73 c2                   jae    40112e <insert_junk+0x233>
  40116c:   bf 0a 00 00 00          mov    edi,0xa
  401171:   e8 1a f9 ff ff          call   400a90 <putchar@plt>
  401176:   48 8b 45 b8             mov    rax,QWORD PTR [rbp-0x48]
  40117a:   48 8d 70 09             lea    rsi,[rax+0x9]
  40117e:   48 8b 45 b8             mov    rax,QWORD PTR [rbp-0x48]
  401182:   48 8d 48 09             lea    rcx,[rax+0x9]
  401186:   48 8b 55 b8             mov    rdx,QWORD PTR [rbp-0x48]
  40118a:   48 8b 45 b8             mov    rax,QWORD PTR [rbp-0x48]
  40118e:   49 89 f0                mov    r8,rsi
  401191:   48 89 c6                mov    rsi,rax
  401194:   bf f0 1e 40 00          mov    edi,0x401ef0
  401199:   b8 00 00 00 00          mov    eax,0x0
  40119e:   e8 6d f9 ff ff          call   400b10 <printf@plt>
  4011a3:   bf 00 00 00 00          mov    edi,0x0
  4011a8:   e8 e3 f9 ff ff          call   400b90 <time@plt>
  4011ad:   89 c7                   mov    edi,eax
  4011af:   e8 8c f9 ff ff          call   400b40 <srand@plt>
  4011b4:   e8 47 fa ff ff          call   400c00 <rand@plt>
  4011b9:   89 c2                   mov    edx,eax
  4011bb:   89 d0                   mov    eax,edx
  4011bd:   c1 f8 1f                sar    eax,0x1f
  4011c0:   c1 e8 1e                shr    eax,0x1e
  4011c3:   01 c2                   add    edx,eax
  4011c5:   83 e2 03                and    edx,0x3
  4011c8:   29 c2                   sub    edx,eax
  4011ca:   89 d0                   mov    eax,edx
  4011cc:   88 45 af                mov    BYTE PTR [rbp-0x51],al
  4011cf:   e8 2c fa ff ff          call   400c00 <rand@plt>
  4011d4:   89 c2                   mov    edx,eax
  4011d6:   89 d0                   mov    eax,edx
  4011d8:   c1 f8 1f                sar    eax,0x1f
  4011db:   c1 e8 1e                shr    eax,0x1e
  4011de:   01 c2                   add    edx,eax
  4011e0:   83 e2 03                and    edx,0x3
  4011e3:   29 c2                   sub    edx,eax
  4011e5:   89 d0                   mov    eax,edx
  4011e7:   88 45 ae                mov    BYTE PTR [rbp-0x52],al
  4011ea:   eb 1b                   jmp    401207 <insert_junk+0x30c>
  4011ec:   e8 0f fa ff ff          call   400c00 <rand@plt>
  4011f1:   89 c2                   mov    edx,eax
  4011f3:   89 d0                   mov    eax,edx
  4011f5:   c1 f8 1f                sar    eax,0x1f
  4011f8:   c1 e8 1e                shr    eax,0x1e
  4011fb:   01 c2                   add    edx,eax
  4011fd:   83 e2 03                and    edx,0x3
  401200:   29 c2                   sub    edx,eax
  401202:   89 d0                   mov    eax,edx
  401204:   88 45 ae                mov    BYTE PTR [rbp-0x52],al
  401207:   0f b6 45 ae             movzx  eax,BYTE PTR [rbp-0x52]
  40120b:   3a 45 af                cmp    al,BYTE PTR [rbp-0x51]
  40120e:   74 dc                   je     4011ec <insert_junk+0x2f1>
  401210:   0f b6 45 af             movzx  eax,BYTE PTR [rbp-0x51]
  401214:   83 c0 50                add    eax,0x50
  401217:   88 45 b0                mov    BYTE PTR [rbp-0x50],al
  40121a:   0f b6 45 ae             movzx  eax,BYTE PTR [rbp-0x52]
  40121e:   83 c0 50                add    eax,0x50
  401221:   88 45 b1                mov    BYTE PTR [rbp-0x4f],al
  401224:   0f b6 45 af             movzx  eax,BYTE PTR [rbp-0x51]
  401228:   83 c0 58                add    eax,0x58
  40122b:   88 45 b2                mov    BYTE PTR [rbp-0x4e],al
  40122e:   0f b6 45 ae             movzx  eax,BYTE PTR [rbp-0x52]
  401232:   83 c0 58                add    eax,0x58
  401235:   88 45 b3                mov    BYTE PTR [rbp-0x4d],al
  401238:   c6 45 f0 48             mov    BYTE PTR [rbp-0x10],0x48
  40123c:   c6 45 f1 87             mov    BYTE PTR [rbp-0xf],0x87
  401240:   c6 45 f2 c0             mov    BYTE PTR [rbp-0xe],0xc0
  401244:   0f b6 55 f2             movzx  edx,BYTE PTR [rbp-0xe]
  401248:   0f b6 45 af             movzx  eax,BYTE PTR [rbp-0x51]
  40124c:   01 d0                   add    eax,edx
  40124e:   88 45 f2                mov    BYTE PTR [rbp-0xe],al
  401251:   0f b6 45 f2             movzx  eax,BYTE PTR [rbp-0xe]
  401255:   0f b6 55 ae             movzx  edx,BYTE PTR [rbp-0x52]
  401259:   c1 e2 03                shl    edx,0x3
  40125c:   01 d0                   add    eax,edx
  40125e:   88 45 f2                mov    BYTE PTR [rbp-0xe],al
  401261:   48 8b 55 98             mov    rdx,QWORD PTR [rbp-0x68]
  401265:   48 8b 45 b8             mov    rax,QWORD PTR [rbp-0x48]
  401269:   48 01 c2                add    rdx,rax
  40126c:   0f b6 45 b0             movzx  eax,BYTE PTR [rbp-0x50]
  401270:   88 02                   mov    BYTE PTR [rdx],al
  401272:   48 8b 45 b8             mov    rax,QWORD PTR [rbp-0x48]
  401276:   48 8d 50 01             lea    rdx,[rax+0x1]
  40127a:   48 8b 45 98             mov    rax,QWORD PTR [rbp-0x68]
  40127e:   48 01 c2                add    rdx,rax
  401281:   0f b6 45 b1             movzx  eax,BYTE PTR [rbp-0x4f]
  401285:   88 02                   mov    BYTE PTR [rdx],al
  401287:   48 8b 45 b8             mov    rax,QWORD PTR [rbp-0x48]
  40128b:   48 8d 50 02             lea    rdx,[rax+0x2]
  40128f:   48 8b 45 98             mov    rax,QWORD PTR [rbp-0x68]
  401293:   48 01 c2                add    rdx,rax
  401296:   0f b6 45 f0             movzx  eax,BYTE PTR [rbp-0x10]
  40129a:   88 02                   mov    BYTE PTR [rdx],al
  40129c:   48 8b 45 b8             mov    rax,QWORD PTR [rbp-0x48]
  4012a0:   48 8d 50 03             lea    rdx,[rax+0x3]
  4012a4:   48 8b 45 98             mov    rax,QWORD PTR [rbp-0x68]
  4012a8:   48 01 c2                add    rdx,rax
  4012ab:   0f b6 45 f1             movzx  eax,BYTE PTR [rbp-0xf]
  4012af:   88 02                   mov    BYTE PTR [rdx],al
  4012b1:   48 8b 45 b8             mov    rax,QWORD PTR [rbp-0x48]
  4012b5:   48 8d 50 04             lea    rdx,[rax+0x4]
  4012b9:   48 8b 45 98             mov    rax,QWORD PTR [rbp-0x68]
  4012bd:   48 01 c2                add    rdx,rax
  4012c0:   0f b6 45 f2             movzx  eax,BYTE PTR [rbp-0xe]
  4012c4:   88 02                   mov    BYTE PTR [rdx],al
  4012c6:   48 8b 45 b8             mov    rax,QWORD PTR [rbp-0x48]
  4012ca:   48 8d 50 05             lea    rdx,[rax+0x5]
  4012ce:   48 8b 45 98             mov    rax,QWORD PTR [rbp-0x68]
  4012d2:   48 01 c2                add    rdx,rax
  4012d5:   0f b6 45 f0             movzx  eax,BYTE PTR [rbp-0x10]
  4012d9:   88 02                   mov    BYTE PTR [rdx],al
  4012db:   48 8b 45 b8             mov    rax,QWORD PTR [rbp-0x48]
  4012df:   48 8d 50 06             lea    rdx,[rax+0x6]
  4012e3:   48 8b 45 98             mov    rax,QWORD PTR [rbp-0x68]
  4012e7:   48 01 c2                add    rdx,rax
  4012ea:   0f b6 45 f1             movzx  eax,BYTE PTR [rbp-0xf]
  4012ee:   88 02                   mov    BYTE PTR [rdx],al
  4012f0:   48 8b 45 b8             mov    rax,QWORD PTR [rbp-0x48]
  4012f4:   48 8d 50 07             lea    rdx,[rax+0x7]
  4012f8:   48 8b 45 98             mov    rax,QWORD PTR [rbp-0x68]
  4012fc:   48 01 c2                add    rdx,rax
  4012ff:   0f b6 45 f2             movzx  eax,BYTE PTR [rbp-0xe]
  401303:   88 02                   mov    BYTE PTR [rdx],al
  401305:   48 8b 45 b8             mov    rax,QWORD PTR [rbp-0x48]
  401309:   48 8d 50 08             lea    rdx,[rax+0x8]
  40130d:   48 8b 45 98             mov    rax,QWORD PTR [rbp-0x68]
  401311:   48 01 c2                add    rdx,rax
  401314:   0f b6 45 b3             movzx  eax,BYTE PTR [rbp-0x4d]
  401318:   88 02                   mov    BYTE PTR [rdx],al
  40131a:   48 8b 45 b8             mov    rax,QWORD PTR [rbp-0x48]
  40131e:   48 8d 50 09             lea    rdx,[rax+0x9]
  401322:   48 8b 45 98             mov    rax,QWORD PTR [rbp-0x68]
  401326:   48 01 c2                add    rdx,rax
  401329:   0f b6 45 b2             movzx  eax,BYTE PTR [rbp-0x4e]
  40132d:   88 02                   mov    BYTE PTR [rdx],al
  40132f:   48 8b 45 b8             mov    rax,QWORD PTR [rbp-0x48]
  401333:   48 89 45 d8             mov    QWORD PTR [rbp-0x28],rax
  401337:   eb 30                   jmp    401369 <insert_junk+0x46e>
  401339:   48 8b 55 98             mov    rdx,QWORD PTR [rbp-0x68]
  40133d:   48 8b 45 d8             mov    rax,QWORD PTR [rbp-0x28]
  401341:   48 01 d0                add    rax,rdx
  401344:   0f b6 00                movzx  eax,BYTE PTR [rax]
  401347:   0f b6 c8                movzx  ecx,al
  40134a:   48 8b 55 d8             mov    rdx,QWORD PTR [rbp-0x28]
  40134e:   48 8b 45 d8             mov    rax,QWORD PTR [rbp-0x28]
  401352:   48 89 c6                mov    rsi,rax
  401355:   bf c7 1e 40 00          mov    edi,0x401ec7
  40135a:   b8 00 00 00 00          mov    eax,0x0
  40135f:   e8 ac f7 ff ff          call   400b10 <printf@plt>
  401364:   48 83 45 d8 01          add    QWORD PTR [rbp-0x28],0x1
  401369:   48 8b 45 b8             mov    rax,QWORD PTR [rbp-0x48]
  40136d:   48 83 c0 13             add    rax,0x13
  401371:   48 3b 45 d8             cmp    rax,QWORD PTR [rbp-0x28]
  401375:   73 c2                   jae    401339 <insert_junk+0x43e>
  401377:   83 45 b4 01             add    DWORD PTR [rbp-0x4c],0x1
  40137b:   8b 45 b4                mov    eax,DWORD PTR [rbp-0x4c]
  40137e:   3b 45 8c                cmp    eax,DWORD PTR [rbp-0x74]
  401381:   0f 86 b0 fb ff ff       jbe    400f37 <insert_junk+0x3c>
  401387:   90                      nop
  401388:   48 8b 45 f8             mov    rax,QWORD PTR [rbp-0x8]
  40138c:   64 48 33 04 25 28 00    xor    rax,QWORD PTR fs:0x28
  401393:   00 00 
  401395:   74 05                   je     40139c <insert_junk+0x4a1>
  401397:   e8 64 f7 ff ff          call   400b00 <__stack_chk_fail@plt>
  40139c:   c9                      leave  
  40139d:   c3                      ret    

000000000040139e <load_file>:
  40139e:   55                      push   rbp
  40139f:   48 89 e5                mov    rbp,rsp
  4013a2:   48 83 ec 30             sub    rsp,0x30
  4013a6:   48 89 7d e8             mov    QWORD PTR [rbp-0x18],rdi
  4013aa:   48 89 75 e0             mov    QWORD PTR [rbp-0x20],rsi
  4013ae:   48 89 55 d8             mov    QWORD PTR [rbp-0x28],rdx
  4013b2:   48 8b 45 d8             mov    rax,QWORD PTR [rbp-0x28]
  4013b6:   be 24 1f 40 00          mov    esi,0x401f24
  4013bb:   48 89 c7                mov    rdi,rax
  4013be:   e8 0d f8 ff ff          call   400bd0 <fopen@plt>
  4013c3:   48 89 45 f8             mov    QWORD PTR [rbp-0x8],rax
  4013c7:   48 83 7d f8 00          cmp    QWORD PTR [rbp-0x8],0x0
  4013cc:   75 27                   jne    4013f5 <load_file+0x57>
  4013ce:   48 8b 05 2b 1d 20 00    mov    rax,QWORD PTR [rip+0x201d2b]        # 603100 <stderr@@GLIBC_2.2.5>
  4013d5:   48 8b 55 d8             mov    rdx,QWORD PTR [rbp-0x28]
  4013d9:   be 28 1f 40 00          mov    esi,0x401f28
  4013de:   48 89 c7                mov    rdi,rax
  4013e1:   b8 00 00 00 00          mov    eax,0x0
  4013e6:   e8 85 f7 ff ff          call   400b70 <fprintf@plt>
  4013eb:   b8 01 00 00 00          mov    eax,0x1
  4013f0:   e9 43 01 00 00          jmp    401538 <load_file+0x19a>
  4013f5:   48 8b 45 f8             mov    rax,QWORD PTR [rbp-0x8]
  4013f9:   ba 02 00 00 00          mov    edx,0x2
  4013fe:   be 00 00 00 00          mov    esi,0x0
  401403:   48 89 c7                mov    rdi,rax
  401406:   e8 a5 f7 ff ff          call   400bb0 <fseek@plt>
  40140b:   48 8b 45 f8             mov    rax,QWORD PTR [rbp-0x8]
  40140f:   48 89 c7                mov    rdi,rax
  401412:   e8 69 f7 ff ff          call   400b80 <ftell@plt>
  401417:   48 85 c0                test   rax,rax
  40141a:   7f 1f                   jg     40143b <load_file+0x9d>
  40141c:   48 8b 05 dd 1c 20 00    mov    rax,QWORD PTR [rip+0x201cdd]        # 603100 <stderr@@GLIBC_2.2.5>
  401423:   48 8b 55 d8             mov    rdx,QWORD PTR [rbp-0x28]
  401427:   be 4b 1f 40 00          mov    esi,0x401f4b
  40142c:   48 89 c7                mov    rdi,rax
  40142f:   b8 00 00 00 00          mov    eax,0x0
  401434:   e8 37 f7 ff ff          call   400b70 <fprintf@plt>
  401439:   eb 14                   jmp    40144f <load_file+0xb1>
  40143b:   48 8b 45 f8             mov    rax,QWORD PTR [rbp-0x8]
  40143f:   48 89 c7                mov    rdi,rax
  401442:   e8 39 f7 ff ff          call   400b80 <ftell@plt>
  401447:   89 c2                   mov    edx,eax
  401449:   48 8b 45 e0             mov    rax,QWORD PTR [rbp-0x20]
  40144d:   89 10                   mov    DWORD PTR [rax],edx
  40144f:   48 8b 45 e0             mov    rax,QWORD PTR [rbp-0x20]
  401453:   8b 00                   mov    eax,DWORD PTR [rax]
  401455:   89 c0                   mov    eax,eax
  401457:   48 89 c7                mov    rdi,rax
  40145a:   e8 41 f7 ff ff          call   400ba0 <malloc@plt>
  40145f:   48 89 c2                mov    rdx,rax
  401462:   48 8b 45 e8             mov    rax,QWORD PTR [rbp-0x18]
  401466:   48 89 10                mov    QWORD PTR [rax],rdx
  401469:   48 8b 45 e8             mov    rax,QWORD PTR [rbp-0x18]
  40146d:   48 8b 00                mov    rax,QWORD PTR [rax]
  401470:   48 85 c0                test   rax,rax
  401473:   75 28                   jne    40149d <load_file+0xff>
  401475:   48 8b 05 84 1c 20 00    mov    rax,QWORD PTR [rip+0x201c84]        # 603100 <stderr@@GLIBC_2.2.5>
  40147c:   48 89 c1                mov    rcx,rax
  40147f:   ba 20 00 00 00          mov    edx,0x20
  401484:   be 01 00 00 00          mov    esi,0x1
  401489:   bf 68 1f 40 00          mov    edi,0x401f68
  40148e:   e8 4d f7 ff ff          call   400be0 <fwrite@plt>
  401493:   b8 01 00 00 00          mov    eax,0x1
  401498:   e9 9b 00 00 00          jmp    401538 <load_file+0x19a>
  40149d:   48 8b 45 f8             mov    rax,QWORD PTR [rbp-0x8]
  4014a1:   ba 00 00 00 00          mov    edx,0x0
  4014a6:   be 00 00 00 00          mov    esi,0x0
  4014ab:   48 89 c7                mov    rdi,rax
  4014ae:   e8 fd f6 ff ff          call   400bb0 <fseek@plt>
  4014b3:   48 8b 45 e0             mov    rax,QWORD PTR [rbp-0x20]
  4014b7:   8b 00                   mov    eax,DWORD PTR [rax]
  4014b9:   89 c6                   mov    esi,eax
  4014bb:   48 8b 45 e8             mov    rax,QWORD PTR [rbp-0x18]
  4014bf:   48 8b 00                mov    rax,QWORD PTR [rax]
  4014c2:   48 8b 55 f8             mov    rdx,QWORD PTR [rbp-0x8]
  4014c6:   48 89 d1                mov    rcx,rdx
  4014c9:   ba 01 00 00 00          mov    edx,0x1
  4014ce:   48 89 c7                mov    rdi,rax
  4014d1:   e8 fa f5 ff ff          call   400ad0 <fread@plt>
  4014d6:   48 83 f8 01             cmp    rax,0x1
  4014da:   74 30                   je     40150c <load_file+0x16e>
  4014dc:   48 8b 05 1d 1c 20 00    mov    rax,QWORD PTR [rip+0x201c1d]        # 603100 <stderr@@GLIBC_2.2.5>
  4014e3:   48 8b 55 d8             mov    rdx,QWORD PTR [rbp-0x28]
  4014e7:   be 89 1f 40 00          mov    esi,0x401f89
  4014ec:   48 89 c7                mov    rdi,rax
  4014ef:   b8 00 00 00 00          mov    eax,0x0
  4014f4:   e8 77 f6 ff ff          call   400b70 <fprintf@plt>
  4014f9:   48 8b 45 e8             mov    rax,QWORD PTR [rbp-0x18]
  4014fd:   48 89 c7                mov    rdi,rax
  401500:   e8 7b f5 ff ff          call   400a80 <free@plt>
  401505:   b8 01 00 00 00          mov    eax,0x1
  40150a:   eb 2c                   jmp    401538 <load_file+0x19a>
  40150c:   48 8b 45 f8             mov    rax,QWORD PTR [rbp-0x8]
  401510:   48 89 c7                mov    rdi,rax
  401513:   e8 c8 f5 ff ff          call   400ae0 <fclose@plt>
  401518:   48 8b 45 e0             mov    rax,QWORD PTR [rbp-0x20]
  40151c:   8b 00                   mov    eax,DWORD PTR [rax]
  40151e:   48 8b 55 d8             mov    rdx,QWORD PTR [rbp-0x28]
  401522:   89 c6                   mov    esi,eax
  401524:   bf a4 1f 40 00          mov    edi,0x401fa4
  401529:   b8 00 00 00 00          mov    eax,0x0
  40152e:   e8 dd f5 ff ff          call   400b10 <printf@plt>
  401533:   b8 00 00 00 00          mov    eax,0x0
  401538:   c9                      leave  
  401539:   c3                      ret    

000000000040153a <read_elf_header>:
  40153a:   55                      push   rbp
  40153b:   48 89 e5                mov    rbp,rsp
  40153e:   48 83 ec 50             sub    rsp,0x50
  401542:   48 89 7d c8             mov    QWORD PTR [rbp-0x38],rdi
  401546:   48 89 75 c0             mov    QWORD PTR [rbp-0x40],rsi
  40154a:   48 89 55 b8             mov    QWORD PTR [rbp-0x48],rdx
  40154e:   48 89 4d b0             mov    QWORD PTR [rbp-0x50],rcx
  401552:   48 8b 45 c8             mov    rax,QWORD PTR [rbp-0x38]
  401556:   48 89 45 d8             mov    QWORD PTR [rbp-0x28],rax
  40155a:   48 8b 45 d8             mov    rax,QWORD PTR [rbp-0x28]
  40155e:   ba 04 00 00 00          mov    edx,0x4
  401563:   be be 1f 40 00          mov    esi,0x401fbe
  401568:   48 89 c7                mov    rdi,rax
  40156b:   e8 e0 f5 ff ff          call   400b50 <memcmp@plt>
  401570:   85 c0                   test   eax,eax
  401572:   74 14                   je     401588 <read_elf_header+0x4e>
  401574:   bf c3 1f 40 00          mov    edi,0x401fc3
  401579:   e8 32 f5 ff ff          call   400ab0 <puts@plt>
  40157e:   b8 01 00 00 00          mov    eax,0x1
  401583:   e9 6f 01 00 00          jmp    4016f7 <read_elf_header+0x1bd>
  401588:   48 8b 45 d8             mov    rax,QWORD PTR [rbp-0x28]
  40158c:   48 8b 50 28             mov    rdx,QWORD PTR [rax+0x28]
  401590:   48 8b 45 d8             mov    rax,QWORD PTR [rbp-0x28]
  401594:   0f b7 40 3e             movzx  eax,WORD PTR [rax+0x3e]
  401598:   0f b7 c8                movzx  ecx,ax
  40159b:   48 8b 45 d8             mov    rax,QWORD PTR [rbp-0x28]
  40159f:   0f b7 40 3a             movzx  eax,WORD PTR [rax+0x3a]
  4015a3:   0f b7 c0                movzx  eax,ax
  4015a6:   0f af c1                imul   eax,ecx
  4015a9:   48 98                   cdqe   
  4015ab:   48 01 d0                add    rax,rdx
  4015ae:   48 89 45 e0             mov    QWORD PTR [rbp-0x20],rax
  4015b2:   48 8b 55 c8             mov    rdx,QWORD PTR [rbp-0x38]
  4015b6:   48 8b 45 e0             mov    rax,QWORD PTR [rbp-0x20]
  4015ba:   48 01 d0                add    rax,rdx
  4015bd:   48 89 45 e8             mov    QWORD PTR [rbp-0x18],rax
  4015c1:   66 c7 45 d6 00 00       mov    WORD PTR [rbp-0x2a],0x0
  4015c7:   e9 a8 00 00 00          jmp    401674 <read_elf_header+0x13a>
  4015cc:   48 8b 45 d8             mov    rax,QWORD PTR [rbp-0x28]
  4015d0:   48 8b 50 28             mov    rdx,QWORD PTR [rax+0x28]
  4015d4:   0f b7 4d d6             movzx  ecx,WORD PTR [rbp-0x2a]
  4015d8:   48 8b 45 d8             mov    rax,QWORD PTR [rbp-0x28]
  4015dc:   0f b7 40 3a             movzx  eax,WORD PTR [rax+0x3a]
  4015e0:   0f b7 c0                movzx  eax,ax
  4015e3:   0f af c1                imul   eax,ecx
  4015e6:   48 98                   cdqe   
  4015e8:   48 01 d0                add    rax,rdx
  4015eb:   48 89 45 f0             mov    QWORD PTR [rbp-0x10],rax
  4015ef:   48 8b 55 c8             mov    rdx,QWORD PTR [rbp-0x38]
  4015f3:   48 8b 45 f0             mov    rax,QWORD PTR [rbp-0x10]
  4015f7:   48 01 d0                add    rax,rdx
  4015fa:   48 89 45 f8             mov    QWORD PTR [rbp-0x8],rax
  4015fe:   48 8b 45 e8             mov    rax,QWORD PTR [rbp-0x18]
  401602:   48 8b 50 18             mov    rdx,QWORD PTR [rax+0x18]
  401606:   48 8b 45 f8             mov    rax,QWORD PTR [rbp-0x8]
  40160a:   8b 00                   mov    eax,DWORD PTR [rax]
  40160c:   89 c0                   mov    eax,eax
  40160e:   48 01 c2                add    rdx,rax
  401611:   48 8b 45 c8             mov    rax,QWORD PTR [rbp-0x38]
  401615:   48 01 d0                add    rax,rdx
  401618:   be d9 1f 40 00          mov    esi,0x401fd9
  40161d:   48 89 c7                mov    rdi,rax
  401620:   e8 3b f5 ff ff          call   400b60 <strcmp@plt>
  401625:   85 c0                   test   eax,eax
  401627:   75 40                   jne    401669 <read_elf_header+0x12f>
  401629:   bf df 1f 40 00          mov    edi,0x401fdf
  40162e:   e8 7d f4 ff ff          call   400ab0 <puts@plt>
  401633:   48 8b 45 f8             mov    rax,QWORD PTR [rbp-0x8]
  401637:   48 8b 50 18             mov    rdx,QWORD PTR [rax+0x18]
  40163b:   48 8b 45 b0             mov    rax,QWORD PTR [rbp-0x50]
  40163f:   48 89 10                mov    QWORD PTR [rax],rdx
  401642:   48 8b 45 f8             mov    rax,QWORD PTR [rbp-0x8]
  401646:   48 8b 50 20             mov    rdx,QWORD PTR [rax+0x20]
  40164a:   48 8b 45 b8             mov    rax,QWORD PTR [rbp-0x48]
  40164e:   48 89 10                mov    QWORD PTR [rax],rdx
  401651:   48 8b 45 b0             mov    rax,QWORD PTR [rbp-0x50]
  401655:   48 8b 10                mov    rdx,QWORD PTR [rax]
  401658:   48 8b 45 b8             mov    rax,QWORD PTR [rbp-0x48]
  40165c:   48 8b 00                mov    rax,QWORD PTR [rax]
  40165f:   48 01 c2                add    rdx,rax
  401662:   48 8b 45 c0             mov    rax,QWORD PTR [rbp-0x40]
  401666:   48 89 10                mov    QWORD PTR [rax],rdx
  401669:   0f b7 45 d6             movzx  eax,WORD PTR [rbp-0x2a]
  40166d:   83 c0 01                add    eax,0x1
  401670:   66 89 45 d6             mov    WORD PTR [rbp-0x2a],ax
  401674:   48 8b 45 d8             mov    rax,QWORD PTR [rbp-0x28]
  401678:   0f b7 40 3c             movzx  eax,WORD PTR [rax+0x3c]
  40167c:   66 3b 45 d6             cmp    ax,WORD PTR [rbp-0x2a]
  401680:   0f 87 46 ff ff ff       ja     4015cc <read_elf_header+0x92>
  401686:   48 8b 45 b8             mov    rax,QWORD PTR [rbp-0x48]
  40168a:   48 8b 00                mov    rax,QWORD PTR [rax]
  40168d:   48 85 c0                test   rax,rax
  401690:   75 25                   jne    4016b7 <read_elf_header+0x17d>
  401692:   48 8b 05 67 1a 20 00    mov    rax,QWORD PTR [rip+0x201a67]        # 603100 <stderr@@GLIBC_2.2.5>
  401699:   48 89 c1                mov    rcx,rax
  40169c:   ba 1d 00 00 00          mov    edx,0x1d
  4016a1:   be 01 00 00 00          mov    esi,0x1
  4016a6:   bf f3 1f 40 00          mov    edi,0x401ff3
  4016ab:   e8 30 f5 ff ff          call   400be0 <fwrite@plt>
  4016b0:   b8 01 00 00 00          mov    eax,0x1
  4016b5:   eb 40                   jmp    4016f7 <read_elf_header+0x1bd>
  4016b7:   48 8b 45 c0             mov    rax,QWORD PTR [rbp-0x40]
  4016bb:   48 8b 38                mov    rdi,QWORD PTR [rax]
  4016be:   48 8b 45 c0             mov    rax,QWORD PTR [rbp-0x40]
  4016c2:   48 8b 30                mov    rsi,QWORD PTR [rax]
  4016c5:   48 8b 45 b8             mov    rax,QWORD PTR [rbp-0x48]
  4016c9:   48 8b 08                mov    rcx,QWORD PTR [rax]
  4016cc:   48 8b 45 b0             mov    rax,QWORD PTR [rbp-0x50]
  4016d0:   48 8b 10                mov    rdx,QWORD PTR [rax]
  4016d3:   48 8b 45 b0             mov    rax,QWORD PTR [rbp-0x50]
  4016d7:   48 8b 00                mov    rax,QWORD PTR [rax]
  4016da:   49 89 f9                mov    r9,rdi
  4016dd:   49 89 f0                mov    r8,rsi
  4016e0:   48 89 c6                mov    rsi,rax
  4016e3:   bf 18 20 40 00          mov    edi,0x402018
  4016e8:   b8 00 00 00 00          mov    eax,0x0
  4016ed:   e8 1e f4 ff ff          call   400b10 <printf@plt>
  4016f2:   b8 00 00 00 00          mov    eax,0x0
  4016f7:   c9                      leave  
  4016f8:   c3                      ret    

00000000004016f9 <remove_junk>:
  4016f9:   55                      push   rbp
  4016fa:   48 89 e5                mov    rbp,rsp
  4016fd:   48 83 ec 40             sub    rsp,0x40
  401701:   48 89 7d d8             mov    QWORD PTR [rbp-0x28],rdi
  401705:   48 89 75 d0             mov    QWORD PTR [rbp-0x30],rsi
  401709:   48 89 55 c8             mov    QWORD PTR [rbp-0x38],rdx
  40170d:   48 89 4d c0             mov    QWORD PTR [rbp-0x40],rcx
  401711:   48 8b 45 c0             mov    rax,QWORD PTR [rbp-0x40]
  401715:   48 89 45 e0             mov    QWORD PTR [rbp-0x20],rax
  401719:   e9 4d 02 00 00          jmp    40196b <remove_junk+0x272>
  40171e:   48 8b 55 d8             mov    rdx,QWORD PTR [rbp-0x28]
  401722:   48 8b 45 e0             mov    rax,QWORD PTR [rbp-0x20]
  401726:   48 01 d0                add    rax,rdx
  401729:   0f b6 00                movzx  eax,BYTE PTR [rax]
  40172c:   3c 4f                   cmp    al,0x4f
  40172e:   0f 86 32 02 00 00       jbe    401966 <remove_junk+0x26d>
  401734:   48 8b 55 d8             mov    rdx,QWORD PTR [rbp-0x28]
  401738:   48 8b 45 e0             mov    rax,QWORD PTR [rbp-0x20]
  40173c:   48 01 d0                add    rax,rdx
  40173f:   0f b6 00                movzx  eax,BYTE PTR [rax]
  401742:   3c 53                   cmp    al,0x53
  401744:   0f 87 1c 02 00 00       ja     401966 <remove_junk+0x26d>
  40174a:   48 8b 45 e0             mov    rax,QWORD PTR [rbp-0x20]
  40174e:   48 8d 50 01             lea    rdx,[rax+0x1]
  401752:   48 8b 45 d8             mov    rax,QWORD PTR [rbp-0x28]
  401756:   48 01 d0                add    rax,rdx
  401759:   0f b6 00                movzx  eax,BYTE PTR [rax]
  40175c:   3c 4f                   cmp    al,0x4f
  40175e:   0f 86 02 02 00 00       jbe    401966 <remove_junk+0x26d>
  401764:   48 8b 45 e0             mov    rax,QWORD PTR [rbp-0x20]
  401768:   48 8d 50 01             lea    rdx,[rax+0x1]
  40176c:   48 8b 45 d8             mov    rax,QWORD PTR [rbp-0x28]
  401770:   48 01 d0                add    rax,rdx
  401773:   0f b6 00                movzx  eax,BYTE PTR [rax]
  401776:   3c 53                   cmp    al,0x53
  401778:   0f 87 e8 01 00 00       ja     401966 <remove_junk+0x26d>
  40177e:   48 83 7d e0 46          cmp    QWORD PTR [rbp-0x20],0x46
  401783:   0f 94 c0                sete   al
  401786:   0f b6 d0                movzx  edx,al
  401789:   48 8b 45 d8             mov    rax,QWORD PTR [rbp-0x28]
  40178d:   48 01 d0                add    rax,rdx
  401790:   0f b6 00                movzx  eax,BYTE PTR [rax]
  401793:   84 c0                   test   al,al
  401795:   0f 84 cb 01 00 00       je     401966 <remove_junk+0x26d>
  40179b:   48 8b 45 e0             mov    rax,QWORD PTR [rbp-0x20]
  40179f:   48 8d 50 03             lea    rdx,[rax+0x3]
  4017a3:   48 8b 45 d8             mov    rax,QWORD PTR [rbp-0x28]
  4017a7:   48 01 d0                add    rax,rdx
  4017aa:   0f b6 00                movzx  eax,BYTE PTR [rax]
  4017ad:   3c 87                   cmp    al,0x87
  4017af:   0f 85 b1 01 00 00       jne    401966 <remove_junk+0x26d>
  4017b5:   48 8b 45 e0             mov    rax,QWORD PTR [rbp-0x20]
  4017b9:   48 8d 50 04             lea    rdx,[rax+0x4]
  4017bd:   48 8b 45 d8             mov    rax,QWORD PTR [rbp-0x28]
  4017c1:   48 01 d0                add    rax,rdx
  4017c4:   0f b6 00                movzx  eax,BYTE PTR [rax]
  4017c7:   3c bf                   cmp    al,0xbf
  4017c9:   0f 86 97 01 00 00       jbe    401966 <remove_junk+0x26d>
  4017cf:   48 8b 45 e0             mov    rax,QWORD PTR [rbp-0x20]
  4017d3:   48 8d 50 05             lea    rdx,[rax+0x5]
  4017d7:   48 8b 45 d8             mov    rax,QWORD PTR [rbp-0x28]
  4017db:   48 01 d0                add    rax,rdx
  4017de:   0f b6 00                movzx  eax,BYTE PTR [rax]
  4017e1:   3c 48                   cmp    al,0x48
  4017e3:   0f 85 7d 01 00 00       jne    401966 <remove_junk+0x26d>
  4017e9:   48 8b 45 e0             mov    rax,QWORD PTR [rbp-0x20]
  4017ed:   48 8d 50 06             lea    rdx,[rax+0x6]
  4017f1:   48 8b 45 d8             mov    rax,QWORD PTR [rbp-0x28]
  4017f5:   48 01 d0                add    rax,rdx
  4017f8:   0f b6 00                movzx  eax,BYTE PTR [rax]
  4017fb:   3c 87                   cmp    al,0x87
  4017fd:   0f 85 63 01 00 00       jne    401966 <remove_junk+0x26d>
  401803:   48 8b 45 e0             mov    rax,QWORD PTR [rbp-0x20]
  401807:   48 8d 50 07             lea    rdx,[rax+0x7]
  40180b:   48 8b 45 d8             mov    rax,QWORD PTR [rbp-0x28]
  40180f:   48 01 d0                add    rax,rdx
  401812:   0f b6 00                movzx  eax,BYTE PTR [rax]
  401815:   3c bf                   cmp    al,0xbf
  401817:   0f 86 49 01 00 00       jbe    401966 <remove_junk+0x26d>
  40181d:   48 8b 45 c8             mov    rax,QWORD PTR [rbp-0x38]
  401821:   8b 00                   mov    eax,DWORD PTR [rax]
  401823:   8d 50 01                lea    edx,[rax+0x1]
  401826:   48 8b 45 c8             mov    rax,QWORD PTR [rbp-0x38]
  40182a:   89 10                   mov    DWORD PTR [rax],edx
  40182c:   48 8b 45 e0             mov    rax,QWORD PTR [rbp-0x20]
  401830:   48 8d 70 0a             lea    rsi,[rax+0xa]
  401834:   48 8b 45 e0             mov    rax,QWORD PTR [rbp-0x20]
  401838:   48 8d 48 0a             lea    rcx,[rax+0xa]
  40183c:   48 8b 55 e0             mov    rdx,QWORD PTR [rbp-0x20]
  401840:   48 8b 45 e0             mov    rax,QWORD PTR [rbp-0x20]
  401844:   49 89 f0                mov    r8,rsi
  401847:   48 89 c6                mov    rsi,rax
  40184a:   bf 68 20 40 00          mov    edi,0x402068
  40184f:   b8 00 00 00 00          mov    eax,0x0
  401854:   e8 b7 f2 ff ff          call   400b10 <printf@plt>
  401859:   bf 9c 20 40 00          mov    edi,0x40209c
  40185e:   e8 4d f2 ff ff          call   400ab0 <puts@plt>
  401863:   48 8b 45 e0             mov    rax,QWORD PTR [rbp-0x20]
  401867:   48 89 45 e8             mov    QWORD PTR [rbp-0x18],rax
  40186b:   eb 30                   jmp    40189d <remove_junk+0x1a4>
  40186d:   48 8b 55 d8             mov    rdx,QWORD PTR [rbp-0x28]
  401871:   48 8b 45 e8             mov    rax,QWORD PTR [rbp-0x18]
  401875:   48 01 d0                add    rax,rdx
  401878:   0f b6 00                movzx  eax,BYTE PTR [rax]
  40187b:   0f b6 c8                movzx  ecx,al
  40187e:   48 8b 55 e8             mov    rdx,QWORD PTR [rbp-0x18]
  401882:   48 8b 45 e8             mov    rax,QWORD PTR [rbp-0x18]
  401886:   48 89 c6                mov    rsi,rax
  401889:   bf c7 1e 40 00          mov    edi,0x401ec7
  40188e:   b8 00 00 00 00          mov    eax,0x0
  401893:   e8 78 f2 ff ff          call   400b10 <printf@plt>
  401898:   48 83 45 e8 01          add    QWORD PTR [rbp-0x18],0x1
  40189d:   48 8b 45 e8             mov    rax,QWORD PTR [rbp-0x18]
  4018a1:   48 3b 45 d0             cmp    rax,QWORD PTR [rbp-0x30]
  4018a5:   76 c6                   jbe    40186d <remove_junk+0x174>
  4018a7:   bf 0a 00 00 00          mov    edi,0xa
  4018ac:   e8 df f1 ff ff          call   400a90 <putchar@plt>
  4018b1:   48 8b 45 e0             mov    rax,QWORD PTR [rbp-0x20]
  4018b5:   48 89 45 f0             mov    QWORD PTR [rbp-0x10],rax
  4018b9:   eb 24                   jmp    4018df <remove_junk+0x1e6>
  4018bb:   48 8b 45 f0             mov    rax,QWORD PTR [rbp-0x10]
  4018bf:   48 8d 50 0a             lea    rdx,[rax+0xa]
  4018c3:   48 8b 45 d8             mov    rax,QWORD PTR [rbp-0x28]
  4018c7:   48 01 d0                add    rax,rdx
  4018ca:   48 8b 4d d8             mov    rcx,QWORD PTR [rbp-0x28]
  4018ce:   48 8b 55 f0             mov    rdx,QWORD PTR [rbp-0x10]
  4018d2:   48 01 ca                add    rdx,rcx
  4018d5:   0f b6 00                movzx  eax,BYTE PTR [rax]
  4018d8:   88 02                   mov    BYTE PTR [rdx],al
  4018da:   48 83 45 f0 01          add    QWORD PTR [rbp-0x10],0x1
  4018df:   48 8b 45 d0             mov    rax,QWORD PTR [rbp-0x30]
  4018e3:   48 83 e8 0a             sub    rax,0xa
  4018e7:   48 3b 45 f0             cmp    rax,QWORD PTR [rbp-0x10]
  4018eb:   73 ce                   jae    4018bb <remove_junk+0x1c2>
  4018ed:   48 8b 45 d0             mov    rax,QWORD PTR [rbp-0x30]
  4018f1:   48 8d 50 f7             lea    rdx,[rax-0x9]
  4018f5:   48 8b 45 d8             mov    rax,QWORD PTR [rbp-0x28]
  4018f9:   48 01 d0                add    rax,rdx
  4018fc:   ba 0a 00 00 00          mov    edx,0xa
  401901:   be 90 00 00 00          mov    esi,0x90
  401906:   48 89 c7                mov    rdi,rax
  401909:   e8 12 f2 ff ff          call   400b20 <memset@plt>
  40190e:   bf a4 20 40 00          mov    edi,0x4020a4
  401913:   e8 98 f1 ff ff          call   400ab0 <puts@plt>
  401918:   48 8b 45 e0             mov    rax,QWORD PTR [rbp-0x20]
  40191c:   48 89 45 f8             mov    QWORD PTR [rbp-0x8],rax
  401920:   eb 30                   jmp    401952 <remove_junk+0x259>
  401922:   48 8b 55 d8             mov    rdx,QWORD PTR [rbp-0x28]
  401926:   48 8b 45 f8             mov    rax,QWORD PTR [rbp-0x8]
  40192a:   48 01 d0                add    rax,rdx
  40192d:   0f b6 00                movzx  eax,BYTE PTR [rax]
  401930:   0f b6 c8                movzx  ecx,al
  401933:   48 8b 55 f8             mov    rdx,QWORD PTR [rbp-0x8]
  401937:   48 8b 45 f8             mov    rax,QWORD PTR [rbp-0x8]
  40193b:   48 89 c6                mov    rsi,rax
  40193e:   bf c7 1e 40 00          mov    edi,0x401ec7
  401943:   b8 00 00 00 00          mov    eax,0x0
  401948:   e8 c3 f1 ff ff          call   400b10 <printf@plt>
  40194d:   48 83 45 f8 01          add    QWORD PTR [rbp-0x8],0x1
  401952:   48 8b 45 f8             mov    rax,QWORD PTR [rbp-0x8]
  401956:   48 3b 45 d0             cmp    rax,QWORD PTR [rbp-0x30]
  40195a:   76 c6                   jbe    401922 <remove_junk+0x229>
  40195c:   bf 0a 00 00 00          mov    edi,0xa
  401961:   e8 2a f1 ff ff          call   400a90 <putchar@plt>
  401966:   48 83 45 e0 01          add    QWORD PTR [rbp-0x20],0x1
  40196b:   48 8b 45 e0             mov    rax,QWORD PTR [rbp-0x20]
  40196f:   48 3b 45 d0             cmp    rax,QWORD PTR [rbp-0x30]
  401973:   0f 86 a5 fd ff ff       jbe    40171e <remove_junk+0x25>
  401979:   90                      nop
  40197a:   c9                      leave  
  40197b:   c3                      ret    

000000000040197c <replace_junk>:
  40197c:   55                      push   rbp
  40197d:   48 89 e5                mov    rbp,rsp
  401980:   48 83 ec 50             sub    rsp,0x50
  401984:   48 89 7d c8             mov    QWORD PTR [rbp-0x38],rdi
  401988:   48 89 75 c0             mov    QWORD PTR [rbp-0x40],rsi
  40198c:   48 89 55 b8             mov    QWORD PTR [rbp-0x48],rdx
  401990:   48 89 4d b0             mov    QWORD PTR [rbp-0x50],rcx
  401994:   64 48 8b 04 25 28 00    mov    rax,QWORD PTR fs:0x28
  40199b:   00 00 
  40199d:   48 89 45 f8             mov    QWORD PTR [rbp-0x8],rax
  4019a1:   31 c0                   xor    eax,eax
  4019a3:   48 8b 45 b8             mov    rax,QWORD PTR [rbp-0x48]
  4019a7:   48 89 45 f0             mov    QWORD PTR [rbp-0x10],rax
  4019ab:   e9 b9 00 00 00          jmp    401a69 <replace_junk+0xed>
  4019b0:   48 c7 45 e0 00 00 00    mov    QWORD PTR [rbp-0x20],0x0
  4019b7:   00 
  4019b8:   48 c7 45 e8 00 00 00    mov    QWORD PTR [rbp-0x18],0x0
  4019bf:   00 
  4019c0:   48 8d 7d e8             lea    rdi,[rbp-0x18]
  4019c4:   48 8b 4d b0             mov    rcx,QWORD PTR [rbp-0x50]
  4019c8:   48 8d 55 e0             lea    rdx,[rbp-0x20]
  4019cc:   48 8b 75 f0             mov    rsi,QWORD PTR [rbp-0x10]
  4019d0:   48 8b 45 c8             mov    rax,QWORD PTR [rbp-0x38]
  4019d4:   49 89 f8                mov    r8,rdi
  4019d7:   48 89 c7                mov    rdi,rax
  4019da:   e8 47 f3 ff ff          call   400d26 <find_markers>
  4019df:   48 8b 45 e8             mov    rax,QWORD PTR [rbp-0x18]
  4019e3:   48 85 c0                test   rax,rax
  4019e6:   74 67                   je     401a4f <replace_junk+0xd3>
  4019e8:   48 8b 45 e0             mov    rax,QWORD PTR [rbp-0x20]
  4019ec:   48 85 c0                test   rax,rax
  4019ef:   74 5e                   je     401a4f <replace_junk+0xd3>
  4019f1:   c7 45 dc 00 00 00 00    mov    DWORD PTR [rbp-0x24],0x0
  4019f8:   48 8b 4d e8             mov    rcx,QWORD PTR [rbp-0x18]
  4019fc:   48 8b 75 e0             mov    rsi,QWORD PTR [rbp-0x20]
  401a00:   48 8d 55 dc             lea    rdx,[rbp-0x24]
  401a04:   48 8b 45 b0             mov    rax,QWORD PTR [rbp-0x50]
  401a08:   48 89 c7                mov    rdi,rax
  401a0b:   e8 e9 fc ff ff          call   4016f9 <remove_junk>
  401a10:   8b 45 dc                mov    eax,DWORD PTR [rbp-0x24]
  401a13:   89 c6                   mov    esi,eax
  401a15:   bf b0 20 40 00          mov    edi,0x4020b0
  401a1a:   b8 00 00 00 00          mov    eax,0x0
  401a1f:   e8 ec f0 ff ff          call   400b10 <printf@plt>
  401a24:   48 8b 4d e8             mov    rcx,QWORD PTR [rbp-0x18]
  401a28:   8b 55 dc                mov    edx,DWORD PTR [rbp-0x24]
  401a2b:   48 8b 75 e0             mov    rsi,QWORD PTR [rbp-0x20]
  401a2f:   48 8b 45 b0             mov    rax,QWORD PTR [rbp-0x50]
  401a33:   48 89 c7                mov    rdi,rax
  401a36:   e8 c0 f4 ff ff          call   400efb <insert_junk>
  401a3b:   bf 0a 00 00 00          mov    edi,0xa
  401a40:   e8 4b f0 ff ff          call   400a90 <putchar@plt>
  401a45:   48 8b 45 e0             mov    rax,QWORD PTR [rbp-0x20]
  401a49:   48 89 45 f0             mov    QWORD PTR [rbp-0x10],rax
  401a4d:   eb 15                   jmp    401a64 <replace_junk+0xe8>
  401a4f:   48 8b 45 e8             mov    rax,QWORD PTR [rbp-0x18]
  401a53:   48 85 c0                test   rax,rax
  401a56:   75 0c                   jne    401a64 <replace_junk+0xe8>
  401a58:   48 8b 45 e0             mov    rax,QWORD PTR [rbp-0x20]
  401a5c:   48 85 c0                test   rax,rax
  401a5f:   75 03                   jne    401a64 <replace_junk+0xe8>
  401a61:   90                      nop
  401a62:   eb 13                   jmp    401a77 <replace_junk+0xfb>
  401a64:   48 83 45 f0 01          add    QWORD PTR [rbp-0x10],0x1
  401a69:   48 8b 45 f0             mov    rax,QWORD PTR [rbp-0x10]
  401a6d:   48 3b 45 c8             cmp    rax,QWORD PTR [rbp-0x38]
  401a71:   0f 86 39 ff ff ff       jbe    4019b0 <replace_junk+0x34>
  401a77:   90                      nop
  401a78:   48 8b 45 f8             mov    rax,QWORD PTR [rbp-0x8]
  401a7c:   64 48 33 04 25 28 00    xor    rax,QWORD PTR fs:0x28
  401a83:   00 00 
  401a85:   74 05                   je     401a8c <replace_junk+0x110>
  401a87:   e8 74 f0 ff ff          call   400b00 <__stack_chk_fail@plt>
  401a8c:   c9                      leave  
  401a8d:   c3                      ret    

0000000000401a8e <write_file>:
  401a8e:   55                      push   rbp
  401a8f:   48 89 e5                mov    rbp,rsp
  401a92:   48 83 ec 40             sub    rsp,0x40
  401a96:   48 89 7d d8             mov    QWORD PTR [rbp-0x28],rdi
  401a9a:   89 75 d4                mov    DWORD PTR [rbp-0x2c],esi
  401a9d:   48 89 55 c8             mov    QWORD PTR [rbp-0x38],rdx
  401aa1:   48 8b 45 c8             mov    rax,QWORD PTR [rbp-0x38]
  401aa5:   48 89 c7                mov    rdi,rax
  401aa8:   e8 43 f0 ff ff          call   400af0 <strlen@plt>
  401aad:   83 e8 01                sub    eax,0x1
  401ab0:   89 45 ec                mov    DWORD PTR [rbp-0x14],eax
  401ab3:   8b 45 ec                mov    eax,DWORD PTR [rbp-0x14]
  401ab6:   48 63 d0                movsxd rdx,eax
  401ab9:   48 8b 45 c8             mov    rax,QWORD PTR [rbp-0x38]
  401abd:   48 01 d0                add    rax,rdx
  401ac0:   0f b6 00                movzx  eax,BYTE PTR [rax]
  401ac3:   88 45 eb                mov    BYTE PTR [rbp-0x15],al
  401ac6:   48 8b 45 c8             mov    rax,QWORD PTR [rbp-0x38]
  401aca:   48 89 c7                mov    rdi,rax
  401acd:   e8 1e f1 ff ff          call   400bf0 <strdup@plt>
  401ad2:   48 89 45 f0             mov    QWORD PTR [rbp-0x10],rax
  401ad6:   e8 35 f1 ff ff          call   400c10 <__ctype_b_loc@plt>
  401adb:   48 8b 00                mov    rax,QWORD PTR [rax]
  401ade:   48 0f be 55 eb          movsx  rdx,BYTE PTR [rbp-0x15]
  401ae3:   48 01 d2                add    rdx,rdx
  401ae6:   48 01 d0                add    rax,rdx
  401ae9:   0f b7 00                movzx  eax,WORD PTR [rax]
  401aec:   0f b7 c0                movzx  eax,ax
  401aef:   25 00 08 00 00          and    eax,0x800
  401af4:   85 c0                   test   eax,eax
  401af6:   74 2e                   je     401b26 <write_file+0x98>
  401af8:   0f be 45 eb             movsx  eax,BYTE PTR [rbp-0x15]
  401afc:   8d 48 d1                lea    ecx,[rax-0x2f]
  401aff:   ba 67 66 66 66          mov    edx,0x66666667
  401b04:   89 c8                   mov    eax,ecx
  401b06:   f7 ea                   imul   edx
  401b08:   c1 fa 02                sar    edx,0x2
  401b0b:   89 c8                   mov    eax,ecx
  401b0d:   c1 f8 1f                sar    eax,0x1f
  401b10:   29 c2                   sub    edx,eax
  401b12:   89 d0                   mov    eax,edx
  401b14:   c1 e0 02                shl    eax,0x2
  401b17:   01 d0                   add    eax,edx
  401b19:   01 c0                   add    eax,eax
  401b1b:   29 c1                   sub    ecx,eax
  401b1d:   89 ca                   mov    edx,ecx
  401b1f:   89 d0                   mov    eax,edx
  401b21:   83 c0 30                add    eax,0x30
  401b24:   eb 05                   jmp    401b2b <write_file+0x9d>
  401b26:   b8 30 00 00 00          mov    eax,0x30
  401b2b:   88 45 eb                mov    BYTE PTR [rbp-0x15],al
  401b2e:   8b 45 ec                mov    eax,DWORD PTR [rbp-0x14]
  401b31:   48 63 d0                movsxd rdx,eax
  401b34:   48 8b 45 f0             mov    rax,QWORD PTR [rbp-0x10]
  401b38:   48 01 c2                add    rdx,rax
  401b3b:   0f b6 45 eb             movzx  eax,BYTE PTR [rbp-0x15]
  401b3f:   88 02                   mov    BYTE PTR [rdx],al
  401b41:   48 8b 45 f0             mov    rax,QWORD PTR [rbp-0x10]
  401b45:   be d6 20 40 00          mov    esi,0x4020d6
  401b4a:   48 89 c7                mov    rdi,rax
  401b4d:   e8 7e f0 ff ff          call   400bd0 <fopen@plt>
  401b52:   48 89 45 f8             mov    QWORD PTR [rbp-0x8],rax
  401b56:   48 83 7d f8 00          cmp    QWORD PTR [rbp-0x8],0x0
  401b5b:   75 33                   jne    401b90 <write_file+0x102>
  401b5d:   48 8b 05 9c 15 20 00    mov    rax,QWORD PTR [rip+0x20159c]        # 603100 <stderr@@GLIBC_2.2.5>
  401b64:   48 8b 55 c8             mov    rdx,QWORD PTR [rbp-0x38]
  401b68:   be e0 20 40 00          mov    esi,0x4020e0
  401b6d:   48 89 c7                mov    rdi,rax
  401b70:   b8 00 00 00 00          mov    eax,0x0
  401b75:   e8 f6 ef ff ff          call   400b70 <fprintf@plt>
  401b7a:   48 8b 45 f0             mov    rax,QWORD PTR [rbp-0x10]
  401b7e:   48 89 c7                mov    rdi,rax
  401b81:   e8 fa ee ff ff          call   400a80 <free@plt>
  401b86:   b8 01 00 00 00          mov    eax,0x1
  401b8b:   e9 b4 00 00 00          jmp    401c44 <write_file+0x1b6>
  401b90:   8b 75 d4                mov    esi,DWORD PTR [rbp-0x2c]
  401b93:   48 8b 55 f8             mov    rdx,QWORD PTR [rbp-0x8]
  401b97:   48 8b 45 d8             mov    rax,QWORD PTR [rbp-0x28]
  401b9b:   48 89 d1                mov    rcx,rdx
  401b9e:   ba 01 00 00 00          mov    edx,0x1
  401ba3:   48 89 c7                mov    rdi,rax
  401ba6:   e8 35 f0 ff ff          call   400be0 <fwrite@plt>
  401bab:   48 8b 45 f8             mov    rax,QWORD PTR [rbp-0x8]
  401baf:   48 89 c7                mov    rdi,rax
  401bb2:   e8 09 ef ff ff          call   400ac0 <ferror@plt>
  401bb7:   85 c0                   test   eax,eax
  401bb9:   74 1d                   je     401bd8 <write_file+0x14a>
  401bbb:   48 8b 05 3e 15 20 00    mov    rax,QWORD PTR [rip+0x20153e]        # 603100 <stderr@@GLIBC_2.2.5>
  401bc2:   48 8b 55 c8             mov    rdx,QWORD PTR [rbp-0x38]
  401bc6:   be 08 21 40 00          mov    esi,0x402108
  401bcb:   48 89 c7                mov    rdi,rax
  401bce:   b8 00 00 00 00          mov    eax,0x0
  401bd3:   e8 98 ef ff ff          call   400b70 <fprintf@plt>
  401bd8:   48 8b 45 f8             mov    rax,QWORD PTR [rbp-0x8]
  401bdc:   48 89 c7                mov    rdi,rax
  401bdf:   e8 fc ee ff ff          call   400ae0 <fclose@plt>
  401be4:   48 8b 45 f0             mov    rax,QWORD PTR [rbp-0x10]
  401be8:   be ed 01 00 00          mov    esi,0x1ed
  401bed:   48 89 c7                mov    rdi,rax
  401bf0:   e8 cb ef ff ff          call   400bc0 <chmod@plt>
  401bf5:   85 c0                   test   eax,eax
  401bf7:   74 24                   je     401c1d <write_file+0x18f>
  401bf9:   48 8b 05 00 15 20 00    mov    rax,QWORD PTR [rip+0x201500]        # 603100 <stderr@@GLIBC_2.2.5>
  401c00:   48 8b 55 f0             mov    rdx,QWORD PTR [rbp-0x10]
  401c04:   be 28 21 40 00          mov    esi,0x402128
  401c09:   48 89 c7                mov    rdi,rax
  401c0c:   b8 00 00 00 00          mov    eax,0x0
  401c11:   e8 5a ef ff ff          call   400b70 <fprintf@plt>
  401c16:   b8 01 00 00 00          mov    eax,0x1
  401c1b:   eb 27                   jmp    401c44 <write_file+0x1b6>
  401c1d:   48 8b 45 f0             mov    rax,QWORD PTR [rbp-0x10]
  401c21:   48 89 c6                mov    rsi,rax
  401c24:   bf 48 21 40 00          mov    edi,0x402148
  401c29:   b8 00 00 00 00          mov    eax,0x0
  401c2e:   e8 dd ee ff ff          call   400b10 <printf@plt>
  401c33:   48 8b 45 f0             mov    rax,QWORD PTR [rbp-0x10]
  401c37:   48 89 c7                mov    rdi,rax
  401c3a:   e8 41 ee ff ff          call   400a80 <free@plt>
  401c3f:   b8 00 00 00 00          mov    eax,0x0
  401c44:   c9                      leave  
  401c45:   c3                      ret    

0000000000401c46 <main>:
  401c46:   55                      push   rbp
  401c47:   48 89 e5                mov    rbp,rsp
  401c4a:   48 83 ec 40             sub    rsp,0x40
  401c4e:   89 7d cc                mov    DWORD PTR [rbp-0x34],edi
  401c51:   48 89 75 c0             mov    QWORD PTR [rbp-0x40],rsi
  401c55:   64 48 8b 04 25 28 00    mov    rax,QWORD PTR fs:0x28
  401c5c:   00 00 
  401c5e:   48 89 45 f8             mov    QWORD PTR [rbp-0x8],rax
  401c62:   31 c0                   xor    eax,eax
  401c64:   48 c7 45 d8 00 00 00    mov    QWORD PTR [rbp-0x28],0x0
  401c6b:   00 
  401c6c:   48 8b 45 c0             mov    rax,QWORD PTR [rbp-0x40]
  401c70:   48 8b 10                mov    rdx,QWORD PTR [rax]
  401c73:   48 8d 4d d4             lea    rcx,[rbp-0x2c]
  401c77:   48 8d 45 d8             lea    rax,[rbp-0x28]
  401c7b:   48 89 ce                mov    rsi,rcx
  401c7e:   48 89 c7                mov    rdi,rax
  401c81:   e8 18 f7 ff ff          call   40139e <load_file>
  401c86:   85 c0                   test   eax,eax
  401c88:   74 0a                   je     401c94 <main+0x4e>
  401c8a:   b8 01 00 00 00          mov    eax,0x1
  401c8f:   e9 a9 00 00 00          jmp    401d3d <main+0xf7>
  401c94:   48 c7 45 e0 00 00 00    mov    QWORD PTR [rbp-0x20],0x0
  401c9b:   00 
  401c9c:   48 c7 45 e8 00 00 00    mov    QWORD PTR [rbp-0x18],0x0
  401ca3:   00 
  401ca4:   48 c7 45 f0 00 00 00    mov    QWORD PTR [rbp-0x10],0x0
  401cab:   00 
  401cac:   48 8b 45 d8             mov    rax,QWORD PTR [rbp-0x28]
  401cb0:   48 8d 4d f0             lea    rcx,[rbp-0x10]
  401cb4:   48 8d 55 e8             lea    rdx,[rbp-0x18]
  401cb8:   48 8d 75 e0             lea    rsi,[rbp-0x20]
  401cbc:   48 89 c7                mov    rdi,rax
  401cbf:   e8 76 f8 ff ff          call   40153a <read_elf_header>
  401cc4:   85 c0                   test   eax,eax
  401cc6:   74 07                   je     401ccf <main+0x89>
  401cc8:   b8 01 00 00 00          mov    eax,0x1
  401ccd:   eb 6e                   jmp    401d3d <main+0xf7>
  401ccf:   48 8b 4d d8             mov    rcx,QWORD PTR [rbp-0x28]
  401cd3:   48 8b 55 f0             mov    rdx,QWORD PTR [rbp-0x10]
  401cd7:   48 8b 75 e8             mov    rsi,QWORD PTR [rbp-0x18]
  401cdb:   48 8b 45 e0             mov    rax,QWORD PTR [rbp-0x20]
  401cdf:   48 89 c7                mov    rdi,rax
  401ce2:   e8 95 fc ff ff          call   40197c <replace_junk>
  401ce7:   48 8b 45 c0             mov    rax,QWORD PTR [rbp-0x40]
  401ceb:   48 8b 10                mov    rdx,QWORD PTR [rax]
  401cee:   8b 4d d4                mov    ecx,DWORD PTR [rbp-0x2c]
  401cf1:   48 8b 45 d8             mov    rax,QWORD PTR [rbp-0x28]
  401cf5:   89 ce                   mov    esi,ecx
  401cf7:   48 89 c7                mov    rdi,rax
  401cfa:   e8 8f fd ff ff          call   401a8e <write_file>
  401cff:   85 c0                   test   eax,eax
  401d01:   74 13                   je     401d16 <main+0xd0>
  401d03:   48 8b 45 d8             mov    rax,QWORD PTR [rbp-0x28]
  401d07:   48 89 c7                mov    rdi,rax
  401d0a:   e8 71 ed ff ff          call   400a80 <free@plt>
  401d0f:   b8 01 00 51 52          mov    eax,0x52510001
  401d14:   48 87 d1                xchg   rcx,rdx
  401d17:   48 87 d1                xchg   rcx,rdx
  401d1a:   5a                      pop    rdx
  401d1b:   59                      pop    rcx
  401d1c:   00 00                   add    BYTE PTR [rax],al
  401d1e:   eb 27                   jmp    401d47 <main+0x101>
  401d20:   48 87 d2                xchg   rdx,rdx
  401d23:   48 87 d2                xchg   rdx,rdx
  401d26:   48 87 ca                xchg   rdx,rcx
  401d29:   48 87 ca                xchg   rdx,rcx
  401d2c:   48 8b 45 d8             mov    rax,QWORD PTR [rbp-0x28]
  401d30:   48 89 c7                mov    rdi,rax
  401d33:   e8 48 ed ff ff          call   400a80 <free@plt>
  401d38:   b8 00 00 00 00          mov    eax,0x0
  401d3d:   48 8b 7d f8             mov    rdi,QWORD PTR [rbp-0x8]
  401d41:   64 48 33 3c 25 28 00    xor    rdi,QWORD PTR fs:0x28
  401d48:   00 00 
  401d4a:   74 05                   je     401d51 <main+0x10b>
  401d4c:   e8 af ed ff ff          call   400b00 <__stack_chk_fail@plt>
  401d51:   c9                      leave  
  401d52:   c3                      ret    
  401d53:   66 2e 0f 1f 84 00 00    nop    WORD PTR cs:[rax+rax*1+0x0]
  401d5a:   00 00 00 
  401d5d:   0f 1f 00                nop    DWORD PTR [rax]
```
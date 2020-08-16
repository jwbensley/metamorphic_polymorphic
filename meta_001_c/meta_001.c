#include <ctype.h>     // isdigit()
#include <inttypes.h>  // intN_t, PRIuN, uintN_t
#include <sys/stat.h>  // chmod()
#include <stdio.h>     // fp, fclose(), fopen(), fprintf(), fread(), fseek(),
                       // ftell(), fwrite(), printf(),
                       // SEEK_END, SEEK_SET, stderr
#include <stdlib.h>    // EXIT_FAILURE, EXIT_SUCCESS, free(), malloc()
#include <string.h>    // strchr(), strcmp(), strdup(), strlen()


#define B_PUSH_RAX ".byte 0x50\n\t" // push rax
#define B_PUSH_RBX ".byte 0x53\n\t" // push rbx
#define B_POP_RAX  ".byte 0x58\n\t" // pop rax
#define B_POP_RBX  ".byte 0x5b\n\t" // pop rbx
//#define B_NOP  ".byte 0x90\n\t" // nop
/*
 REX.W + 0x90+r64 == XCHG RAX, r64
 "xchg rax, rax" is NOP (0x90)
 Hackaround by using REX.W flag + opcode 0x87 + r64 to avoid the use of 0x90
*/
#define B_NOP ".byte 0x48,0x87,0xc0\n\t" // REX.W xchg rax,rax

#define H_PUSH     0x50 // push + reg
#define H_POP      0x58 // pop + reg
//#define H_NOP      0x90 // nop
#define H_NOP_0    0x48 // --------------------
#define H_NOP_1    0x87 // REX.W xchg rax,rax |
#define H_NOP_2    0xC0 // --------------------

#define JUNK_ASM __asm__ __volatile__ (B_PUSH_RBX B_PUSH_RAX B_NOP B_NOP B_POP_RAX B_POP_RBX)
#define JUNKLEN 10


int local_rand () {
  
  /*
    This program has such a short lifetime, srand(time(0)) simply isn't random
    enough within a single execution of the program
  */
  int digit;
  FILE *fp;
  fp = fopen("/dev/urandom", "r");
  fread(&digit, 1, 1, fp);
  fclose(fp);
  
  return digit;

}


void insert_junk(uint8_t *file_data, uint64_t junk_start) {

  JUNK_ASM;

  /*
  This is the matrix of source and destination register opcodes for Intel.
  For example;
  0xB8 == "mov"
  0xB8 + 0xC0 == 0x178 "mov eax, eax"
  0xB8 + 0xC8 == 0x180 "mov eax, ebx"

      EAX ECX EDX EBX ESP EBP ESI EDI
  EAX C0  C8  D0  D8  E0  E8  F0  F8
  ECX C1  C9  D1  D9  E1  E9  F1  F9
  EDX C2  CA  D2  DA  E2  EA  F2  FA
  EBX C3  CB  D3  DB  E3  EB  F3  FB
  ESP C4  CC  D4  DC  E4  EC  F4  FC
  EBP C5  CD  D5  DD  E5  ED  F5  FD
  ESI C6  CE  D6  DE  E6  EE  F6  FE
  EDI C7  CF  D7  DF  E7  EF  F7  FF
  */

  /*
   The junk assembly instructions use the following pattern so that they can be identified:
   r1 = random register from RAX, RBX, RCX or RDX
   r2 = a different random register from RAX, RBX, RCX, RDX
   push r1
   push r2
   xchg r1, r2
   xchg r1, r2
   pop r2
   pop r1
  */


  uint8_t reg_1 = (local_rand()%4);
  uint8_t reg_2 = (local_rand()%4);
  
  while(reg_2 == reg_1) {
    reg_2 = (local_rand()%4);
  }

  uint8_t push_r1 = 0x50 + reg_1;
  uint8_t push_r2 = 0x50 + reg_2;

  uint8_t pop_r1 = 0x58 + reg_1;
  uint8_t pop_r2 = 0x58 + reg_2;

  uint8_t nop[3] = {0x48,0x87,0xC0};
  nop[2] += reg_1;
  nop[2] += (reg_2 * 8);

  file_data[junk_start] = push_r1;
  file_data[junk_start + 1] = push_r2;

  file_data[junk_start + 2] = nop[0];
  file_data[junk_start + 3] = nop[1];
  file_data[junk_start + 4] = nop[2];
  file_data[junk_start + 5] = nop[0];
  file_data[junk_start + 6] = nop[1];
  file_data[junk_start + 7] = nop[2];
  file_data[junk_start + 8] = pop_r2;
  file_data[junk_start + 9] = pop_r1;

}


int32_t load_file(uint8_t **file_data, uint32_t *file_len, const char *filename) {

  JUNK_ASM;

  FILE *fp = fopen(filename, "rb");
  if (fp == NULL) {
    fprintf(stderr, "Error opening file %s for reading\n", filename);
    return(EXIT_FAILURE);
  }

  fseek(fp, 0L, SEEK_END);
  if (ftell(fp) < 1) {
    fprintf(stderr, "File %s 0 bytes in length\n", filename);
  } else {
    *file_len = ftell(fp);
  }

  *file_data = malloc(*file_len);
  if (*file_data == NULL) {
    fprintf(stderr, "Failed to malloc space for file\n");
    return(EXIT_FAILURE);
  }

  fseek(fp, 0L, SEEK_SET);
  if (fread((void*)*file_data, *file_len, 1, fp) != 1) {
    fprintf(stderr, "Failed to fread() file %s\n", filename);
    free(file_data);
    return EXIT_FAILURE;
  }
  fclose(fp);

  printf("Loaded %" PRIu32 " bytes from %s\n\n", *file_len, filename);
  return EXIT_SUCCESS;

}


void replace_junk(uint8_t *file_data, uint64_t file_len)
{

  JUNK_ASM;

  for (uint64_t i = 0; i < file_len; i += 1) {

    // Start of the junk ASM
    if (file_data[i] >= H_PUSH && file_data[i] <= (H_PUSH + 3)) {
      if (file_data[i + 1] >= H_PUSH && file_data[i + 1] <= (H_PUSH + 3)) {
        if (file_data[i + 2 == H_NOP_0]) {
          if (file_data[i + 3] == H_NOP_1) {

            printf(
              "Junk found, from offset 0x%" PRIX64 " (%" PRIu64 ") to "
              "0x%" PRIX64 " (%" PRIu64 ")\n",
              i, i, (i + JUNKLEN), (i + JUNKLEN)
            );

            printf("Replaced: ");
            for (uint64_t j = i; j < (i + JUNKLEN); j += 1) {
              printf("0x%" PRIX32 " ", file_data[j]);
            }
            printf("\n");

            insert_junk(file_data, i);

            printf("With: ");
            for (uint64_t j = i; j < (i + JUNKLEN); j += 1) {
              printf("0x%" PRIX32 " ", file_data[j]);
            }
            printf("\n");

            printf("\n");
          }
        }
      }
    }
  }

  printf("All existing junk sequences have been replaced\n\n");
}


int32_t write_file(uint8_t *file_data, uint32_t file_len, const char *filename) {

  JUNK_ASM;

  FILE *fp;  
  int lastoffset = strlen(filename)-1;
  char lastchar = filename[lastoffset];
  char *newfilename = strdup(filename);
  lastchar = '0'+(isdigit(lastchar)?(lastchar-'0'+1)%10:0);
  newfilename[lastoffset] = lastchar; 
  
  fp = fopen(newfilename, "wb");
  if (fp == NULL){
    fprintf(stderr, "Error opening file %s for writing\n", filename);
    free(newfilename);
    return(EXIT_FAILURE);
  }

  fwrite(file_data, file_len, 1, fp);
  if (ferror (fp))
    fprintf(stderr, "Unable to write to new file %s\n", filename);

  fclose(fp);

  if (chmod(newfilename, S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH) != 0) {
    fprintf(stderr, "Unable to chmod file %s\n", newfilename);
    return EXIT_FAILURE;
  }

  printf("Wrote modified binary data to %s\n\n", newfilename);
  free(newfilename);
  return EXIT_SUCCESS;

}


int main(int argc, char* argv[]) {

  JUNK_ASM;

  // Load this file into memory
  uint8_t  *file_data = NULL;
  uint32_t file_len;
  if (load_file(&file_data, &file_len, argv[0]) != EXIT_SUCCESS)
    return EXIT_FAILURE;

  // Replace the existing junk ASM sequences with new ones
  replace_junk(file_data, file_len);

  // Write to disk as new file
  if (write_file(file_data, file_len, argv[0]) != EXIT_SUCCESS) {
    free(file_data);
    return EXIT_FAILURE;
  }

  free(file_data);
  return EXIT_SUCCESS;

}
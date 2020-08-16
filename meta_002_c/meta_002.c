#include <ctype.h>     // isdigit()
#include <inttypes.h>  // intN_t, PRIuN, uintN_t
#include <sys/stat.h>  // chmod()
#include <stdio.h>     // fp, fclose(), fopen(), fprintf(), fread(), fseek(),
                       // ftell(), fwrite(), printf(), SEEK_END, SEEK_SET,
                       // sprintf(), stderr()
#include <stdlib.h>    // EXIT_FAILURE, EXIT_SUCCESS, free(), malloc()
#include <string.h>    // strcpy(), strlen()


/*
  Define the opcodes to built the initial junk assembly state used when the
  code is first compiled
*/
#define B_PUSH_RAX ".byte 0x50\n\t" // push rax
#define B_PUSH_RBX ".byte 0x53\n\t" // push rbx
#define B_POP_RAX  ".byte 0x58\n\t" // pop rax
#define B_POP_RBX  ".byte 0x5b\n\t" // pop rbx
/*
 Hackaround by using REX.W flag + opcode 0x87 + r64 to avoid the use of 0x90:
 REX.W + 0x90+r64 == XCHG RAX, r64
 "xchg rax, rax" is NOP (0x90)
*/
#define B_NOP ".byte 0x48,0x87,0xc0\n\t" // REX.W xchg rax,rax

// Build the initial junk assembly using the byte codes above
#define JUNK_ASM __asm__ __volatile__ (B_PUSH_RBX B_PUSH_RAX B_NOP B_NOP B_POP_RAX B_POP_RBX)
#define JUNKLEN 10


/*
  Define the same opcodes encoded as binary bytes in hex, used at run-time
  to find the junk assemlby in memory
*/
#define H_PUSH     0x50 // push + reg
#define H_POP      0x58 // pop + reg
#define H_NOP_0    0x48 // --------------------
#define H_NOP_1    0x87 // REX.W xchg rax,rax |
#define H_NOP_2    0xC0 // --------------------


/*
  Define the required typdefs and macro which can be copied and pasted
  throughout the code. This will randomly call one of the varying junk
  assembly functions.
*/
int local_rand();
#define RANDOM_CALL jfs.junk_function[local_rand()%4]();

typedef void (*p_junk_function) ();

struct s_junk_functions {
    p_junk_function junk_function[5];
} jfs;


/*****************************************************************************/


void junk_0() {

  printf("junk_0()\n");
  JUNK_ASM;

}


void insert_junk(uint8_t *file_data, uint64_t junk_start) {

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
   The blocks junk assembly instructions use the following pattern so that they
   can be identified:
   r1 = random register from: RAX, RBX, RCX or RDX
   r2 = a different random register from: RAX, RBX, RCX, or RDX
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
    RANDOM_CALL // Having this inisde a loop makes for "nice" debugging :)
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


void junk_1() {

  printf("junk_1()\n");
  JUNK_ASM;

}


int32_t load_file(uint8_t **file_data, uint32_t *file_len, const char *filename) {

  FILE *fp = fopen(filename, "rb");
  if (fp == NULL) {
    fprintf(stderr, "Error opening file %s for reading\n", filename);
    return(EXIT_FAILURE);
  }

  fseek(fp, 0L, SEEK_END);
  if (ftell(fp) < 1) {
    RANDOM_CALL
    fprintf(stderr, "File %s 0 bytes in length\n", filename);
  } else {
    RANDOM_CALL
    *file_len = ftell(fp);
  }

  *file_data = malloc(*file_len);
  if (*file_data == NULL) {
    fprintf(stderr, "Failed to malloc space for file\n");
    return(EXIT_FAILURE);
  }

  RANDOM_CALL

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


void junk_2() {

  printf("junk_2()\n");
  JUNK_ASM;

}


void replace_junk(uint8_t *file_data, uint64_t file_len)
{

  for (uint64_t i = 0; i < file_len; i += 1) {

    // Start of the junk ASM
    if (file_data[i] >= H_PUSH && file_data[i] <= (H_PUSH + 3)) {
      if (file_data[i + 1] >= H_PUSH && file_data[i + 1] <= (H_PUSH + 3)) {
        RANDOM_CALL
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


void junk_3() {

  printf("junk_3()\n");
  JUNK_ASM;

}


int32_t write_file(uint8_t *file_data, uint32_t file_len, const char *filename) {

  char *new_filename = calloc(1, strlen(filename)+2);
  strcpy(new_filename, filename);
  char last_char = filename[strlen(filename)-1];
  if (isdigit(last_char)) {
    int next_digit = (atoi(&last_char) + 1)%10;
    sprintf(&new_filename[strlen(filename)-1], "%d", next_digit);
  } else {
    new_filename[strlen(filename)] = *"0";
  }
  
  FILE *fp;
  fp = fopen(new_filename, "wb");
  if (fp == NULL){
    fprintf(stderr, "Error opening file %s for writing\n", filename);
    free(new_filename);
    return(EXIT_FAILURE);
  }

  RANDOM_CALL

  fwrite(file_data, file_len, 1, fp);
  if (ferror (fp))
    fprintf(stderr, "Unable to write to new file %s\n", filename);

  fclose(fp);

  if (chmod(new_filename, S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH) != 0) {
    fprintf(stderr, "Unable to chmod file %s\n", new_filename);
    return EXIT_FAILURE;
  }

  printf("Wrote modified binary data to %s\n\n", new_filename);
  free(new_filename);
  return EXIT_SUCCESS;

}


void junk_4() {
  
  printf("junk_4()\n");
  JUNK_ASM;

}


int main(int argc, char* argv[]) {

  // Populate pointers to junk assembly functions
  jfs.junk_function[0] = &junk_0;
  jfs.junk_function[1] = &junk_1;
  jfs.junk_function[2] = &junk_2;
  jfs.junk_function[3] = &junk_3;
  jfs.junk_function[4] = &junk_4;

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
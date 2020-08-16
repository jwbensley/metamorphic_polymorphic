#include <ctype.h>     // isdigit()
#include <elf.h>       // EI_NIDENT, Elf64_Ehdr, Elf64_Shdr, ELFMAG, SHN_UNDEF,
#include <inttypes.h>  // intN_t, PRIuN, uintN_t
#include <math.h>      // floor()
#include <sys/stat.h>  // chmod()
#include <stdio.h>     // fp, fclose(), fopen(), fprintf(), fread(), fseek(),
                       // ftell(), fwrite(), memcpy(), memset(), printf(),
                       // SEEK_END, SEEK_SET, stderr
#include <stdlib.h>    // EXIT_FAILURE, EXIT_SUCCESS, free(), malloc(), rand(),
                       // srand()
#include <string.h>    // strchr(), strcmp(), strdup(), strlen()
#include <time.h>      // time()


#define B_PUSH_RAX ".byte 0x50\n\t" // push rax
#define B_POP_RAX  ".byte 0x58\n\t" // pop rax
//#define B_NOP  ".byte 0x90\n\t" // nop
/*
 REX.W + 0x90+r64 == XCHG RAX, r64
 "xchg rax, rax" is NOP (0x90)
 Hackaround by using REX.W flag + opcode 0x87 + r64 to avoid the use of 0x90
*/
#define B_NOP ".byte 0x48,0x87,0xC0\n\t" // REX.W xchg rax,rax

#define H_PUSH_RAX 0x50 // push rax
#define H_POP_RAX  0x58 // pop rax
//#define H_NOP      0x90 // noop
#define H_NOP_0    0x48 // --------------------
#define H_NOP_1    0x87 // REX.W xchg rax,rax |
#define H_NOP_2    0xC0 // --------------------
 

 // REX.W xchg rdx,rdx:
#define START_MARKER __asm__ __volatile__ (".byte 0x48,0x87,0xD2,0x48,0x87,0xD2\n\t")
#define START_1 0x48
#define START_2 0x87
#define START_3 0xD2
// REX.W xchg rcx,rcx:
#define STOP_MARKER  __asm__ __volatile__ (".byte 0x48,0x87,0xCA,0x48,0x87,0xCA\n\t")
#define STOP_1 0x48
#define STOP_2 0x87
#define STOP_3 0xCA

#define JUNK_ASM __asm__ __volatile__ (B_PUSH_RAX B_PUSH_RAX B_NOP B_NOP B_POP_RAX B_POP_RAX)
#define JUNK_LEN 10


/*
  Search through a section of file data looking for function start and
  end markers.
*/
void find_markers(uint64_t code_end, uint64_t code_start, uint64_t *end,
  uint8_t *file_data, uint64_t *start) {

  for(uint64_t i = code_start; i <= code_end; i += 1) {

    if ((file_data[i] == START_1) &&
        (file_data[i + 1] == START_2) &&
        (file_data[i + 2] == START_3) &&
        (file_data[i + 3] == START_1) &&
        (file_data[i + 4] == START_2) &&
        (file_data[i + 5] == START_3)) {

      printf("Found start marker at 0x%" PRIX64 " (%" PRIu64 ")\n", i, i);
      *start = (i + 6);

      for(uint64_t j = *start; i <= code_end; j += 1) {
        if ((file_data[j] == STOP_1) &&
            (file_data[j + 1] == STOP_2) &&
            (file_data[j + 2] == STOP_3) &&
            (file_data[j + 3] == STOP_1) &&
            (file_data[j + 4] == STOP_2) &&
            (file_data[j + 5] == STOP_3))
        {
          printf("Found end marker at 0x%" PRIX64 " (%" PRIu64 ")\n", j, j);
          *end = (j - 1);
          return;
        }
      }

    }

  }

}


/*
  Insert the same number of junk assembly statements that were removed,
  between the start and end markers, to keep the function length the same.
*/
void insert_junk(uint8_t *file_data, uint64_t end, uint32_t junk_count,
  uint64_t start) {

  printf("insert_junk\n");

  /*
    Insert blocks of ASM surrounded by the signature opcodes, junk_count times.
  */
  for (uint32_t i = 1; i <= junk_count; i += 1) {

    // Spread the junk statements roughly evenly throughout the function
    uint64_t junk_offset = floor((end-start)/(junk_count));
    uint64_t junk_start = start + (i * junk_offset);

    /*
      We don't want to insert bytes in the middle of an existing opcode,
      starting from the the semi-random offset search for 0x48, the REX.W flag,
      which signals the start of an opcode using a 64-bit register:
    */
    while(junk_start < (end - (2 * JUNK_LEN) + 1)) {
      if (file_data[junk_start] != 0x48) {
        junk_start += 1;
      } else  {
        printf("Offset set to %" PRIu64 "\n", junk_start);
        break;
      }
    }
    /*
      If the REX.W flag couldn't be found, default to inserting the junk
      assembly at the end.
    */
    if (junk_start >= (end - (2 * JUNK_LEN) + 1)) {
      junk_start = end - (2 * JUNK_LEN) + 1;
      printf("Offset defaulted to %" PRIu64 "\n", junk_start);
    }

    uint64_t junk_end = (junk_start + JUNK_LEN);


    // Shuffle the .text section backwards by JUNK_LEN from the offset
    printf(
      "Shifting 0x%" PRIX64 " (%" PRIu64 ") to 0x%" PRIX64 " (%" PRIu64 ")"
      ", back to 0x%" PRIx64 " (%" PRIu64 ") to 0x%" PRIX64 " (%" PRIu64 ")\n",
      junk_start, junk_start, (end - JUNK_LEN), (end - JUNK_LEN),
      junk_end, junk_end, end, end
    );

    
    printf("\nBefore copy:\n");

    for (uint64_t j = junk_start; j <= junk_start + 19; j += 1){
      printf("Offset 0x%" PRIX64 " (%" PRIu64 ") == %" PRIX32 "\n",
        j, j, file_data[j]);
    }
    

    for (uint64_t j = end; j >= junk_end; j--) {
      memcpy(&file_data[j], &file_data[(j - JUNK_LEN)], 1);
    }

    
    printf("\nAfter copy:\n");

    for (uint64_t j = junk_start; j <= junk_start + 19; j += 1){
      printf("Offset 0x%" PRIX64 " (%" PRIu64 ") == %" PRIX32 "\n",
        j, j, file_data[j]);
    }

    printf("\n");
    

    printf("Inserting junk ASM from 0x%" PRIX64 " (%" PRIu64 ") to 0x%" PRIX64
      " (%" PRIu64 ")\n",
      junk_start, junk_start, (junk_start + JUNK_LEN - 1),
      (junk_start + JUNK_LEN - 1)
    );
 
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

    srand(time(0));

    uint8_t reg_1 = (rand()%4);
    uint8_t reg_2 = (rand()%4);
    
    while(reg_2 == reg_1) {
      reg_2 = (rand()%4);
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

    
    for (uint64_t j = junk_start; j <= junk_start + 19; j += 1){
      printf("Offset 0x%" PRIX64 " (%" PRIu64 ") == %" PRIX32 "\n",
        j, j, file_data[j]);
    }
    

  }

}


int32_t load_file(uint8_t **file_data, uint32_t *file_len, const char *filename) {

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


int32_t read_elf_header(void* file_data, uint64_t *code_end,
  uint64_t *code_len, uint64_t *code_start) {

  Elf64_Ehdr *elf_header;
  elf_header = file_data;

  if (memcmp(elf_header->e_ident, ELFMAG, SELFMAG) != 0) {
    printf("Invalid ELF identity!\n");
    return (EXIT_FAILURE);
  }

  uint64_t sht_e_string_off = elf_header->e_shoff +
  (elf_header->e_shstrndx * elf_header->e_shentsize);

  Elf64_Shdr *shdr_string =
    (Elf64_Shdr*)((uint8_t*)file_data + sht_e_string_off);


  for (uint16_t i = 0; i < elf_header->e_shnum; i++) {

    uint64_t sht_e_offset =
      elf_header->e_shoff + (i * elf_header->e_shentsize);

    Elf64_Shdr *shdr = (Elf64_Shdr*)((uint8_t*)file_data + sht_e_offset);

    if (strcmp((char*)file_data + shdr_string->sh_offset + shdr->sh_name, ".text") == 0) {
      printf("Found .text section\n");
      *code_start = shdr->sh_offset;
      *code_len = shdr->sh_size;
      *code_end = *code_start + *code_len;
    }

  }

  if (*code_len == 0) {
    fprintf(stderr, "Couldn't find .text section!\n");
    return EXIT_FAILURE;
  }

  printf("Section offset is 0x%" PRIX64 " (%" PRIu64 "), length is %"
    PRIu64 " bytes, end is 0x%" PRIX64 " (%" PRIu64 ")\n\n",
    *code_start, *code_start, *code_len, *code_end, *code_end);

  return EXIT_SUCCESS;

}


/*
  Find and remove any junk assembly within the file data between the specific
  start and end offsets passed.
*/
void remove_junk(uint8_t *file_data, uint64_t end, uint32_t *junk_count,
  uint64_t start) {
  
  for (uint64_t i = start; i <= end; i += 1) {

    // Start of the junk ASM?
    if ((file_data[i] >= H_PUSH_RAX) &&
        (file_data[i] <= (H_PUSH_RAX + 3))) {
      
      if ((file_data[i + 1] >= H_PUSH_RAX) &&
          (file_data[i + 1] <= (H_PUSH_RAX + 3))) {
      
        if ((file_data[i + 2 == H_NOP_0]) && 
            (file_data[i + 3] == H_NOP_1) &&
            (file_data[i + 4] >= H_NOP_2) &&
            (file_data[i + 5] == H_NOP_0) &&
            (file_data[i + 6] == H_NOP_1) &&
            (file_data[i + 7] >= H_NOP_2)) {

          *junk_count += 1;

          printf(
            "Junk found, from offset 0x%" PRIX64 " (%" PRIu64 ") to "
            "0x%" PRIX64 " (%" PRIu64 ")\n",
            i, i, (i + JUNK_LEN), (i + JUNK_LEN)
          );

          
          printf("Before:\n");
          //for (uint64_t j = i; j < (i + JUNK_LEN); j += 1) {
          for (uint64_t j = i; j <= end; j += 1) {
            printf("Offset 0x%" PRIX64 " (%" PRIu64 ") == %" PRIX32 "\n",
              j, j, file_data[j]);
          }
          printf("\n");
          

          //uint64_t junk_start = i;
          //uint64_t junk_end = i + JUNK_LEN;

          // Shuffle the .text section forwards by JUNK_LEN
          for (uint64_t j = i; j <= (end - JUNK_LEN); j += 1) {
            memcpy(&file_data[j], &file_data[j + JUNK_LEN], 1);  
          }

          memset(&file_data[(end - (JUNK_LEN - 1))], 0x90, JUNK_LEN);

          
          printf("After:\n");
          //for (uint64_t j = i; j < (i + JUNK_LEN); j += 1) {
          for (uint64_t j = i; j <= end; j += 1) {
            printf("Offset 0x%" PRIX64 " (%" PRIu64 ") == %" PRIX32 "\n",
              j, j, file_data[j]);
          }
          printf("\n");
          

        }
      }
    }
  }
}

/*
  Find all the start and end function markers in the file data, for each
  pair of markers found, remove junk assembly between those markers and
  insert new junk assembly between those markers.
*/
void replace_junk(uint64_t code_end, uint64_t code_len, uint64_t code_start,
  uint8_t *file_data) {

  for (uint64_t i = code_start; i <= code_end; i += 1) {
    uint64_t end = 0;
    uint64_t start = 0;

    find_markers(code_end, i, &end, file_data, &start);

    if (start != 0 && end != 0) {
      uint32_t junk_count = 0;
      remove_junk(file_data, end, &junk_count, start);
      printf("Removed %" PRIu32 " existing junk sequence(s)\n", junk_count);
      insert_junk(file_data, end, junk_count, start);
      printf("\n");
      i = end;

    } else if (start == 0 && end == 0) {
      break;
    }

  }

}


int32_t write_file(uint8_t *file_data, uint32_t file_len, const char *filename) {

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

  // Load this file into memory
  uint8_t  *file_data = NULL;
  uint32_t file_len;
  if (load_file(&file_data, &file_len, argv[0]) != EXIT_SUCCESS)
    return EXIT_FAILURE;

  // Find the start and length of the .text section
  uint64_t code_end = 0;
  uint64_t code_len = 0;
  uint64_t code_start = 0;
  if (read_elf_header(file_data, &code_end, &code_len, &code_start) 
    != EXIT_SUCCESS)
    return EXIT_FAILURE;


  replace_junk(code_end, code_len, code_start, file_data);

  if (write_file(file_data, file_len, argv[0]) != EXIT_SUCCESS) {
    free(file_data);
    return EXIT_FAILURE;
  }

  START_MARKER;
  JUNK_ASM;
  STOP_MARKER;


  free(file_data);
  return EXIT_SUCCESS;

}
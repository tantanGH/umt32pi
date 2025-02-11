#include <stdint.h>
#include <string.h>
#include <doslib.h>
#include <iocslib.h>
#include "rcd.h"

//
//  driver process address
//
static uint8_t* rcd_process_addr = NULL;

//
//  get RCD.X process address
//
static uint8_t* get_rcd_process_addr() {

  uint8_t* addr = NULL;

  // get the current process PSP address by using PDB address
  uint8_t* psp = (uint8_t*)GETPDB() - 16;

  // PSP (Memory Management Pointer) 16 bytes
  //  offset 0x00 ... PREVMEM (Previous memory block address)
  //  offset 0x04 ... MOTHER  (Parent process memory block address)
  //  offset 0x08 ... MEMEND  (Last address of this memory block)
  //  offset 0x0c ... NEXTMEM (Next memory block address)
  //  the following 240 bytes are defined as struct PDBADR in XC library

  // find the root process
  for (;;) {
    uint32_t parent = B_LPEEK((uint32_t*)(psp + 4));
    if (parent == 0) {
      // no more parent process (most likely this is Human68k)
      break;
    }
    //printf("parent=%08X\n", parent);
    psp = (uint8_t*)parent;
  }

  // check memory blocks in the link
  for (;;) {
    //printf("checking process at %08X\n",psp);
    if (B_BPEEK((uint8_t*)(psp+4)) == 0xff) {   // is this a KEEP process?
      //printf("found keep process.\n");
      if (memcmp(psp + 0x100, "RCD 3.", 6) == 0) {
        //printf("found at %08X.\n", psp + 0x10);
        addr = psp + 0x100;
        break;
      }
    }
    uint32_t child = B_LPEEK((uint32_t*)(psp + 12));
    if (child == 0) {
      //printf("not found.\n");
      break;
    }
    psp = (uint8_t*)child;
  }

  return addr;
}

//
//  status
//
int32_t rcd_status() {
  return B_LPEEK((uint32_t*)(rcd_process_addr + 0x40));
}

//
//  pause
//
void rcd_pause() {
  if (B_LPEEK((uint32_t*)(rcd_process_addr + 0x40)) != 0x00000000) {
    B_LPOKE((uint32_t*)(rcd_process_addr + 0x44), 0x00000001);
    B_LPOKE((uint32_t*)(rcd_process_addr + 0x40), 0x00000000);
  }
}

//
//  resume
//
void rcd_resume() {
  if (B_LPEEK((uint32_t*)(rcd_process_addr + 0x44)) == 0x00000001) {
    B_LPOKE((uint32_t*)(rcd_process_addr + 0x44), 0x00000000);
    B_LPOKE((uint32_t*)(rcd_process_addr + 0x40), 0x00000001);
  }
}

//
//  rcd keep check & type check
//
int32_t rcd_keepchk() {

  int16_t check_vectors[] = { 0x8e, 0x43 };   // MIDI, OPM
  int32_t type = -1;

  for (int16_t i = 0; i < 2; i++) {

    uint32_t eye_catch_addr = INTVCG(check_vectors[i]) - 8;

    uint8_t eye_catch[6];
    for (int16_t j = 0; j < 6; j++) {
      eye_catch[j] = B_BPEEK((uint8_t*)(eye_catch_addr + j));
    }

    if (memcmp(eye_catch, "RCD 3.", 6) == 0) {
      type = i;
      rcd_process_addr = get_rcd_process_addr();
      break;
    }
    
  }
  
  return type;
}
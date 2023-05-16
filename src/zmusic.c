#include <stdint.h>
#include <string.h>
#include <doslib.h>
#include <iocslib.h>
#include "zmusic.h"

//
//  status (0x09)
//
int32_t zmusic_status() {

  register uint32_t reg_d0 asm ("d0") = 0x00;
  register uint32_t reg_d1 asm ("d1") = 0x09;
  register uint32_t reg_d2 asm ("d2") = 0x00;

  asm volatile (
    "trap #3\n"
    : "+r"  (reg_d0)    // output (&input) operand
    : "r"   (reg_d1),   // input operand
      "r"   (reg_d2)    // input operand
    :                   // clobbered register
  );

  return reg_d0;
}

//
//  pause (0x0a)
//
void zmusic_pause() {

  register uint32_t reg_d0 asm ("d0") = 0x00;
  register uint32_t reg_d1 asm ("d1") = 0x0a;
  register uint32_t reg_d2 asm ("d2") = 0x00;
  register uint32_t reg_d3 asm ("d3") = 0x00;
  register uint32_t reg_d4 asm ("d4") = 0x00;

  asm volatile (
    "trap #3\n"
    : "+r"  (reg_d0)    // output (&input) operand
    : "r"   (reg_d1),   // input operand
      "r"   (reg_d2),   // input operand
      "r"   (reg_d3),   // input operand
      "r"   (reg_d4)    // input operand
    :                   // clobbered register
  );

  //return reg_d0;
}

//
//  resume (0x0b)
//
void zmusic_resume() {

  register uint32_t reg_d0 asm ("d0") = 0x00;
  register uint32_t reg_d1 asm ("d1") = 0x0b;
  register uint32_t reg_d2 asm ("d2") = 0x00;
  register uint32_t reg_d3 asm ("d3") = 0x00;
  register uint32_t reg_d4 asm ("d4") = 0x00;

  asm volatile (
    "trap #3\n"
    : "+r"  (reg_d0)    // output (&input) operand
    : "r"   (reg_d1),   // input operand
      "r"   (reg_d2),   // input operand
      "r"   (reg_d3),   // input operand
      "r"   (reg_d4)    // input operand
    :                   // clobbered register
  );

  //return reg_d0;
}

//
//  zmusic keep check & type check
//
int32_t zmusic_keepchk() {

  uint32_t eye_catch_addr = INTVCG(0x23) - 8;

  uint8_t eye_catch[6];
  for (int16_t i = 0; i < 6; i++) {
    eye_catch[i] = B_BPEEK((uint8_t*)(eye_catch_addr + i));
  }

  uint8_t type = B_BPEEK((uint8_t*)(eye_catch_addr + 7)) >> 4;

  return (memcmp(eye_catch, "ZmuSiC", 6) == 0) ? type : -1;
}
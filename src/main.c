#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <doslib.h>
#include <iocslib.h>
#include "zmusic.h"
#include "rcd.h"
#include "umt32pi.h"

//
//  send MIDI exclusive message
//
void send_exclusive_message(int16_t interface, uint8_t* sysex, size_t sysex_len) {

  if (interface == INTERFACE_MIDI_BOARD) {

    for (size_t i = 0; i < sysex_len; i++) {
      B_BPEEK((uint8_t*)0xe9a001);              // dummy OPM read for wait
      B_BPOKE((uint8_t*)0xeafa03, 5);           // register group 5
      B_BPEEK((uint8_t*)0xe9a001);              // wait
      B_BPOKE((uint8_t*)0xeafa0d, sysex[i]);
    }

  } else if (interface == INTERFACE_RS_MIDI) {
    
    for (size_t i = 0; i < sysex_len; i++) {
      while ((B_BPEEK((uint8_t*)0xe98005) & 0x04) == 0) {
        ;
      }
      B_BPOKE((uint8_t*)0xe98007, sysex[i]);
    }

  }

}

//
//  show help message
//
static void show_help_message() {
  printf("usage: umt32pi <command> [parameter...]\n");
  printf("command:\n");
  printf("    gsvol <vol>    ... [GS] master volume (0-127)\n");
  printf("    gsreset        ... [GS] reset\n");
  printf("    reverb <type>  ... [GS] reverb type (0-7)\n");
  printf("    chorus <type>  ... [GS] chorus type (0-7)\n");
  printf("    print <str>    ... [GS] print string (max 32chars)\n");
  printf("    mtvol <vol>    ... [mt32-pi] master volume (0-100)\n");
  printf("    synth <n>      ... [mt32-pi] 0:MT-32 1:SoundFont\n");
  printf("    sfont <index>  ... [mt32-pi] sound font (0-127)\n");
  printf("    reboot         ... [mt32-pi] reboot Raspberry Pi\n");
}

//
//  main
//
int32_t main(int32_t argc, uint8_t* argv[]) {

  // default exit code
  int32_t rc = 0;

  // credit
  printf("UMT32PI.X - A simple mt32-pi and GS MIDI control utility version " PROGRAM_VERSION " tantan\n");

  // check command line
  if (argc < 2) {
    show_help_message();
    rc = 1;
    goto exit;
  }

  // zmusic check
  int32_t zmusic_type = zmusic_keepchk();
  if (zmusic_type == 2) {
    printf("MIDI driver: ZMUSIC RS-MIDI\n");
  } else if (zmusic_type >= 0) {
    printf("MIDI driver: ZMUSIC\n");
  }

  // rcd check
  int32_t rcd_mode = rcd_keepchk();
  if (rcd_mode == 1) {
    printf("MIDI driver: RCD RS-MIDI\n");
  } else if (rcd_mode == 0) {
    printf("MIDI driver: RCD\n");
  }

  // one must be running
  if (zmusic_type < 0 && rcd_mode < 0) {
    printf("error: ZMUSIC/RCD MIDI driver is not running.\n");
    rc = -1;
    goto exit;
  }

  // pause
  int16_t paused = 0;
  if (zmusic_type >= 0 && zmusic_status() != 0) {
    zmusic_pause();
    paused = 1;
  }
  if (rcd_mode >= 0 && rcd_status() != 0) {
    rcd_pause();
    paused = 2;
  }

  // midi interface
  int16_t midi_if = (zmusic_type == 2 || rcd_mode == 1) ? INTERFACE_RS_MIDI : INTERFACE_MIDI_BOARD;

  // sysex command and parameters
  uint8_t* sysex_command = argv[1];
  uint8_t* sysex_param = argc > 2 ? argv[2] : NULL;
  uint8_t* sysex_param2 = argc > 3 ? argv[3] : NULL;

  if (stricmp(sysex_command, "gsvol") == 0) {

    // master volume (GS)
    int16_t volume = sysex_param != NULL ? atoi(sysex_param) : -1;
    if (volume >= 0 && volume <= 127) {
      //uint8_t sysex_mes[] = { 0xf0, 0x7f, 0x7f, 0x04, 0x01, 0x00, volume, 0xf7 }; // GM master volume sysex
      uint8_t sysex_mes[] = { 0xf0, 0x41, 0x10, 0x42, 0x12, 0x40, 0x00, 0x04, volume, (0x40 + 0x04 + volume) > 0x80 ? 0x100 - (0x40 + 0x04 + volume) : 0x80 - (0x40 + 0x04 + volume), 0xf7 };
      send_exclusive_message(midi_if, sysex_mes, sizeof(sysex_mes));
      printf("sent GS master volume command. (%d)\n", volume);
    } else {
      printf("error: volume must be 0 - 127.\n");
      rc = -1;
    }

  } else if (stricmp(sysex_command, "gsreset") == 0) {

    // reset (GS)
    uint8_t sysex_mes[]= { 0xf0, 0x41, 0x10, 0x42, 0x12, 0x40, 0x00, 0x7f, 0x00, 0x41, 0xf7 };
    send_exclusive_message(midi_if, sysex_mes, sizeof(sysex_mes));
    printf("sent GS reset command.\n");

  } else if (stricmp(sysex_command, "reverb") == 0) {

    // reberb type (GS)
    int16_t reberb = sysex_param != NULL ? atoi(sysex_param) : -1;
    if (reberb >= 0 && reberb <= 7) {
      uint8_t sysex_mes[] = { 0xf0, 0x41, 0x10, 0x42, 0x12, 0x40, 0x01, 0x30, reberb, 15 - reberb, 0xf7 };
      send_exclusive_message(midi_if, sysex_mes, sizeof(sysex_mes));
      printf("sent reberb type command. (%d:%s)\n", reberb,
            reberb == 0 ? "Room1" :
            reberb == 1 ? "Room2" :
            reberb == 2 ? "Room3" :
            reberb == 3 ? "Hall1" :
            reberb == 4 ? "Hall2 (default)" :
            reberb == 5 ? "Plate" :
            reberb == 6 ? "Delay" :
            reberb == 7 ? "Panning Delay" : "");
    } else {
      printf("error: reberb type must be 0 to 7.\n");
      rc = -1;
    }

  } else if (stricmp(sysex_command, "chorus") == 0) {

    // chorus type (GS)
    int16_t chorus = sysex_param != NULL ? atoi(sysex_param) : -1;
    if (chorus >= 0 && chorus <= 7) {
      uint8_t sysex_mes[] = { 0xf0, 0x41, 0x10, 0x42, 0x12, 0x40, 0x01, 0x38, chorus, 7 - chorus, 0xf7 };
      send_exclusive_message(midi_if, sysex_mes, sizeof(sysex_mes));
      printf("sent chorus type command. (%d:%s)\n", chorus,
            chorus == 0 ? "Chorus1" :
            chorus == 1 ? "Chorus2" :
            chorus == 2 ? "Chorus3 (default)" :
            chorus == 3 ? "Chorus4" :
            chorus == 4 ? "Feedback Chorus" :
            chorus == 5 ? "Flanger" :
            chorus == 6 ? "Short Delay" :
            chorus == 7 ? "Short Delay Feedback" : "");
    } else {
      printf("error: chorus type must be 0 to 7.\n");
      rc = -1;
    }

  } else if (stricmp(sysex_command, "print") == 0) {

    // print string (GS)
    int16_t mes_len = strlen(sysex_param);
    if (sysex_param != NULL && mes_len <= 32) {
      uint8_t sysex_mes[ 8 + mes_len + 1 + 1 ];
      uint8_t sysex_header[] = { 0xf0, 0x41, 0x10, 0x45, 0x12, 0x10, 0x00, 0x00 };
      memcpy(sysex_mes, sysex_header, 8);
      memcpy(sysex_mes + 8, sysex_param, mes_len);
      uint16_t sum = 0x10 + 0x00 + 0x00;
      for (int16_t i = 0; i < mes_len; i++) {
        sum = ( sum + sysex_param[i] ) & 0x7f;
      }
      sysex_mes[ 8 + mes_len ] = 0x80 - sum;
      sysex_mes[ 8 + mes_len + 1 ] = 0xf7;
      send_exclusive_message(midi_if, sysex_mes, mes_len + 10);
      printf("sent print command. (%s)\n", sysex_param);
    } else {
      printf("error: print string must be 1 - 32 ascii chars.\n");
      rc = -1;
    }
  
  } else if (stricmp(sysex_command, "mtvol") == 0) {

    // master volume (mt32-pi)
    int16_t volume = sysex_param != NULL ? atoi(sysex_param) : -1;
    if (volume >= 0 && volume <= 100) {
      uint8_t sysex_mes[] = { 0xf0, 0x41, 0x10, 0x16, 0x12, 0x10, 0x00, 0x16, volume, (0x10 + 0x16 + volume) > 0x80 ? 0x100 - (0x10 + 0x16 + volume) : 0x80 - (0x10 + 0x16 + volume), 0xf7 };
      send_exclusive_message(midi_if, sysex_mes, sizeof(sysex_mes));
      printf("sent master volume command. (%d)\n", volume);
    } else {
      printf("error: volume must be 0 - 100.\n");
      rc = -1;
    }

  } else if (stricmp(sysex_command, "synth") == 0) {
  
    // select synthesizer (mt32-pi)
    int16_t synth = sysex_param != NULL ? atoi(sysex_param) : -1;
    if (synth >= 0 && synth <= 1) {
      uint8_t sysex_mes[] = { 0xf0, 0x7d, 0x03, synth, 0xf7 };
      send_exclusive_message(midi_if, sysex_mes, sizeof(sysex_mes));
      printf("sent synth select command. (%d)\n", synth);
    } else {
      printf("error: synth must be 0(MT-32) or 1(SoundFont).\n");
      rc = -1;
    }

  } else if (stricmp(sysex_command, "sfont") == 0) {
  
    // select sound font (mt32-pi)
    int16_t font_index = sysex_param != NULL ? atoi(sysex_param) : -1;
    if (font_index >= 0 && font_index <= 127) {
      uint8_t sysex_mes[] = { 0xf0, 0x7d, 0x02, font_index & 0xff, 0xf7 };
      send_exclusive_message(midi_if, sysex_mes, sizeof(sysex_mes));
      printf("sent sound font select command. (%d)\n", font_index);
    } else {
      printf("error: font index must be 0 to 127.\n");
      rc = -1;
    }

  } else if (stricmp(sysex_command, "reboot") == 0) {

    // reboot (mt32-pi)
    uint8_t sysex_mes[] = { 0xf0, 0x7d, 0x00, 0xf7 };
    send_exclusive_message(midi_if, sysex_mes, sizeof(sysex_mes));

  } else {

    // unknown
    printf("error: unknown command.\n");
    show_help_message();
    rc = -1;

  }

  // resume
  if (paused == 1) {
    zmusic_resume();
  } else if (paused == 2) {
    rcd_resume();
  }

exit:
  return rc;
}
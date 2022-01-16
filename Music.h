#ifndef MUSIC_H
#define MUSIC_H
#include "Tone.h"

// Spiderman Theme Tune
const int SpiderMan_note_array[] = {
    NOTE_D4, NOTE_F4, NOTE_A4, 0, NOTE_GS4, 
    NOTE_F4, NOTE_D4, 0, NOTE_D4, NOTE_F4, 
    NOTE_A4, NOTE_A4, NOTE_GS4, NOTE_F4, 
    NOTE_D4,0, NOTE_G4, NOTE_AS4, NOTE_D5, 
    0, NOTE_C5, NOTE_AS4, NOTE_G4, 0, NOTE_D4, 
    NOTE_F4, NOTE_A4, 0, NOTE_GS4, NOTE_F4, 
    NOTE_D4, NOTE_AS4, NOTE_G4, NOTE_G4, 
    NOTE_F4, NOTE_G4, NOTE_F4, NOTE_D4
};

// 8 corresponds to 1/8
const int SpiderMan_duration_array[] = {
    4, 8, 4, 4, 4, 8, 4, 4, 4, 8, 4, 4, 
    4, 8, 4, 4, 4, 8, 4, 4, 4, 8, 4, 4, 
    4, 8, 4, 4, 4, 8, 4, 8, 2, 4, 6, 4, 
    6, 2
};

// Ironman Theme Tune
const int IronMan_note_array[] = {
    NOTE_B3, NOTE_D4, NOTE_D4, NOTE_E4, NOTE_E4, 
    NOTE_G4, NOTE_FS4, NOTE_G4, NOTE_FS4, NOTE_G4, 
    NOTE_D4, NOTE_D4, NOTE_E4, NOTE_E4, NOTE_B5, 
    NOTE_D5, NOTE_D5, NOTE_E5, NOTE_E5, NOTE_G5, 
    NOTE_FS5, NOTE_G5, NOTE_FS5, NOTE_G5, NOTE_D5, 
    NOTE_D5, NOTE_E5, NOTE_E5
};

const int IronMan_duration_array[] = {
    4, 4, 8, 8, 4, 16, 16, 16, 16, 8, 8,
    8, 8, 4, 4, 4, 8, 8, 4, 16, 16, 16,
    16, 8, 8, 8, 8, 4
};

// coffin dance theme song
const int coffin_note_array[] = {
  NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_AS4,
  NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_AS4,
  NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_AS4,
  NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_AS4,
  NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_AS4,
  NOTE_D5, NOTE_D5, NOTE_D5, NOTE_D5,
  NOTE_C5, NOTE_C5, NOTE_C5, NOTE_C5,
  NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5,
  NOTE_G5, NOTE_G5, NOTE_G5, NOTE_G5,
  NOTE_G5, NOTE_G5, NOTE_G5, NOTE_G5,
  NOTE_G5, NOTE_G5, NOTE_G5, NOTE_G5,
  NOTE_C5, NOTE_AS4, NOTE_A4, NOTE_F4,
  NOTE_G4, 0, NOTE_G4, NOTE_D5,
  NOTE_C5, 0, NOTE_AS4, 0,
  NOTE_A4, 0, NOTE_A4, NOTE_A4,
  NOTE_C5, 0, NOTE_AS4, NOTE_A4,
  NOTE_G4, 0, NOTE_G4, NOTE_AS5,
  NOTE_A5, NOTE_AS5, NOTE_A5, NOTE_AS5,
  NOTE_G4, 0, NOTE_G4, NOTE_AS5,
  NOTE_A5, NOTE_AS5, NOTE_A5, NOTE_AS5,
  NOTE_G4, 0, NOTE_G4, NOTE_D5,
  NOTE_C5, 0, NOTE_AS4, 0,
  NOTE_A4, 0, NOTE_A4, NOTE_A4,
  NOTE_C5, 0, NOTE_AS4, NOTE_A4,
  NOTE_G4, 0, NOTE_G4, NOTE_AS5,
  NOTE_A5, NOTE_AS5, NOTE_A5, NOTE_AS5,
  NOTE_G4, 0, NOTE_G4, NOTE_AS5,
  NOTE_A5, NOTE_AS5, NOTE_A5, NOTE_AS5
};

const int coffin_duration_array[] = {
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
};

#endif
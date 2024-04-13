#include "Audio.h"

void Audio::Setup() {
  InitAudioDevice();
  music = LoadSound("data/music.wav");
  SetSoundVolume(music, 0.2);
  PlaySound(music);
}

void Audio::TearDown() {
  UnloadSound(music);
  CloseAudioDevice();
}

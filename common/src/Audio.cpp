#include "Audio.h"

void Audio::Setup() {
  InitAudioDevice();
  music = LoadSound("data/music.wav");
  SetSoundVolume(music, 0.2);
}

void Audio::TearDown() {
  UnloadSound(music);
  CloseAudioDevice();
}

#include "Audio.h"

void Audio::Setup() {
  InitAudioDevice();
  music = LoadMusicStream("data/music.wav");
  music.looping = true;
  SetMusicVolume(music, 0.2f);

  PlayMusicStream(music);
}

void Audio::Update()
{ UpdateMusicStream(music); }

void Audio::TearDown() {
  UnloadMusicStream(music);
  CloseAudioDevice();
}

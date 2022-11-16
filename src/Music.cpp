#include "GentooEngine.h"

Music::Music () {
    music = nullptr;
}

Music::Music (std::string file) {
    music = nullptr;
    Open(file);
}

Music::~Music () {}

void Music::Open (std::string file) {
    music = Resources::GetMusic(file);
}

void Music::Play (int times) {
    if (Mix_PlayMusic(music.get(), times) == MUSIC_ERROR_PLAY)
        SDL_Log("Mix_PlayMusic: %s", SDL_GetError());
}

void Music::Play (std::string file, int times) {
    Open(file);
    Play(times);
}

void Music::Stop (int msToStop) {
    if (Mix_FadeOutMusic(msToStop) == MUSIC_ERROR_FADE)
        SDL_Log("Mix_PlayMusic: %s", SDL_GetError());
}

bool Music::IsOpen () {
    return (music != nullptr);
}
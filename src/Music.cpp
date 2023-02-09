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

void Music::Play (int times, int fadeIn) {
    if (Mix_FadeInMusic(music.get(), times, fadeIn) == MUSIC_ERROR_PLAY)
        SDL_Log("Mix_PlayMusic: %s", SDL_GetError());
}

void Music::Play (std::string file, int times, int fadeIn) {
    Open(file);
    Play(times, fadeIn);
}

void Music::Stop (int fadeOut) {
    if (Mix_FadeOutMusic(fadeOut) == MUSIC_ERROR_FADE)
        SDL_Log("Mix_PlayMusic: %s", SDL_GetError());
}

bool Music::IsOpen () {
    return (music != nullptr);
}
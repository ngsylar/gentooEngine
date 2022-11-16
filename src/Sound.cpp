#include "GentooEngine.h"

Sound::Sound (GameObject& associated): Component(associated) {
    selfDestruction = false;
    chunk = nullptr;
}

Sound::Sound (GameObject& associated, std::string file): Sound(associated) {
    Open(file);
}

Sound::~Sound () {}

void Sound::Open (std::string file) {
    chunk = Resources::GetSound(file);
    channel = SOUND_AUTO_CHANNEL;
}

bool Sound::IsOpen () {
    return (chunk != nullptr);
}

void Sound::Play (int times, bool destructAfterPlay) {
    int loops;

    loops = times - ((times > 0)? 1:0);
    selfDestruction = destructAfterPlay;
    channel = Mix_PlayChannel(channel, chunk.get(), loops);
    if (channel == SOUND_ERROR_PLAY) {
        SDL_Log("Mix_PlayChannel: %s", SDL_GetError());
    }
}

bool Sound::Playing () {
    return Mix_Playing(channel);
}

void Sound::Stop () {
    if (chunk and Mix_Playing(channel)) {
        Mix_HaltChannel(channel);
    }
}

void Sound::Update (float dt) {
    if ((not Mix_Playing(channel)) and selfDestruction)
        associated.RequestDelete();
}

void Sound::Render () {}

bool Sound::Is (std::string type) {
    return (type == "Sound");
}
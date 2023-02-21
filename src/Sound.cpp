#include "GentooEngine.h"

Sound::Sound (GameObject& associated): Component(associated) {
    selfDestruction = false;
    chunk = nullptr;
    audioPan = false;
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
    
    if(Mix_Playing(channel)) {
        if(audioPan) {
            float SoundPos = associated.box.GetGlobalCenter().x - Camera::GetPosition().x; //Game::GetInstance().GetState().Cam.Position.x;
            int Location = (SoundPos/Vec2(GAME_RESOLUTION).x * 400.f);
            (Location < 0 ? Location = 0 : (Location > 400 ? Location = 400 : Location));
            Location-=200;
            int R = 255 + Location;
            R>255?R=255:R;
            int L = 255 - Location;            
            L>255?L=255:L;
            Mix_SetPanning(channel, L, R);
        }
    } else {
        if(selfDestruction)
            associated.RequestDelete();
    }
}

void Sound::Render () {}

bool Sound::Is (std::string type) {
    return (type == "Sound");
}
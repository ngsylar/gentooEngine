#ifndef MUSIC_H
#define MUSIC_H

#include <string>

#define INCLUDE_SDL_MIXER
#include "SDL_include.h"

#define MUSIC_REPEAT_ON     -1
#define MUSIC_FADE_TIME     1500
#define MUSIC_ERROR_PLAY    -1
#define MUSIC_ERROR_FADE    0

class Music {
    private:
        std::shared_ptr<Mix_Music> music;

    public:
        Music();
        Music(std::string file);
        ~Music();
        void Open(std::string file);
        void Play(int times=0);
        void Play(std::string file, int times=0);
        void Stop(int msToStop=0);
        bool IsOpen();
};

#endif
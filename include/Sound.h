#ifndef SOUND_H
#define SOUND_H

#define INCLUDE_SDL_MIXER
#include "SDL_include.h"

#include "Component.h"

#define SOUND_ERROR_PLAY    -1
#define SOUND_AUTO_CHANNEL  -1

class Sound: public Component {
    private:
        std::shared_ptr<Mix_Chunk> chunk;
        int channel;
        bool selfDestruction;
        
    public:
        Sound(GameObject& associated);
        Sound(GameObject& associated, std::string file);
        ~Sound();
        void Stop();
        void Open(std::string file);
        bool IsOpen();
        void Play(int times=1, bool selfDestruction=false);
        bool Playing();
        void Update(float dt);
        void Render();
        bool Is(std::string type);
};

#endif
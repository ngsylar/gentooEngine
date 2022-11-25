#ifndef SPRITE_H
#define SPRITE_H

#define INCLUDE_SDL
#include "SDL_include.h"

#include "Component.h"
#include "Timer.h"

#define SPRITE_ERROR            -1
#define SPRITE_CLIP_START_POINT 0, 0
#define SPRITE_DEFAULT_SCALE    1.0f, 1.0f

class Sprite: public Component {
    private:
        std::shared_ptr<SDL_Texture> texture;
        SDL_Rect clipRect;
        int width, height;
        Vec2 scale;
        Timer frameTimer;
        int frameWidth, frameCount, currentFrame;
        bool frameOneshot, selfDestruction;
        std::vector<Vec2> positionArray;

    public:
        float parallaxFactor;
        
        Sprite(GameObject& associated);
        Sprite(
            GameObject& associated,
            std::string file,
            int frameCount=1,
            float frameTime=0.0f,
            bool frameOneshot=false,
            bool selfDestruction=false
        );
        ~Sprite();
        void Open(
            std::string file,
            int frameCount=1,
            float frameTime=0.0f,
            bool frameOneshot=false,
            bool selfDestruction=false
        );
        void SetClip(int x, int y, int w, int h);
        void SetScale(float scaleX, float scaleY);
        void SetScale(float scale);
        Vec2 GetScale();
        int GetWidth();
        int GetHeight();
        void SetFrame(int frame);
        void SetFrameTime(float frameTime);
        void SetFrameCount(int frameCount);
        void AddPosition(Vec2 position);
        void Update(float dt);
        void Render();
        void Render(int startX, int startY);
        void RenderWithNoOffset(int startX, int startY);
        bool IsOpen();
        bool Is(std::string type);
};

#endif
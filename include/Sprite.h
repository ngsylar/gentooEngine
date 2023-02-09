#ifndef SPRITE_H
#define SPRITE_H

#define INCLUDE_SDL
#include "SDL_include.h"

#include "Component.h"
#include "Timer.h"

#define SPRITE_ERROR                    -1
#define SPRITE_CLIP_START_POINT         0, 0
#define SPRITE_DEFAULT_SCALE            1.0f, 1.0f
#define SPRITE_ONESHOT_TRUE             true
#define SPRITE_ONESHOT_FALSE            false
#define SPRITE_SELFDESTRUCTION_TRUE     true
#define SPRITE_SELFDESTRUCTION_FALSE    false

class Sprite: public Component {
    private:
        std::shared_ptr<SDL_Texture> texture;
        SDL_Rect clipRect;
        int width, height;
        Vec2 scale;
        Timer frameTimer;
        int frameWidth, frameCount, currentFrame;
        bool oneshot, oneshotIsOver, selfDestruction;

    public:
        enum Axis {HORIZONTAL, VERTICAL};
        SDL_RendererFlip textureFlip;
        std::vector<Vec2> positionArray;
        Vec2 parallaxFactor;

        Sprite(GameObject& associated);
        Sprite(
            GameObject& associated,
            std::string file,
            int frameCount=1,
            float frameTime=0.0f,
            bool oneshot=false,
            bool selfDestruction=false
        );
        ~Sprite();
        void Open(
            std::string file,
            int frameCount=1,
            float frameTime=0.0f,
            bool oneshot=false,
            bool selfDestruction=false
        );
        void SetClip(int x, int y, int w, int h);
        void SetScale(float scaleX, float scaleY);
        void SetScale(float scale);
        void SetBlendMode(SDL_BlendMode blendMode);
        void SetFrame(int frame);
        void SetFrameTime(float frameTime);
        void SetFrameCount(int frameCount);
        void SetOneshot(bool oneshot);
        void SetSelfDestruction(bool selfDestruction);
        Vec2 GetScale();
        int GetWidth();
        int GetHeight();
        bool OneshotIsOver();
        void Flip(Axis axis);
        void Update(float dt);
        void Render();
        void Render(int startX, int startY);
        void RenderWithoutOffset(int startX, int startY);
        bool Is(std::string type);
        bool Is(ComponentType type);
};

#endif
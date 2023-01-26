#ifndef SPRITE_H
#define SPRITE_H

#define INCLUDE_SDL
#include "SDL_include.h"

#include "Component.h"
#include "Timer.h"

#define SPRITE_ERROR                -1
#define SPRITE_CLIP_START_POINT     0, 0
#define SPRITE_DEFAULT_SCALE        1.0f, 1.0f

class Sprite: public Component {
    private:
        struct TextureStruct {
            std::shared_ptr<SDL_Texture> texture_sptr;
            int textureFrameCount;
            float textureFrameTime;
            int textureWidth, textureHeight;
            bool textureOneshot, textureDestruction;
        };
        std::vector<TextureStruct> textureArray;
        std::weak_ptr<SDL_Texture> texture;
        SDL_Rect clipRect;
        int width, height;
        Vec2 scale;
        Timer frameTimer;
        int frameWidth, frameCount, currentFrame;
        bool frameOneshot, selfDestruction;
        int activeTexture;

    public:
        enum TextureFlipper {HORIZONTAL, VERTICAL};
        SDL_RendererFlip textureFlip;
        std::vector<Vec2> positionArray;
        Vec2 parallaxFactor;

        Sprite(GameObject& associated);
        ~Sprite();
        Sprite(
            GameObject& associated,
            std::string file,
            int frameCount=1,
            float frameTime=0.0f,
            bool frameOneshot=false,
            bool selfDestruction=false
        );
        void AddTexture(
            std::string file,
            int frameCount=1,
            float frameTime=0.0f,
            bool frameOneshot=false,
            bool selfDestruction=false
        );
        void SetTexture(int textureId);
        void FlipTexture(TextureFlipper axis);
        int GetActiveTexture();
        void SetClip(int x, int y, int w, int h);
        void SetScale(float scaleX, float scaleY);
        void SetScale(float scale);
        Vec2 GetScale();
        int GetWidth();
        int GetHeight();
        void SetFrame(int frame);
        void SetFrameTime(float frameTime);
        void SetFrameCount(int frameCount);
        void Update(float dt);
        void Render();
        void Render(int startX, int startY);
        void RenderWithNoOffset(int startX, int startY);
        bool HasTexture();
        bool Is(std::string type);
        bool Is(ComponentType type);
};

#endif
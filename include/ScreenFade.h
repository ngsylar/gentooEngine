#ifndef FOG_SCRFADE
#define FOG_SCRFADE

#define INCLUDE_SDL
#include "SDL_include.h"

#include "Color.h"
#include "Component.h"

class ScreenFade: public Component {
    private:
        ScreenFilter _FadeFilter;
        bool _FadeIn;
        bool _Finished;
        float _StartPercent;
        float _FinishPercent;
        float _CurrPercent;
        float _Step;
        float _Time;

    public:
        Color filterColor;
        ScreenFade(GameObject& associated, Color filterColor, float start, float finish, float time);
        ~ScreenFade();
        void RedirectFade(float finish);
        void SetTime(float time);
        bool Is(std::string type);
        bool Is(ComponentType type);
        void Render();
        void Update(float dt);
};

#endif// FOG_SCRFADE
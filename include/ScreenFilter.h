#ifndef FOG_SCRFILTER
#define FOG_SCRFILTER

#define INCLUDE_SDL
#include "SDL_include.h"

#include "Color.h"
#include "Component.h"

class ScreenFilter: public Component {
    private:
        SDL_Surface* _FilterSurface;

    public:
        Color filterColor;
        ScreenFilter(GameObject& associated, Color filterColor);
        ~ScreenFilter();
        bool Is(std::string type);
        bool Is(ComponentType type);
        void Render();
        void Update(float dt);
};

#endif// FOG_SCRFILTER
#ifndef TEXT_H
#define TEXT_H

#define INCLUDE_SDL
#define INCLUDE_SDL_TTF
#include "SDL_include.h"

#include "Component.h"

#define TEXT_ERROR          -1
#define TEXT_COLOR_BLACK    {0x00, 0x00, 0x00, 0xFF}
#define TEXT_COLOR_WHITE    {0xFF, 0xFF, 0xFF, 0xFF}
#define TEXT_COLOR_YELLOW   {0xF9, 0xB6, 0x3D, 0xFF}
#define TEXT_COLOR_SHADOW   {0x00, 0x00, 0x00, 0xBE}
#define TEXT_COLOR_HIDDEN   {0x00, 0x00, 0x00, 0x00}

class Text: public Component {
    public:
        enum Style {SOLID, SHADED, BLENDED};
    
        Text(
            GameObject& associated,
            std::string text,
            std::string fontFileName,
            int fontSize=24,
            Style style=BLENDED,
            SDL_Color color=TEXT_COLOR_WHITE,
            float displayTime=0.0f,
            float concealTime=0.0f
        );
        ~Text();
        void SetFontFile(std::string fontFileName);
        void SetFontSize(int fontSize);
        void SetStyle(Style style);
        void SetColor(SDL_Color color);
        void SetText(std::string text);
        void SetFlickTime(float displayTime, float concealTime);
        void Start();
        void Update(float dt);
        void Render();
        bool Is(std::string type);

    private:
        std::shared_ptr<TTF_Font> font;
        SDL_Texture* texture;
        std::string fontFileName;
        int fontSize;
        Style style;
        SDL_Color color;
        std::string text;
        Timer flickTimer;
        float displayTime;
        float concealTime;
        bool flicker;

        void RemakeTexture();
};

#endif
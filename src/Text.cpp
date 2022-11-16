#include "GentooEngine.h"

Text::Text (
    GameObject& associated, std::string text,
    std::string fontFile, int fontSize, Style style, SDL_Color color,
    float displayTime, float concealTime
): Component(associated) {
    texture = nullptr;
    font = nullptr;

    fontFileName = fontFile;
    this->fontSize = fontSize;
    this->style = style;
    this->color = color;

    flickTimer.SetResetTime(displayTime);
    this->displayTime = displayTime;
    this->concealTime = concealTime;
    flicker = true;

    this->text = text;
    RemakeTexture();
}

Text::~Text () {
    SDL_DestroyTexture(texture);
}

void Text::SetFontFile (std::string fontFileName) {
    this->fontFileName = fontFileName;
    RemakeTexture();
}

void Text::SetFontSize (int fontSize) {
    this->fontSize = fontSize;
    RemakeTexture();
}

void Text::SetStyle (Style style) {
    this->style = style;
    RemakeTexture();
}

void Text::SetColor (SDL_Color color) {
    this->color = color;
    RemakeTexture();
}

void Text::SetText (std::string text) {
    this->text = text;
    RemakeTexture();
}

void Text::RemakeTexture () {
    SDL_DestroyTexture(texture);
    SDL_Surface* surface;

    font = Resources::GetFont(fontFileName, fontSize);
    if (font == nullptr) {
        return;
    }
    switch (style) {
        case SOLID:
            surface = TTF_RenderText_Solid(font.get(), text.c_str(), color);
            break;
        case SHADED:
            surface = TTF_RenderText_Shaded(font.get(), text.c_str(), color, TEXT_COLOR_BLACK);
            break;
        case BLENDED:
            surface = TTF_RenderText_Blended(font.get(), text.c_str(), color);
            break;
        default: break;
    }
    if (surface == nullptr) {
        SDL_Log("TTF_RenderText: %s", SDL_GetError());
        return;
    }
    texture = SDL_CreateTextureFromSurface(
        Game::GetInstance().GetRenderer(), surface
    );
    if (texture == nullptr) {
        SDL_Log("SDL_CreateTextureFromSurface: %s", SDL_GetError());
        return;
    }

    associated.box.SetSize(surface->w, surface->h);
    SDL_FreeSurface(surface);
}

void Text::Start () {
    if (flickTimer.HasResetTime())
        SDL_SetTextureAlphaMod(texture, 0x00);
}

void Text::Update (float dt) {
    if (not (texture and flickTimer.HasResetTime()))
        return;
    
    flickTimer.Update(dt);
    if (flickTimer.IsOver()) {
        flicker = not flicker;
        SDL_SetTextureAlphaMod(texture, (flicker? 0x00 : color.a));
        flickTimer.SetResetTime(flicker? concealTime : displayTime);
        flickTimer.Reset();
    }
}

void Text::Render () {
    if (texture == nullptr)
        return;

    int x = (int)associated.box.x - Camera::pos.x;
    int y = (int)associated.box.y - Camera::pos.y;
    int w = (int)associated.box.w;
    int h = (int)associated.box.h;

    SDL_Rect clipRect = SDL_Rect{0, 0, w, h};
    SDL_Rect destRect = SDL_Rect{x, y, w, h};

    int rendercpy = SDL_RenderCopyEx(
        Game::GetInstance().GetRenderer(),
        texture, &clipRect, &destRect,
        associated.angleDeg, nullptr,
        SDL_FLIP_NONE
    );
    if (rendercpy == TEXT_ERROR) {
        SDL_Log("SDL_RenderCopy: %s", SDL_GetError());
    }
}

bool Text::Is (std::string type) {
    return (type == "Text");
}
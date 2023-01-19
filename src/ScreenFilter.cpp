#include "GentooEngine.h"


ScreenFilter::ScreenFilter(GameObject& associated, Color filterColor)
: Component(associated) {
    type = ComponentType::_ScreenFilter;

    this->filterColor = filterColor;
    _FilterSurface = nullptr;
    Vec2 SurfRes(GAME_RESOLUTION);
    _FilterSurface = SDL_CreateRGBSurface(0, SurfRes.x, SurfRes.y, 32, 0,0,0,0);
}

ScreenFilter::~ScreenFilter() {
    if(_FilterSurface != nullptr)
    {
        SDL_FreeSurface(_FilterSurface);
        _FilterSurface = nullptr;
    }
}

bool ScreenFilter::Is(std::string type) {
    return ("ScreenFilter" == type);
}

bool ScreenFilter::Is(ComponentType type) {
    return (this->type & type);
}

void ScreenFilter::Render() {
    SDL_Texture* ToShow = SDL_CreateTextureFromSurface(Game::GetInstance().GetRenderer(), _FilterSurface);
    SDL_SetTextureBlendMode(ToShow, SDL_BLENDMODE_BLEND);
	SDL_RenderCopy(Game::GetInstance().GetRenderer(), ToShow, nullptr, nullptr);
	SDL_DestroyTexture(ToShow);
}

void ScreenFilter::Update(float dt) {
    SDL_SetSurfaceAlphaMod(_FilterSurface, 255-filterColor.A);
    SDL_FillRect(_FilterSurface, nullptr, filterColor.ColorUint32());
}
		
		
		

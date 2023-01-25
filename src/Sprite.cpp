#include "GentooEngine.h"

Sprite::Sprite (GameObject& associated): Component(associated) {
    activeTexture = 0;
    clipRect = {0, 0, 0, 0};
    width = 0;
    height = 0;
    scale = Vec2(1.0f, 1.0f);
    frameWidth = 0;
    frameCount = 0;
    currentFrame = 0;
    frameOneshot = false;
    selfDestruction = false;
    parallaxFactor = 1.0f;
    textureFlip = SDL_FLIP_NONE;
    type = ComponentType::_Sprite;
}

Sprite::~Sprite () {
    textureArray.clear();
}

Sprite::Sprite (
    GameObject& associated, std::string file,
    int frameCount, float frameTime, bool oneshot, bool destruct
): Sprite(associated) {
    AddTexture(file, frameCount, frameTime, oneshot, destruct);
}

void Sprite::AddTexture (
    std::string file,
    int frameCount, float frameTime, bool oneshot, bool destruct
) {
    texture = Resources::GetImage(file);
    scale = Vec2(SPRITE_DEFAULT_SCALE);

    int qtexture = SDL_QueryTexture(
        texture.lock().get(), nullptr, nullptr,
        &width, &height
    );
    if (qtexture == SPRITE_ERROR) {
        SDL_Log("SDL_QueryTexture: %s", SDL_GetError());
    }

    TextureStruct textureStruct;
    textureStruct.textureFrameCount = frameCount;
    textureStruct.textureFrameTime = frameTime;
    textureStruct.textureOneshot = oneshot;
    textureStruct.textureDestruction = destruct;
    textureStruct.textureWidth = width;
    textureStruct.textureHeight = height;

    textureArray.push_back(textureStruct);
    textureArray.back().texture_sptr = texture.lock();
    SetTexture(textureArray.size()-1);
}

void Sprite::SetTexture (int textureId) {
    int i = textureId % textureArray.size();
    if (i < 0) i += textureArray.size();
    texture = textureArray[i].texture_sptr;
    activeTexture = i;

    width = textureArray[i].textureWidth;
    height = textureArray[i].textureHeight;

    frameCount = textureArray[i].textureFrameCount;
    frameTimer = Timer(textureArray[i].textureFrameTime);
    frameOneshot = textureArray[i].textureOneshot;
    selfDestruction = textureArray[i].textureDestruction;

    frameWidth = width / frameCount;
    SetClip(SPRITE_CLIP_START_POINT, frameWidth, height);
    associated.box.SetSize((float)frameWidth, (float)height);
    currentFrame = 0;
}

void Sprite::FlipTexture (TextureFlipper axis) {
    SDL_RendererFlip flip = (axis == HORIZONTAL) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_VERTICAL;
    textureFlip = SDL_RendererFlip(textureFlip ^ flip);
}

int Sprite::GetActiveTexture () {
    return activeTexture;
}

void Sprite::SetClip (int x, int y, int w, int h) {
    clipRect = SDL_Rect{x, y, w, h};
}

void Sprite::SetScale (float scaleX, float scaleY) {
    Vec2 size = Vec2((float)frameWidth, (float)height);
    Vec2 position = associated.box.GetPosition();
    scale = Vec2(scaleX, scaleY);
    
    associated.box.SetSize(size.x*scale.x, size.y*scale.y);
    associated.box.SetPosition(position);
}

void Sprite::SetScale (float scale) {
    SetScale(scale, scale);
}

void Sprite::SetBlendMode (SDL_BlendMode blendMode) {
    SDL_SetTextureBlendMode(textureArray[activeTexture].texture_sptr.get(),blendMode);
}

Vec2 Sprite::GetScale () {
    return scale;
}

int Sprite::GetWidth () {
    return (frameWidth * (int)scale.x);
}

int Sprite::GetHeight () {
    return (height * (int)scale.y);
}

void Sprite::SetFrame (int frame) {
    currentFrame = frame % frameCount;
    clipRect.x = currentFrame * frameWidth;
}

void Sprite::SetFrameTime (float frameTime) {
    frameTimer = Timer(frameTime, frameTimer.GetTime());
}

void Sprite::SetFrameCount (int frameCount) {
    this->frameCount = frameCount;
    frameWidth = width / frameCount;
    currentFrame = 0;
    
    clipRect.x = currentFrame * frameWidth;
    clipRect.w = frameWidth;

    associated.box.w = (float)frameWidth * scale.x;
}

void Sprite::Update (float dt) {
    if (frameTimer.HasResetTime()) {
        frameTimer.Update(dt);

        if (frameTimer.IsOver()) {
            SetFrame(currentFrame+1);
            frameTimer.Reset();
            
            if ((currentFrame == frameCount-1) and frameOneshot) {
                frameTimer.SetResetTime(0.0f);

                if (selfDestruction)
                    associated.RequestDelete();
            }
        }
    }
}

void Sprite::Render () {
    if (positionArray.empty())
        Render(
            (int)associated.box.x - Camera::pos.x,
            (int)associated.box.y - Camera::pos.y
        );
    else for (int i=0; i < (int)positionArray.size(); i++)
        Render(
            (int)positionArray[i].x - (parallaxFactor * Camera::pos.x),
            (int)positionArray[i].y - (parallaxFactor * Camera::pos.y)
        );
}

void Sprite::Render (int startX, int startY) {
    if (textureArray.empty())
        return;

    SDL_Rect destRect = SDL_Rect{
        startX, startY,
        (int)associated.box.w, (int)associated.box.h
    };
    SDL_Point boxCenter = SDL_Point{
        destRect.w/2 + (int)associated.box.offset.x,
        destRect.h/2 + (int)associated.box.offset.y
    };
    int rendercpy = SDL_RenderCopyEx(
        Game::GetInstance().GetRenderer(),
        texture.lock().get(), &clipRect, &destRect,
        associated.angleDeg, &boxCenter, textureFlip
    );
    if (rendercpy == SPRITE_ERROR) {
        SDL_Log("SDL_RenderCopy: %s", SDL_GetError());
    }
}

void Sprite::RenderWithNoOffset (int startX, int startY) {
    if (textureArray.empty())
        return;

    SDL_Rect destRect = SDL_Rect{
        startX, startY,
        (int)associated.box.w, (int)associated.box.h
    };
    int rendercpy = SDL_RenderCopyEx(
        Game::GetInstance().GetRenderer(),
        texture.lock().get(), &clipRect, &destRect,
        associated.angleDeg, nullptr, textureFlip
    );
    if (rendercpy == SPRITE_ERROR) {
        SDL_Log("SDL_RenderCopy: %s", SDL_GetError());
    }
}

bool Sprite::HasTexture () {
    return (not textureArray.empty());
}

bool Sprite::Is (std::string type) {
    return (type == "Sprite");
}

bool Sprite::Is (ComponentType type) {
    return (type & this->type);
}
#include "GentooEngine.h"

Sprite::Sprite (GameObject& associated): Component(associated) {
    clipRect = {0, 0, 0, 0};
    width = 0;
    height = 0;
    scale = Vec2(1.0f, 1.0f);
    frameWidth = 0;
    frameCount = 0;
    currentFrame = 0;
    oneshot = false;
    oneshotIsOver = false;
    selfDestruction = false;
    parallaxFactor = Vec2(1.0f, 1.0f);
    textureFlip = SDL_FLIP_NONE;
    type = ComponentType::_Sprite;
}

Sprite::~Sprite () {}

Sprite::Sprite (
    GameObject& associated, std::string file,
    int frameCount, float frameTime, bool oneshot, bool destruct
): Sprite(associated) {
    Open(file, frameCount, frameTime, oneshot, destruct);
}

void Sprite::Open (
    std::string file,
    int frameCount, float frameTime, bool oneshot, bool destruct
) {
    texture = Resources::GetImage(file);
    scale = Vec2(SPRITE_DEFAULT_SCALE);

    int qtexture = SDL_QueryTexture(
        texture.get(), nullptr, nullptr,
        &width, &height
    );
    if (qtexture == SPRITE_ERROR) {
        SDL_Log("SDL_QueryTexture: %s", SDL_GetError());
    }

    this->frameCount = frameCount;
    frameTimer = Timer(frameTime);
    this->oneshot = oneshot;
    selfDestruction = destruct;

    frameWidth = width / frameCount;
    SetClip(SPRITE_CLIP_START_POINT, frameWidth, height);
    associated.box.SetSize((float)frameWidth, (float)height);
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
    SDL_SetTextureBlendMode(texture.get(), blendMode);
}

void Sprite::SetFrame (int frame) {
    currentFrame = frame % frameCount;
    clipRect.x = currentFrame * frameWidth;
    oneshotIsOver = false;
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

void Sprite::SetOneshot (bool oneshot) {
    this->oneshot = oneshot;
}

void Sprite::SetSelfDestruction (bool destruct) {
    selfDestruction = destruct;
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

bool Sprite::OneshotIsOver () {
    return oneshotIsOver;
}

void Sprite::Flip (Axis axis) {
    SDL_RendererFlip flip = (axis == HORIZONTAL) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_VERTICAL;
    textureFlip = SDL_RendererFlip(textureFlip ^ flip);
}

void Sprite::Update (float dt) {
    if ((not oneshotIsOver) and frameTimer.HasResetTime()) {
        frameTimer.Update(dt);

        if (frameTimer.IsOver()) {
            if ((++currentFrame == frameCount) and oneshot) {
                oneshotIsOver = true;
                if (selfDestruction)
                    associated.RequestDelete();
                return;
            }
            SetFrame(currentFrame);
            frameTimer.Reset();
        }
    }
}

void Sprite::Render () {
    if (positionArray.empty())
        Render(
            (int)associated.box.x - (parallaxFactor.x * Camera::pos.x),
            (int)associated.box.y - (parallaxFactor.y * Camera::pos.y)
        );
    else for (int i=0; i < (int)positionArray.size(); i++)
        Render(
            (int)positionArray[i].x - (parallaxFactor.x * Camera::pos.x),
            (int)positionArray[i].y - (parallaxFactor.y * Camera::pos.y)
        );
}

void Sprite::Render (int startX, int startY) {
    if (texture == nullptr)
        return;

    SDL_Rect destRect = SDL_Rect{
        startX, startY,
        (int)associated.box.w, (int)associated.box.h
    };
    SDL_Point boxCenter = SDL_Point{
        (destRect.w >> 1) + (int)associated.box.offset.x,
        (destRect.h >> 1) + (int)associated.box.offset.y
    };
    if (associated.label == "Player")
        SDL_SetTextureColorMod(texture.get(), 0, 0, 0);
    int rendercpy = SDL_RenderCopyEx(
        Game::GetInstance().GetRenderer(),
        texture.get(), &clipRect, &destRect,
        associated.angleDeg, &boxCenter, textureFlip
    );
    if (rendercpy == SPRITE_ERROR) {
        SDL_Log("SDL_RenderCopy: %s", SDL_GetError());
    }
}

void Sprite::RenderWithoutOffset (int startX, int startY) {
    if (texture == nullptr)
        return;

    SDL_Rect destRect = SDL_Rect{
        startX, startY,
        (int)associated.box.w, (int)associated.box.h
    };
    int rendercpy = SDL_RenderCopyEx(
        Game::GetInstance().GetRenderer(),
        texture.get(), &clipRect, &destRect,
        associated.angleDeg, nullptr, textureFlip
    );
    if (rendercpy == SPRITE_ERROR) {
        SDL_Log("SDL_RenderCopy: %s", SDL_GetError());
    }
}

bool Sprite::Is (std::string type) {
    return (type == "Sprite");
}

bool Sprite::Is (ComponentType type) {
    return (type & this->type);
}
#include "GentooEngine.h"
#include "TestObjects.h"

// editar: deixar halfSize proporcional a layerCount
LoopedBackground::LoopedBackground (
    GameObject& associated, std::string fileName, int layerCount
): Component(associated) {
    sprite = new Sprite(associated, fileName);
    renderingCount = (layerCount * 2) - 1;
    middlePositionId = renderingCount >> 1;
    halfSize = ((layerCount - 1) * (int)std::min(associated.box.w, associated.box.h)) >> 1;
}

LoopedBackground::~LoopedBackground () {
    delete(sprite);
}

// editar: deixar halfSize proporcional a layerCount
void LoopedBackground::SetLayerCount (int layerCount) {
    renderingCount = (layerCount * 2) - 1;
    middlePositionId = renderingCount >> 1;
    halfSize = ((layerCount - 1) * (int)std::min(associated.box.w, associated.box.h)) >> 1;
}

void LoopedBackground::Start () {
    associated.box.SetPosition(Camera::GetPosition());
    SDL_Log("%d", halfSize);
}

// editar: respeitar distancia do centro do objeto para o centro da camera
void LoopedBackground::LateUpdate (float dt) {
    Vec2 cameraDistance = Camera::GetPosition() - associated.box.GetPosition();
    if (cameraDistance.Magnitude() >= halfSize)
        associated.box.SetPosition(Camera::GetPosition());
}

void LoopedBackground::Render () {
    for (int r=-middlePositionId; r <= middlePositionId; r++)
        for (int c=-middlePositionId; c <= middlePositionId; c++)
            sprite->Render(
                associated.box.x + (c * associated.box.w) - Camera::pos.x,
                associated.box.y + (r * associated.box.h) - Camera::pos.y
            );
}
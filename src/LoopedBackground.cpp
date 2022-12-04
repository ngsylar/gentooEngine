#include "GentooEngine.h"

LoopedBackground::LoopedBackground (
    GameObject& associated, std::string fileName, int layerCount, float parallaxFactor
): Component(associated) {
    sprite = new Sprite(associated, fileName);
    renderingCount = (layerCount * 2) - 1;
    middlePositionId = renderingCount >> 1;
    halfSizes[HORIZONTAL] = ((layerCount - 1) * (int)associated.box.w) >> 1;
    halfSizes[VERTICAL] = ((layerCount - 1) * (int)associated.box.h) >> 1;
    this->parallaxFactor = parallaxFactor;
}

LoopedBackground::~LoopedBackground () {
    delete(sprite);
}

void LoopedBackground::SetLayerCount (int layerCount) {
    renderingCount = (layerCount * 2) - 1;
    middlePositionId = renderingCount >> 1;
    halfSizes[HORIZONTAL] = ((layerCount - 1) * (int)associated.box.w) >> 1;
    halfSizes[VERTICAL] = ((layerCount - 1) * (int)associated.box.h) >> 1;
}

void LoopedBackground::Start () {
    associated.box.SetPosition(Camera::GetPosition());
}

void LoopedBackground::LateUpdate (float dt) {
    Vec2 position = associated.box.GetPosition();
    Vec2 cameraDistance = (Camera::GetPosition() * parallaxFactor) - position;

    if (fabs(cameraDistance.x) >= halfSizes[HORIZONTAL]) {
        int signX = (std::signbit(cameraDistance.x)? -1 : 1);
        associated.box.SetPosition(
            position.x + (signX * halfSizes[HORIZONTAL]) + cameraDistance.x, position.y
        );
    }
    if (fabs(cameraDistance.y) >= halfSizes[VERTICAL]) {
        int signY = (std::signbit(cameraDistance.y) ? -1 : 1);
        associated.box.SetPosition(
            position.x, position.y + (signY * halfSizes[VERTICAL]) + cameraDistance.y
        );
    }
}

void LoopedBackground::Render () {
    for (int r=-middlePositionId; r <= middlePositionId; r++)
        for (int c=-middlePositionId; c <= middlePositionId; c++)
            sprite->Render(
                associated.box.x + (c * associated.box.w) - (parallaxFactor * Camera::pos.x),
                associated.box.y + (r * associated.box.h) - (parallaxFactor * Camera::pos.y)
            );
}
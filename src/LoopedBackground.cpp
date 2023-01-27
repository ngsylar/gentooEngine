#include "GentooEngine.h"

LoopedBackground::LoopedBackground (
    GameObject& associated, std::string fileName, int layerCount, Vec2 parallaxFactor
): Component(associated) {
    sprite = new Sprite(associated, fileName);
    renderingCount = (layerCount * 2) - 1;
    middlePositionId = renderingCount >> 1;
    halfSizes[HORIZONTAL] = ((layerCount - 1) * (int)associated.box.w) >> 1;
    halfSizes[VERTICAL] = ((layerCount - 1) * (int)associated.box.h) >> 1;
    this->parallaxFactor = parallaxFactor;
    type = ComponentType::_LoopedBackground;
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

void LoopedBackground::SetPosition (float positionX, float positionY) {
    associated.box.SetPosition(positionX, positionY);
    Camera::AddMethod(this, std::bind(&LateStart, this));
}

void LoopedBackground::SetPosition (Vec2 position) {
    associated.box.SetPosition(position);
    Camera::AddMethod(this, std::bind(&LateStart, this));
}

void LoopedBackground::FixLeaks () {
    Vec2 cameraDistance = Camera::GetPosition() - associated.box.GetPosition();

    std::array<int, 2> sign = {
        (std::signbit(cameraDistance.x)? -1 : 1),
        (std::signbit(cameraDistance.x)? -1 : 1)
    };
    Vec2 rectsDistance(
        floor((cameraDistance.x / halfSizes[HORIZONTAL]) * sign[HORIZONTAL]),
        floor((cameraDistance.y / halfSizes[VERTICAL]) * sign[VERTICAL])
    );
    if ((int)rectsDistance.x % 2 != 0) rectsDistance.x++;
    if ((int)rectsDistance.y % 2 != 0) rectsDistance.y++;
    
    associated.box.x += rectsDistance.x * halfSizes[HORIZONTAL] * sign[HORIZONTAL];
    associated.box.y += rectsDistance.y * halfSizes[VERTICAL] * sign[VERTICAL];
}

void LoopedBackground::Start () {
    cameraInitialPosition = Camera::pos;
    cameraAdjustment = (cameraInitialPosition * parallaxFactor) - cameraInitialPosition;
    cameraInitialPosition += cameraAdjustment;
}

void* LoopedBackground::LateStart () {
    FixLeaks();
    Start();
    Camera::RemoveMethod(this);
    return nullptr;
}

void LoopedBackground::LateUpdate (float dt) {
    Vec2 cameraDistance = (Camera::pos * parallaxFactor) - cameraInitialPosition;

    if (fabs(cameraDistance.x) >= halfSizes[HORIZONTAL]) {
        int signX = (std::signbit(cameraDistance.x)? -1 : 1);
        float displacement = (signX * halfSizes[HORIZONTAL]) + cameraDistance.x;
        associated.box.x += displacement;
        cameraInitialPosition.x += displacement;
    }
    if (fabs(cameraDistance.y) >= halfSizes[VERTICAL]) {
        int signY = (std::signbit(cameraDistance.y) ? -1 : 1);
        float displacement = (signY * halfSizes[VERTICAL]) + cameraDistance.y;
        associated.box.y += displacement;
        cameraInitialPosition.y += displacement;
    }
}

void LoopedBackground::Render () {
    for (int r=-middlePositionId; r <= middlePositionId; r++)
        for (int c=-middlePositionId; c <= middlePositionId; c++) {
            Vec2 displacement = (parallaxFactor * Camera::pos) - cameraAdjustment;
            sprite->Render(
                associated.box.x + (c * associated.box.w) - (int)displacement.x,
                associated.box.y + (r * associated.box.h) - (int)displacement.y
            );
        }
}

bool LoopedBackground::Is (ComponentType type) {
    return (type & this->type);
}
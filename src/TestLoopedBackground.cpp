#include "GentooEngine.h"
#include "TestObjects.h"

LoopedBackground::LoopedBackground (
    GameObject& associated, std::string fileName, int layerCount
): Component(associated) {
    sprite = new Sprite(associated, fileName);
    halfSize = (int)std::min(associated.box.w, associated.box.h) >> 1;
    renderingCount = (layerCount * 2) - 1;
    middlePositionId = renderingCount >> 1;
}

LoopedBackground::~LoopedBackground () {
    delete(sprite);
}

void LoopedBackground::SetLayerCount (int layerCount) {
    renderingCount = (layerCount * 2) - 1;
    middlePositionId = renderingCount >> 1;
}

void LoopedBackground::Start () {
    associated.box.SetPosition(Camera::GetPosition());
}

void LoopedBackground::Update (float dt) {
    if (Camera::GetPosition().DistanceTo(Vec2(associated.box.x, associated.box.y)) >= halfSize)
        ; // editar: conferir a direcao do vetor resultante e mover o objeto do meio (respeitar distancia do centro do objeto para o centro da camera)
}

void LoopedBackground::Render () {
    for (int r=-middlePositionId; r <= middlePositionId; r++)
        for (int c=-middlePositionId; c <= middlePositionId; c++)
            sprite->Render(associated.box.x+(c*associated.box.w), associated.box.y-(r*associated.box.h));
}
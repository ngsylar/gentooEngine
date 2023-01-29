#include "GentooEngine.h"
#include "Attack.h"

Attack::Attack (GameObject& associated, GameObject* externalAssociated): Component(associated) {
    type = ComponentType::_Attack;

    if (externalAssociated != nullptr) {
        this->externalAssociated = Game::GetInstance().GetCurrentState().GetObjectPtr(externalAssociated);
        usingExternalAssociated = true;
        collider = nullptr;
        sprite = nullptr;
    } else {
        usingExternalAssociated = false;
        collider = (Collider*)associated.GetComponent(ComponentType::_Collider);
        sprite = nullptr;
    }
}

// sprite is only configurable if an external associated is used
void Attack::OpenSprite (
    std::string file, int frameCount, float frameTime, bool frameOneshot, bool selfDestruction
) {
    if (not usingExternalAssociated)
        return;

    if (sprite == nullptr)
        sprite = new Sprite(associated);
    sprite->Open(file, frameCount, frameTime, frameOneshot, selfDestruction);
}

// collider is only configurable if an external associated is used
void Attack::SetupCollider (Vec2 offset, Vec2 size) {
    if (not usingExternalAssociated)
        return;

    if (sprite == nullptr) {
        associated.box.x = offset.x;
        associated.box.y = offset.y;
        associated.box.w = size.x;
        associated.box.h = size.y;
    }
    if (collider == nullptr)
        collider = new Collider(associated);
    if (sprite != nullptr)
        collider->SetBox(offset, size);
}

void Attack::Update (float dt) {}

void Attack::NotifyCollision (GameObject& other) {}

bool Attack::UsingInternalAssociated () {
    return (not usingExternalAssociated);
}

bool Attack::UsingExternalAssociated () {
    return usingExternalAssociated;
}

bool Attack::Is (std::string type) {
    return (type == "Attack");
}

bool Attack::Is (ComponentType type) {
    return (type & this->type);
}
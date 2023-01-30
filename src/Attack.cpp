#include "GentooEngine.h"
#include "Attack.h"

Attack::Attack (GameObject& associated, GameObject* externalAssociated): Component(associated) {
    type = ComponentType::_Attack;
    enabled = true;

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

Attack::~Attack () {
    if (usingExternalAssociated) {
        delete(collider);
        delete(sprite);
    }
}

// sprite is only configurable if an external associated is used
void Attack::OpenSprite (
    std::string file, int frameCount, float frameTime, bool frameOneshot, bool selfDestruction
) {
    if (not usingExternalAssociated or (usingExternalAssociated and externalAssociated.expired()))
        return;

    if (sprite == nullptr)
        sprite = new Sprite(associated);
    sprite->Open(file, frameCount, frameTime, frameOneshot, selfDestruction);

    if (collider == nullptr)
        collider = new Collider(associated, COLLIDER_TRIGGER_TRUE);
    collider->Reset();
}

// collider is only configurable if an external associated is used
void Attack::SetupCollider (Vec2 offset, Vec2 size) {
    if (not usingExternalAssociated or (usingExternalAssociated and externalAssociated.expired()))
        return;

    if (sprite == nullptr) {
        associated.box.x = offset.x;
        associated.box.y = offset.y;
        associated.box.w = size.x;
        associated.box.h = size.y;
    }
    if (collider == nullptr)
        collider = new Collider(associated, COLLIDER_TRIGGER_TRUE);

    if (sprite != nullptr)
        collider->SetBox(offset, size);
}

void Attack::Awaken () {}

void Attack::Start () {}

void Attack::Update (float dt) {
    if (usingExternalAssociated and externalAssociated.expired())
        return;

    if (enabled)
        UpdateAttack(dt);
    else return;

    if (usingExternalAssociated) {
        sprite->Update(dt);
        collider->Update(dt);
    }

    if (lifetime.HasResetTime()) {
        lifetime.Update(dt);
        if (lifetime.IsOver()) {
            lifetime.Reset();
            enabled = false;
        }
    }
}

void Attack::UpdateAttack (float dt) {}

void Attack::Render () {
    if (enabled and usingExternalAssociated and (not externalAssociated.expired())) {
        sprite->Render();
        collider->Render();
    }
}

void Attack::NotifyCollision (GameObject& other) {
    if (not enabled)
        return;

    EntityMachine* entity = (EntityMachine*)other.GetComponent(ComponentType::_EntityMachine);
    if (entity != nullptr)
        entity->SetState(EntityState::Injured);
}

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
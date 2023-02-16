#include "GentooEngine.h"
#include "AttackGeneric.h"

AttackGeneric::AttackGeneric (GameObject& associated, GameObject* externalAssociated): Component(associated) {
    type = ComponentType::_Attack;
    force = Vec2(400.0f, 140.0f);
    impulse = 50.0f;
    damage = 1;

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
    ignoreEqualLabels = true;
}

// sprite is only configurable if an external associated is used
void AttackGeneric::OpenSprite (
    std::string file, int frameCount, float frameTime, bool frameOneshot, bool selfDestruction
) {
    if (not usingExternalAssociated or (usingExternalAssociated and externalAssociated.expired()))
        return;

    if (sprite == nullptr) {
        sprite = new Sprite(associated);
        associated.AddComponent(sprite);
    }
    sprite->Open(file, frameCount, frameTime, frameOneshot, selfDestruction);

    if (collider == nullptr) {
        collider = new Collider(associated, COLLIDER_TRIGGER_TRUE);
        associated.AddComponent(collider);
    }
    collider->Reset();
}

// collider is only configurable if an external associated is used
void AttackGeneric::SetupCollider (Vec2 offset, Vec2 size) {
    if (not usingExternalAssociated or (usingExternalAssociated and externalAssociated.expired()))
        return;

    if (sprite == nullptr) {
        associated.box.w = size.x;
        associated.box.h = size.y;
    }
    if (collider == nullptr) {
        collider = new Collider(associated, COLLIDER_TRIGGER_TRUE);
        associated.AddComponent(collider);
    }
    else collider->SetBox(offset, size);
}

void AttackGeneric::SetProperties (Vec2 force, float impulse, int damage, float displacement) {
    this->force = force;
    this->impulse = impulse;
    this->damage = damage;
    this->displacement = displacement;
}

void AttackGeneric::Awaken () {}

void AttackGeneric::Start () {}

void AttackGeneric::Perform () {
    associated.enabled = true;
    lifetime.Reset();
    PerformAttack();
}

void AttackGeneric::PerformAttack () {}

void AttackGeneric::Update (float dt) {
    if (usingExternalAssociated and externalAssociated.expired()) {
        associated.RequestDelete();
        return;
    }
    if (lifetime.HasResetTime()) {
        lifetime.Update(dt);
        if (lifetime.IsOver()) {
            associated.enabled = false;
            return;
        }
    }
    UpdateAttack(dt);

    // melius colliders' pixel correction
    associated.pixelColliderFix0 = (
        usingExternalAssociated and
        (not externalAssociated.expired()) and
        (externalAssociated.lock()->pixelColliderFix0 ^
        externalAssociated.lock()->pixelColliderFix1)
    );
}

void AttackGeneric::UpdateAttack (float dt) {}

void AttackGeneric::CancelAttack () {
    lifetime.Update(lifetime.GetResetTime());
    associated.enabled = false;
}

void AttackGeneric::NotifyCollision (GameObject& other) {
    if (((not externalAssociated.expired()) and (&other == externalAssociated.lock().get()))
    or (ignoreEqualLabels and (associated.label == other.label)))
        return;

    EntityMachine* entity = (EntityMachine*)other.GetComponent(ComponentType::_EntityMachine);
    if (entity == nullptr) return;

    // note: _ForceX, _ForceY, _Impulse, _Damage, _OriginX, _OriginY, _Displacement
    float argsv[7] = {force.x, force.y, impulse, (float)damage, collider->box.x, collider->box.y, 0.0f};
    entity->FormatState(EntityState::Injured, 7, argsv);
}

bool AttackGeneric::UsingInternalAssociated () {
    return (not usingExternalAssociated);
}

bool AttackGeneric::UsingExternalAssociated () {
    return usingExternalAssociated;
}

bool AttackGeneric::Is (std::string type) {
    return (type == "Attack");
}

bool AttackGeneric::Is (ComponentType type) {
    return (type & this->type);
}
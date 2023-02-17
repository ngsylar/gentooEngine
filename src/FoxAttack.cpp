#include "GentooEngine.h"
#include "FoxAttack.h"
#include "Boss.h"

#define SPRITE_ATTACK           "assets/img/fox/magic.png"
#define SPRITE_FRAMES           12, 0.1f
#define SPRITE_OFFSET_X         -6.0f
#define SPRITE_OFFSET_Y         -16.0f

#define CAMERA_SHAKE_COUNT      6
#define CAMERA_SHAKE_RANGE      3
#define CAMERA_SHAKE_RESET_TIME 0.04f

FoxAttack::FoxAttack (
    GameObject& associated, GameObject* externalAssociated
): AttackGeneric(associated, externalAssociated) {
    this->externalAssociated = Game::GetInstance().GetCurrentState().GetObjectPtr(externalAssociated);
    associated.label = externalAssociated->label;
    associated.enabled = false;

    force = Vec2(400.0f, 0.0f);
    impulse = 80.0f;
    damage = 3;
    displacement = 0.0f;

    isOver = false;
}

void FoxAttack::PushAttack (Vec2 offset, Vec2 size, float lifetimeStart, float lifetimeEnd) {
    AttackType attackType;
    attackType.offset_i = offset;
    attackType.size_i = size;
    attackType.lifetimeStart_i = lifetimeStart;
    attackType.lifetimeEnd_i = lifetimeEnd;
    attackTypes.push(attackType);
}

void FoxAttack::Start () {
    OpenSprite(SPRITE_ATTACK, SPRITE_FRAMES, true);

    cameraShakeTimer.SetResetTime(CAMERA_SHAKE_RESET_TIME);
    cameraShakeTimer.FalseStart();
}

void FoxAttack::Perform (AttackDirection direction) {
    PushAttack(Vec2(-41.0f, -23.0f), Vec2(49.0f, 50.0f), 0.3f, 0.4f);
    PushAttack(Vec2(-41.0f, -23.0f), Vec2(74.0f, 50.0f), 0.0f, 0.1f);
    PushAttack(Vec2(-41.0f, -16.0f), Vec2(89.0f, 43.0f), 0.0f, 0.1f);
    PushAttack(Vec2(-41.0f, -17.0f), Vec2(78.0f, 44.0f), 0.0f, 0.1f);
    PushAttack(Vec2(-41.0f, -17.0f), Vec2(78.0f, 44.0f), 0.0f, 0.1f);
    PushAttack(Vec2(-41.0f, -17.0f), Vec2(78.0f, 44.0f), 0.0f, 0.1f);
    PushAttack(Vec2(-41.0f, 3.0f), Vec2(81.0f, 26.0f), 0.0f, 0.1f);

    AttackType attackType = attackTypes.front();
    attackTypes.pop();

    Vec2 colliderOffset = attackType.offset_i + (attackType.size_i * 0.5f);
    if (direction == LEFT) colliderOffset.x *= -1.0f;
    SetupCollider(colliderOffset, attackType.size_i);

    this->lifetimeStart = attackType.lifetimeStart_i;
    lifetime.SetResetTime(attackType.lifetimeEnd_i);

    associated.enabled = true;
    this->direction = direction;
    sprite->SetFrame(0);
    lifetime.Reset();

    Rect externalBox = (Rect)(externalAssociated.lock()->box);

    switch (direction) {
        case LEFT:
            if (sprite != nullptr) sprite->textureFlip = SDL_FLIP_HORIZONTAL;
            associated.box.x = externalBox.x + externalBox.w - SPRITE_OFFSET_X - associated.box.w;
            associated.box.y = externalBox.y + SPRITE_OFFSET_Y;
            break;

        case RIGHT:
            if (sprite != nullptr) sprite->textureFlip = SDL_FLIP_NONE;
            associated.box.x = externalBox.x + SPRITE_OFFSET_X;
            associated.box.y = externalBox.y + SPRITE_OFFSET_Y;
            break;

        default: break;
    }

    isOver = false;
}

void FoxAttack::Update (float dt) {
    if (usingExternalAssociated and externalAssociated.expired()) {
        associated.RequestDelete();
        return;
    }
    if (lifetime.HasResetTime()) {
        lifetime.Update(dt);
        if (lifetime.IsOver()) {

            if (not attackTypes.empty()) {
                AttackType attackType = attackTypes.front();
                attackTypes.pop();

                Vec2 colliderOffset = attackType.offset_i + (attackType.size_i * 0.5f);
                if (direction == LEFT) colliderOffset.x *= -1.0f;
                SetupCollider(colliderOffset, attackType.size_i);

                this->lifetimeStart = attackType.lifetimeStart_i;
                lifetime.SetResetTime(attackType.lifetimeEnd_i);
                lifetime.Reset();

            } else if (not isOver) {
                lifetime.SetResetTime(0.3f);
                isOver = true;
            } else associated.enabled = false;
        }
    }
    // melius colliders' pixel correction
    associated.pixelColliderFix0 = (
        usingExternalAssociated and
        (not externalAssociated.expired()) and
        (externalAssociated.lock()->pixelColliderFix0 ^
        externalAssociated.lock()->pixelColliderFix1)
    );
}

void FoxAttack::NotifyCollision (GameObject& other) {
    GameObject* self = externalAssociated.lock().get();
    if (externalAssociated.expired() or (&other == self) or isOver)
        return;

    EntityMachine* entity = (EntityMachine*)other.GetComponent(ComponentType::_EntityMachine);

    if (entity != nullptr) {
        Vec2 selfPosition = self->box.GetPosition();
        float argsv[7] = {force.x, force.y, impulse, (float)damage, selfPosition.x, selfPosition.y, 0.0f};

        if (lifetime.GetTime() < lifetimeStart)
            return;

        Boss* boss = (Boss*)other.GetComponent(ComponentType::_Boss);
        if ((boss != nullptr) and (not boss->BreakBarrier(1.2f)))
            return;

        // ensures that the collision will only happen once effectively
        if (not entity->FormatState(EntityState::Injured, 7, argsv)) return;
    } else return;

    /*--------------------------------------------------------------------------------------------------*/
    // shakes the camera after a hit
    /*--------------------------------------------------------------------------------------------------*/

    cameraShakeReset = Vec2();
    int shakeRange = (CAMERA_SHAKE_RANGE << 1) + 1;
    for (int i=0; i < CAMERA_SHAKE_COUNT; i++) {
        Vec2 shake = Vec2(
            rand()%shakeRange-CAMERA_SHAKE_RANGE,
            rand()%shakeRange-CAMERA_SHAKE_RANGE);
        cameraShakeQueue.push(shake);
    }
    Camera::AddMethod(this, std::bind(&CameraShake, this));
}

void* FoxAttack::CameraShake () {
    if (cameraShakeQueue.empty()) {
        Camera::RemoveMethod(this);
        return nullptr;
    }

    if (cameraShakeTimer.IsOver()) {
        Camera::masterOffset += cameraShakeQueue.front() - cameraShakeReset;
        cameraShakeReset = cameraShakeQueue.front();
        cameraShakeQueue.pop();
        if (cameraShakeQueue.empty()) {
            Camera::masterOffset -= cameraShakeReset;
            cameraShakeTimer.FalseStart();
        }
    } else cameraShakeTimer.Update(Game::GetInstance().GetDeltaTime());
    return nullptr;
}
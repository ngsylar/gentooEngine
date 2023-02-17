#include "GentooEngine.h"
#include "KidAttackStrong.h"
#include "Boss.h"

#define SPRITE_ATTACK           "assets/img/kid/attackstrongtrail.png"
#define SPRITE_FRAMES           10, 0.1f
#define SPRITE_OFFSET_X         -8.0f
#define SPRITE_OFFSET_Y         0.0f

#define REPULSION_FORCE         140.0f
#define REPULSION_IMPULSE       4.0f

#define CAMERA_SHAKE_COUNT      6
#define CAMERA_SHAKE_RANGE      3
#define CAMERA_SHAKE_RESET_TIME 0.04f

KidAttackStrong::KidAttackStrong (
    GameObject& associated, GameObject* externalAssociated
): AttackGeneric(associated, externalAssociated) {
    this->externalAssociated = Game::GetInstance().GetCurrentState().GetObjectPtr(externalAssociated);
    associated.label = externalAssociated->label;
    associated.enabled = false;

    force = Vec2(640.0f, 0.0f);
    impulse = 80.0f;
    damage = 2;
    displacement = 0.0f;

    isOver = false;
}

void KidAttackStrong::PushAttack (Vec2 offset, Vec2 size, float lifetimeStart, float lifetimeEnd) {
    AttackType attackType;
    attackType.offset_i = offset;
    attackType.size_i = size;
    attackType.lifetimeStart_i = lifetimeStart;
    attackType.lifetimeEnd_i = lifetimeEnd;
    attackTypes.push(attackType);
}

void KidAttackStrong::Start () {
    OpenSprite(SPRITE_ATTACK, SPRITE_FRAMES, true);

    cameraShakeTimer.SetResetTime(CAMERA_SHAKE_RESET_TIME);
    cameraShakeTimer.FalseStart();
}

void KidAttackStrong::Perform (AttackDirection direction) {
    PushAttack(Vec2(-31.0f, -19.0f), Vec2(68.0f, 39.0f), 0.09f, 0.26f);

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

    repulsionEnabled = false;
    isOver = false;
}

void KidAttackStrong::Update (float dt) {
    if (usingExternalAssociated and externalAssociated.expired()) {
        associated.RequestDelete();
        return;
    }

    RigidBody* rigidBody = nullptr;
    if (repulsionEnabled)
        rigidBody = (RigidBody*)externalAssociated.lock()->GetComponent(ComponentType::_RigidBody);

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
                lifetime.SetResetTime(0.74f);
                isOver = true;

            } else {
                if (repulsionEnabled and (rigidBody != nullptr))
                    rigidBody->SetSpeedOnX(0.0f);
                impulseCancel = false;

                associated.enabled = false;
                return;
            }
        }
    }
    if (repulsionEnabled and (rigidBody != nullptr)) {
        if (repulsionIncrease < REPULSION_IMPULSE) {
            rigidBody->SetSpeedOnX(REPULSION_FORCE * (-direction));
            float increase = rigidBody->GetSpeed().x * dt;
            if (increase < 0) increase *= -1.0f;
            repulsionIncrease += increase;
        } else {
            rigidBody->SetSpeedOnX(0.0f);
            repulsionEnabled = false;
        }
    }
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

    // melius colliders' pixel correction
    associated.pixelColliderFix0 = (
        usingExternalAssociated and
        (not externalAssociated.expired()) and
        (externalAssociated.lock()->pixelColliderFix0 ^
        externalAssociated.lock()->pixelColliderFix1)
    );
}

void KidAttackStrong::NotifyCollision (GameObject& other) {
    GameObject* self = externalAssociated.lock().get();
    if (externalAssociated.expired() or (&other == self) or isOver)
        return;

    EntityMachine* entity = (EntityMachine*)other.GetComponent(ComponentType::_EntityMachine);
    if (entity != nullptr) {
        Vec2 selfPosition = self->box.GetPosition();
        float argsv[7] = {force.x, force.y, impulse, (float)damage, selfPosition.x, selfPosition.y, 0.0f};

        RigidBody* rigidBody = (RigidBody*)self->GetComponent(ComponentType::_RigidBody);
        RigidBody* otherRigidBody = (RigidBody*)other.GetComponent(ComponentType::_RigidBody);
        if ((rigidBody != nullptr) and (otherRigidBody != nullptr)) {

            if (lifetime.GetTime() < lifetimeStart) {
                Boss* boss = (Boss*)other.GetComponent(ComponentType::_Boss);
                if (boss != nullptr) {
                    rigidBody->SetSpeedOnX(0.0f);
                    impulseCancel = true;

                    if (not boss->BreakBarrier(1.2f)) return;
                } else otherRigidBody->SetSpeedOnX(rigidBody->GetSpeed().x);
                return;
            }
            if (rigidBody->GetSpeed().x != 0.0f) {
                argsv[_Displacement] = displacement;
                repulsionOriginX = self->box.x;
                repulsionIncrease = 0.0f;
                repulsionEnabled = true;

            } else {
                Collider* otherCollider = (Collider*)other.GetComponent(ComponentType::_Collider);
                float overlap = (direction == LEFT)?
                    ((otherCollider->box.x+otherCollider->box.w)-collider->box.x) :
                    ((collider->box.x+collider->box.w)-otherCollider->box.x);
                if (overlap > 0.0f) argsv[_Impulse] += overlap;
            }
        }
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

void* KidAttackStrong::CameraShake () {
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

bool KidAttackStrong::ImpulseIsCanceled () {
    return impulseCancel;
}
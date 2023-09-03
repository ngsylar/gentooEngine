#include "GentooEngine.h"
#include "BossAttackMelee.h"

#define CAMERA_SHAKE_COUNT      6
#define CAMERA_SHAKE_RANGE      3
#define CAMERA_SHAKE_RESET_TIME 0.04f

BossAttackMelee::BossAttackMelee (
    GameObject& associated, GameObject* externalAssociated
): AttackGeneric(associated, externalAssociated) {
    this->externalAssociated = Game::GetInstance().GetCurrentState().GetObjectPtr(externalAssociated);
    associated.label = externalAssociated->label;
    associated.enabled = false;
}
BossAttackMelee::~BossAttackMelee()
{SDL_Log("eu murri.");}
void BossAttackMelee::PushAttack (Vec2 pureOffset, Vec2 size, float lifetimeStart, float lifetimeEnd) {
    AttackType attackType;
    attackType.pureOffset_i = pureOffset;
    attackType.size_i = size;
    attackType.lifetimeStart_i = lifetimeStart;
    attackType.lifetimeEnd_i = lifetimeEnd;
    attackTypes.push(attackType);
}

void BossAttackMelee::Start () {
    cameraShakeTimer.SetResetTime(CAMERA_SHAKE_RESET_TIME);
    cameraShakeTimer.FalseStart();
}

void BossAttackMelee::Perform (AttackDirection direction) {
    if (attackTypes.empty()) return;
    AttackType attackType = attackTypes.front();
    attackTypes.pop();

    this->pureOffset = attackType.pureOffset_i;
    SetupCollider(Vec2(), attackType.size_i);
    this->lifetimeStart = attackType.lifetimeStart_i;
    lifetime.SetResetTime(attackType.lifetimeEnd_i);

    associated.enabled = true;
    this->direction = direction;
    lifetime.Reset();
}

void BossAttackMelee::Update (float dt) {
    if (usingExternalAssociated and externalAssociated.expired()) {
        associated.RequestDelete();
        return;
    }
    if (lifetime.HasResetTime()) {
        lifetime.Update(dt);
        if (lifetime.IsOver()) {
            if (attackTypes.empty()) {
                associated.enabled = false;
                return;
            }
            else Perform(direction);
        }
    }
    Rect externalBox = (Rect)(externalAssociated.lock()->box);

    switch (direction) {
        case LEFT:
            if (sprite != nullptr) sprite->textureFlip = SDL_FLIP_NONE;
            associated.box.x = externalBox.x + pureOffset.x;
            associated.box.y = externalBox.y + pureOffset.y;
            break;

        case RIGHT:
            if (sprite != nullptr) sprite->textureFlip = SDL_FLIP_HORIZONTAL;
            associated.box.x = externalBox.x + externalBox.w - pureOffset.x - associated.box.w;
            associated.box.y = externalBox.y + pureOffset.y;
            break;

        // case UP: break;
        // case DOWN: break;
        default: break;
    }
}

void BossAttackMelee::CancelAttack () {
    attackTypes = std::queue<AttackType>();
    lifetime.Update(lifetime.GetResetTime());
    associated.enabled = false;
}

void BossAttackMelee::NotifyCollision (GameObject& other) {
    GameObject* self = externalAssociated.lock().get();
    if (externalAssociated.expired() or (&other == self))
        return;

    EntityMachine* entity = (EntityMachine*)other.GetComponent(ComponentType::_EntityMachine);
    if (entity != nullptr) {
        Vec2 selfPosition = self->box.GetPosition();
        float argsv[7] = {force.x, force.y, impulse, (float)damage, selfPosition.x, selfPosition.y, 0.0f};

        if (lifetime.GetTime() < lifetimeStart)
            return;

        RigidBody* rigidBody = (RigidBody*)self->GetComponent(ComponentType::_RigidBody);
        if (rigidBody->GetSpeed().x != 0.0f)
            argsv[_Displacement] = displacement;

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

void* BossAttackMelee::CameraShake () {
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
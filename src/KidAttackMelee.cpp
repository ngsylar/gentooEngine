#include "GentooEngine.h"
#include "KidAttackMelee.h"

#define COLLIDER_OFFSET_X       24.0f
#define COLLIDER_OFFSET_Y       6.0f
#define COLLIDER_SIZE           24.0f, 44.0f

#define LIFETIME_START          0.09f
#define LIFETIME_END            0.26f

#define CAMERA_SHAKE_COUNT      6
#define CAMERA_SHAKE_RANGE      3
#define CAMERA_SHAKE_RESET_TIME 0.04f

KidAttackMelee::KidAttackMelee (
    GameObject& associated, GameObject* externalAssociated
): AttackGeneric(associated, externalAssociated) {
    this->externalAssociated = Game::GetInstance().GetCurrentState().GetObjectPtr(externalAssociated);
    associated.label = externalAssociated->label;
    lifetime.SetResetTime(LIFETIME_END);
    associated.enabled = false;
}

void KidAttackMelee::SetProperties (Vec2 force, float impulse, int damage, float displacement) {
    this->force = force;
    this->impulse = impulse;
    this->damage = damage;
    this->displacement = displacement;
}

void KidAttackMelee::Awaken () {
    SetupCollider(Vec2(), Vec2(COLLIDER_SIZE));
}

void KidAttackMelee::Start () {
    cameraShakeTimer.SetResetTime(CAMERA_SHAKE_RESET_TIME);
    cameraShakeTimer.FalseStart();
}

void KidAttackMelee::Perform () {
    associated.enabled = true;
    lifetime.Reset();
}

void KidAttackMelee::UpdateAttack (float dt) {
    Rect externalBox = (Rect)(externalAssociated.lock()->box);

    switch (direction) {
        case LEFT:
            if (sprite != nullptr) sprite->textureFlip = SDL_FLIP_HORIZONTAL;
            associated.box.x = externalBox.x + externalBox.w - COLLIDER_OFFSET_X - associated.box.w;
            associated.box.y = externalBox.y + COLLIDER_OFFSET_Y;
            break;

        case RIGHT:
            if (sprite != nullptr) sprite->textureFlip = SDL_FLIP_NONE;
            associated.box.x = externalBox.x + COLLIDER_OFFSET_X;
            associated.box.y = externalBox.y + COLLIDER_OFFSET_Y;
            break;

        // case UP: break;
        // case DOWN: break;
        default: break;
    }
}

void KidAttackMelee::NotifyCollision (GameObject& other) {
    GameObject* self = externalAssociated.lock().get();
    if (externalAssociated.expired() or (&other == self))
        return;

    EntityMachine* entity = (EntityMachine*)other.GetComponent(ComponentType::_EntityMachine);
    if (entity != nullptr) {
        float argsv[7] = {force.x, force.y, impulse, (float)damage, self->box.x, self->box.y, 0.0f};

        RigidBody* rigidBody = (RigidBody*)self->GetComponent(ComponentType::_RigidBody);
        RigidBody* otherRigidBody = (RigidBody*)other.GetComponent(ComponentType::_RigidBody);

        if (lifetime.GetTime() < LIFETIME_START) {
            otherRigidBody->SetSpeedOnX(rigidBody->GetSpeed().x);
            return;
        }
        if (rigidBody->GetSpeed().x != 0.0f)
            argsv[_Displacement] = displacement;
        else {

            Collider* otherCollider = (Collider*)other.GetComponent(ComponentType::_Collider);
            float overlap = (direction == LEFT)?
                ((otherCollider->box.x+otherCollider->box.w)-collider->box.x) :
                ((collider->box.x+collider->box.w)-otherCollider->box.x);
            if (overlap > 0.0f) argsv[_Impulse] += overlap;
        }

        // ensures that the collision will only happen once effectively
        if (not entity->FormatState(EntityState::Injured, 5, argsv)) return;
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

void* KidAttackMelee::CameraShake () {
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
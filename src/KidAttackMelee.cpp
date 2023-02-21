#include "GentooEngine.h"
#include "KidAttackMelee.h"

#define COLLIDER_OFFSET_X       24.0f
#define COLLIDER_OFFSET_Y       6.0f
#define COLLIDER_SIZE           24.0f, 44.0f

#define REPULSION_FORCE         140.0f
#define REPULSION_IMPULSE       4.0f

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

    impulseCancel = false;
    repulsionEnabled = false;
}

void KidAttackMelee::Awaken () {
    SetupCollider(Vec2(), Vec2(COLLIDER_SIZE));
}

void KidAttackMelee::Start () {
    cameraShakeTimer.SetResetTime(CAMERA_SHAKE_RESET_TIME);
    cameraShakeTimer.FalseStart();
}

void KidAttackMelee::Perform (AttackDirection direction) {
    associated.enabled = true;
    this->direction = direction;
    lifetime.Reset();

    repulsionEnabled = false;
}

void KidAttackMelee::Update (float dt) {
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

            if (repulsionEnabled and (rigidBody != nullptr))
                rigidBody->SetSpeedOnX(0.0f);
            impulseCancel = false;

            associated.enabled = false;
            return;
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

    // melius colliders' pixel correction
    associated.pixelColliderFix0 = (
        usingExternalAssociated and
        (not externalAssociated.expired()) and
        (externalAssociated.lock()->pixelColliderFix0 ^
        externalAssociated.lock()->pixelColliderFix1)
    );
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
        if ((rigidBody != nullptr) and (otherRigidBody != nullptr)) {

            if (lifetime.GetTime() < LIFETIME_START) {
                if (other.label == "Boss") {
                    rigidBody->SetSpeedOnX(0.0f);
                    impulseCancel = true;
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

bool KidAttackMelee::ImpulseIsCanceled () {
    return impulseCancel;
}
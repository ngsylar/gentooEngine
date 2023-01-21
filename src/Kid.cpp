#include "GentooEngine.h"
#include "Kid.h"

#define SPRITE_IDLE                 "assets/img/prota2.png"
#define SPRITE_RUN                  "assets/img/prota2_run.png"
#define SPRITE_RUN_FRAMES           8, 0.05f

#define RIGIDBODY_VELOCITY_MAX      400.0f
#define COLLIDER_POSITION           0.0f, 9.0f
#define COLLIDER_BOX_SIZE           16.0f, 30.0f

#define DAMAGE_FORCE                20.0f
#define DAMAGE_DISPLACEMENT_MAX     70.0f

#define CAMERA_BOX_RECT             16.0f, 19.0f, 16.0f, 28.0f
#define CAMERA_BOX_SPACING          0.0f, 12.0f
#define CINEMACHINE_LENGTH          14.0f, 86.0f
#define CINEMACHINE_SLICES          8, 32, 2, 24
#define CINEMACHINE_OFFSET          0.0f, -12.0f
#define CINEMACHINE_SETUP           true, true, true, false, true, true, false, false

#define CAMERA_DAMAGE_SHAKE_COUNT   6
#define CAMERA_DAMAGE_SHAKE_RANGE   3
#define CAMERA_GROUNDED_RESET_TIME  1.5f

Kid::Kid (GameObject& associated): Component(associated) {
    type = ComponentType::_Kid;
    // associated.SetLayer();
    associated.label = "Player";
    runSpeedMax = 150.0f;
    runAcceleration = 800.0f;
    jumpForce = 200.0f;
    jumpHeightMax = 70.0f;
    runSpeedNow = 0.0f;
    jumpHeightNow = 0.0f;
    jumpingIsConstant = false;
    damageDisplacement = Vec2(15.0f, -5.0f);
    invincibilityIimer.SetResetTime(1.25f);
    isInvincible = false;
    status = IDLE;
}

void Kid::Start () {
    sprite = new Sprite(associated, SPRITE_IDLE);
    sprite->AddTexture(SPRITE_RUN, SPRITE_RUN_FRAMES);
    sprite->SetTexture(IDLE);
    associated.AddComponent(sprite);
    
    rigidBody = new RigidBody(associated);
    associated.AddComponent(rigidBody);
    rigidBody->velocityMax.y = RIGIDBODY_VELOCITY_MAX;
    rigidBody->noInteractionLabels.push_back("Enemy");
    
    collider = new Collider(associated);
    associated.AddComponent(collider);
    collider->SetBox(Vec2(COLLIDER_POSITION), Vec2(COLLIDER_BOX_SIZE));

    cameraBox = new GameObject(associated.layer);
    CameraBox* boxComponent = new CameraBox(*cameraBox, &associated, CAMERA_BOX_SPACING);
    boxComponent->focusBoxOffset = Rect(CAMERA_BOX_RECT);
    cameraBox->AddComponent(boxComponent);
    boxComponent->AddMethod(this, std::bind(&CameraEffects, this));
    Game::GetInstance().GetCurrentState().AddObject(cameraBox);
    
    Camera::Follow(
        cameraBox, Vec2(CINEMACHINE_LENGTH), CINEMACHINE_SLICES,
        Camera::RIGHT, Camera::UP, Vec2(CINEMACHINE_OFFSET));
    Camera::cinemachine.Setup(CINEMACHINE_SETUP);
    cameraGroundedTimer.SetResetTime(CAMERA_GROUNDED_RESET_TIME);
}

void Kid::Update (float dt) {
    InputManager& input = InputManager::GetInstance();

    // handle status
    if (isInvincible) {
        invincibilityIimer.Update(dt);
        if (invincibilityIimer.IsOver())
            isInvincible = false;
    } if (status == DAMAGE) {
        TakeDamage(dt);
        return;
    } else if (rigidBody->IsGrounded()) {
        status = IDLE;
    } else if (rigidBody->GetVelocity().y > 0.0f) {
        status = FALL;
    }

    // handle jumping
    if (jumpingIsConstant)
        HandleJump(input.IsKeyDown(KEY_ARROW_UP), dt);
    if (input.KeyPress(KEY_ARROW_UP) and rigidBody->IsGrounded())
        StartJump(jumpForce*dt);

    // handle running
    if (input.IsKeyDown(KEY_ARROW_LEFT)) {
        runSpeedNow = (runSpeedNow < runSpeedMax) ? (runSpeedNow+runAcceleration*dt) : runSpeedMax;
        Run(-runSpeedNow*dt, SDL_FLIP_HORIZONTAL);
    }
    if (input.IsKeyDown(KEY_ARROW_RIGHT)) {
        runSpeedNow = (runSpeedNow < runSpeedMax) ? (runSpeedNow+runAcceleration*dt) : runSpeedMax;
        Run(runSpeedNow*dt, SDL_FLIP_NONE);
    }

    // handle animation
    if ((sprite->GetActiveTexture() == RUN) and (status != RUN)) {
        sprite->SetTexture(IDLE);
        runSpeedNow = 0.0f;
    }

    // debugging
    DebugScript(input, dt);
}

void Kid::Run (float displacement, SDL_RendererFlip flip) {
    rigidBody->Translate(Vec2(displacement,0));
    sprite->textureFlip = flip;
    if (status == IDLE) {
        if (sprite->GetActiveTexture() == IDLE)
            sprite->SetTexture(RUN);
        status = RUN;
    }
}

void Kid::StartJump (float displacement) {
    rigidBody->Translate(Vec2(0,-displacement));
    jumpHeightNow = displacement;
    rigidBody->gravityEnabled = false;
    jumpingIsConstant = true;
    status = JUMP;
}

void Kid::HandleJump (bool isKeyDown, float dt) {
    // if the button continues to be pressed increases the vertical jump height
    if (isKeyDown and (jumpHeightNow < jumpHeightMax)) {
        float jumpDisplacement = jumpForce * dt;
        rigidBody->Translate(Vec2(0,-jumpDisplacement));
        jumpHeightNow += jumpDisplacement;
        if (jumpHeightNow > jumpHeightMax)
            jumpHeightNow = jumpHeightMax;
    }
    // applies a constant force that gradually decreases due to the gravity force
    else {
        rigidBody->AddForce(Vec2(0,-((jumpForce*jumpHeightNow)/jumpHeightMax)));
        rigidBody->gravityEnabled = true;
        jumpingIsConstant = false;
    }
    // if it hits the ceiling cancels the jump or the force applied in previous conditions
    if (rigidBody->IsColliding(RigidBody::UP)) {
        rigidBody->CancelForces(RigidBody::VERTICAL);
        rigidBody->gravityEnabled = true;
        jumpingIsConstant = false;
    }
}

void Kid::TakeDamage (float dt) {
    if (collider->box.GetPosition().DistanceTo(damageOrigin) > DAMAGE_DISPLACEMENT_MAX) {
        rigidBody->Translate(damageDisplacement * DAMAGE_FORCE * 0.5f * dt);
        status = FALL;
    } else rigidBody->Translate(damageDisplacement * DAMAGE_FORCE * dt);
}

void Kid::NotifyCollision (GameObject& other) {
    if (not isInvincible) {
        if (other.label == "Enemy") {
            damageOrigin = ((Collider*)other.GetComponent(ComponentType::_Collider))->box.GetPosition();
            float signal = (associated.box.GetPosition().x < other.box.GetPosition().x) ? -1.0f : 1.0f;
            damageDisplacement.x = signal * fabs(damageDisplacement.x);
            rigidBody->CancelForces();
            invincibilityIimer.Reset();
            isInvincible = true;
            status = DAMAGE;
        }
    } else if ((status == DAMAGE) and (other.label != "Enemy")) {
        if (collider->box.GetPosition().DistanceTo(damageOrigin) < DAMAGE_DISPLACEMENT_MAX)
            AnimationShake();
        status = FALL;
    }
}

void Kid::AnimationShake () {
    cameraShakeReset = Vec2();
    int shakeRange = (CAMERA_DAMAGE_SHAKE_RANGE << 1) + 1;
    for (int i=0; i < CAMERA_DAMAGE_SHAKE_COUNT; i++) {
        Vec2 shake = Vec2(
            rand()%shakeRange-CAMERA_DAMAGE_SHAKE_RANGE,
            rand()%shakeRange-CAMERA_DAMAGE_SHAKE_RANGE
        ); cameraShakeQueue.push(shake);
    }
}

bool Kid::Is (ComponentType type) {
    return (type & this->type);
}

void* Kid::CameraEffects () {
    /*--------------------------------------------------------------------------------------------------*/
    // shakes the camera after a hit
    /*--------------------------------------------------------------------------------------------------*/

    if (not cameraShakeQueue.empty()) {
        Camera::masterOffset += cameraShakeQueue.front() - cameraShakeReset;
        cameraShakeReset = cameraShakeQueue.front();
        cameraShakeQueue.pop();
        if (cameraShakeQueue.empty())
            Camera::masterOffset -= cameraShakeReset;
    }

    /*--------------------------------------------------------------------------------------------------*/
    // returns the camera to original focus offset when kid is grounded
    /*--------------------------------------------------------------------------------------------------*/

    if (not rigidBody->IsGrounded()) {
        cameraGroundedTimer.Reset();
        return nullptr;
    }
    float dt = Game::GetInstance().GetDeltaTime();
    if (not cameraGroundedTimer.IsOver()) {
        cameraGroundedTimer.Update(dt);
        return nullptr;
    }

    float cameraBoxY = collider->box.y - (cameraBox->box.h - collider->box.h);
    if (cameraBox->box.y > cameraBoxY) {
        cameraBox->box.y -= (cameraBox->box.y - cameraBoxY) * dt;
        if (cameraBox->box.y < cameraBoxY) {
            cameraBox->box.y = cameraBoxY;
            cameraGroundedTimer.Reset();
        }
    } return nullptr;
}

void Kid::DebugScript (InputManager& input, float dt) {
    // rigidBody->gravityEnabled = false;
    // if (input.IsKeyDown(KEY_ARROW_LEFT))
    //     rigidBody->Translate(Vec2(-runSpeedMax,0)*dt);
    // if (input.IsKeyDown(KEY_ARROW_RIGHT))
    //     rigidBody->Translate(Vec2(runSpeedMax,0)*dt);
    // if (input.IsKeyDown(KEY_ARROW_UP))
    //     rigidBody->Translate(Vec2(0,-runSpeedMax)*dt);
    // if (input.IsKeyDown(KEY_ARROW_DOWN))
    //     rigidBody->Translate(Vec2(0,runSpeedMax)*dt);

    if (input.KeyPress(KEY_D) and cameraShakeQueue.empty())
        AnimationShake();

    if (input.KeyPress(KEY_SPACE)) {
        // SDL_Log("camera %f", Camera::pos.y);
        // SDL_Log("offset %f", Camera::offset.y);
        // SDL_Log("scroff %f", Camera::screenOffset.y);
        // SDL_Log("mstoff %f", Camera::masterOffset.y);
        // SDL_Log("distan %f", Camera::GetPosition().y - associated.box.GetPosition().y);
        SDL_Log("masteroffset %f %f", Camera::masterOffset.x, Camera::masterOffset.y);
    }
}
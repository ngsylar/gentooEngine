#include "GentooEngine.h"
#include "Kid.h"

#define SPRITE_IDLE                 "assets/img/prota2.png"
#define SPRITE_RUN                  "assets/img/prota2_run.png"
#define SPRITE_RUN_FRAMES           8, 0.05f

#define RIGIDBODY_VELOCITY_MAX      400.0f
#define COLLIDER_POSITION           0.0f, 9.0f
#define COLLIDER_BOX_SIZE           16.0f, 28.0f

#define CAMERA_BOX_RECT             16.0f, 19.0f, 16.0f, 28.0f
#define CAMERA_BOX_SPACING          0.0f, 12.0f
#define CINEMACHINE_LENGTH          14.0f, 86.0f
#define CINEMACHINE_SLICES          8, 32, 2, 24
#define CINEMACHINE_OFFSET          0.0f, -12.0f
#define CINEMACHINE_SETUP           true, true, true, false, true, true, false, false

Kid::Kid (GameObject& associated): Component(associated) {
    associated.label = "Player";
    runSpeedMax = 150.0f;
    runAcceleration = 800.0f;
    jumpForce = 200.0f;
    jumpHeightMax = 70.0f;
    runSpeedNow = 0.0f;
    jumpHeightNow = 0.0f;
    jumpingIsConstant = false;
    type = ComponentType::_Kid;
}

void Kid::Start () {
    sprite = new Sprite(associated, SPRITE_IDLE);
    sprite->AddTexture(SPRITE_RUN, SPRITE_RUN_FRAMES);
    sprite->SetTexture(IDLE);
    associated.AddComponent(sprite);
    
    rigidBody = new RigidBody(associated);
    associated.AddComponent(rigidBody);
    rigidBody->velocityMax.y = RIGIDBODY_VELOCITY_MAX;
    
    collider = new Collider(associated);
    associated.AddComponent(collider);
    collider->SetBox(Vec2(COLLIDER_POSITION), Vec2(COLLIDER_BOX_SIZE));

    cameraBox = new GameObject(associated.layer);
    CameraBox* boxComponent = new CameraBox(*cameraBox, &associated, CAMERA_BOX_SPACING);
    boxComponent->focusBoxOffset = Rect(CAMERA_BOX_RECT);
    cameraBox->AddComponent(boxComponent);
    boxComponent->AddMethod(this, std::bind(&CameraCheckGrounded, this));
    Game::GetInstance().GetCurrentState().AddObject(cameraBox);
    
    Camera::Follow(
        cameraBox, Vec2(CINEMACHINE_LENGTH), CINEMACHINE_SLICES,
        Camera::RIGHT, Camera::UP, Vec2(CINEMACHINE_OFFSET));
    Camera::cinemachine.Setup(CINEMACHINE_SETUP);
    cameraGroundedTimer.SetResetTime(1.5f);
}

void Kid::Update (float dt) {
    InputManager& input = InputManager::GetInstance();

    // handle status
    if (rigidBody->IsGrounded())
        status = IDLE;
    else if (rigidBody->GetVelocity().y > 0.0f)
        status = FALL;

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

    // // debugging
    // DebugScript(dt);
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

bool Kid::Is (ComponentType type) {
    return (type & this->type);
}

void Kid::NotifyCollision (GameObject& other) {

}

void* Kid::CameraCheckGrounded () {
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

void Kid::DebugScript (float dt) {
    InputManager& input = InputManager::GetInstance();

    rigidBody->gravityEnabled = false;
    if (input.IsKeyDown(KEY_ARROW_LEFT))
        rigidBody->Translate(Vec2(-runSpeedMax,0)*dt);
    if (input.IsKeyDown(KEY_ARROW_RIGHT))
        rigidBody->Translate(Vec2(runSpeedMax,0)*dt);
    if (input.IsKeyDown(KEY_ARROW_UP))
        rigidBody->Translate(Vec2(0,-runSpeedMax)*dt);
    if (input.IsKeyDown(KEY_ARROW_DOWN))
        rigidBody->Translate(Vec2(0,runSpeedMax)*dt);

    if (input.KeyPress(KEY_SPACE)) {
        SDL_Log("camera %f", Camera::pos.y);
        // SDL_Log("offset %f", Camera::offset.y);
        // SDL_Log("scroff %f", Camera::screenOffset.y);
        // SDL_Log("mstoff %f", Camera::masterOffset.y);
        // SDL_Log("distan %f", Camera::GetPosition().y - associated.box.GetPosition().y);
    }
}
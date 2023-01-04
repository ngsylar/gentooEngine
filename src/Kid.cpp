#include "GentooEngine.h"
#include "Kid.h"

#define SPRITE_IDLE                 "assets/img/prota2.png"
#define SPRITE_RUN                  "assets/img/prota2_run.png"

#define RIGIDBODY_VELOCITY_MAX      400.0f

#define CINEMACHINE_LENGTH          14.0f, 86.0f
#define CINEMACHINE_SLICES          8, 32, 2, 24
#define CINEMACHINE_OFFSET          0.0f, 12.0f
#define CINEMACHINE_SETUP           true, true, true, false, true, true, false, false

Kid::Kid (GameObject& associated): Component(associated) {
    associated.label = "Player";
    runSpeedMax = 150.0f;
    runAcceleration = 800.0f;
    jumpForce = 200.0f;
    jumpHeightMax = 70.0f;
    runSpeedNow = 0.0f;
    jumpHeightNow = 0.0f;
    isJumping = false;
}

void Kid::Start () {
    sprite = new Sprite(associated, SPRITE_IDLE);
    sprite->AddTexture(SPRITE_RUN, 8, 0.05f);
    sprite->SetTexture(IDLE);
    associated.AddComponent(sprite);

    // Camera::Follow(
    //     &associated, Vec2(CINEMACHINE_LENGTH), CINEMACHINE_SLICES,
    //     Camera::RIGHT, Camera::UP, Vec2(CINEMACHINE_OFFSET));
    // Camera::cinemachine.Setup(CINEMACHINE_SETUP);
    // Camera::offset.y = 0.0f;
    
    rigidBody = new RigidBody(associated);
    associated.AddComponent(rigidBody);
    rigidBody->velocityMax.y = RIGIDBODY_VELOCITY_MAX;
    
    collider = new Collider(associated);
    associated.AddComponent(collider);
    collider->SetBox(Vec2(0, 9), Vec2(16, 28));
}

void Kid::Update (float dt) {
    InputManager& input = InputManager::GetInstance();
    // CameraCheckTracking(dt);
    bool isRunning = false;

    if (isJumping)
        HandleJump(input.IsKeyDown(KEY_ARROW_UP), dt);
    if (input.KeyPress(KEY_ARROW_UP) and rigidBody->IsGrounded())
        StartJump(jumpForce*dt);

    if (input.IsKeyDown(KEY_ARROW_LEFT)) {
        runSpeedNow = (runSpeedNow < runSpeedMax) ? (runSpeedNow+runAcceleration*dt) : runSpeedMax;
        isRunning = Run(-runSpeedNow*dt, SDL_FLIP_HORIZONTAL);
    }
    if (input.IsKeyDown(KEY_ARROW_RIGHT)) {
        runSpeedNow = (runSpeedNow < runSpeedMax) ? (runSpeedNow+runAcceleration*dt) : runSpeedMax;
        isRunning = Run(runSpeedNow*dt, SDL_FLIP_NONE);
    }

    if ((sprite->GetActiveTexture() == RUN) and (not isRunning)) {
        sprite->SetTexture(IDLE);
        runSpeedNow = 0.0f;
    }

    // // remover
    // rigidBody->gravityEnabled = false;
    // if (input.IsKeyDown(KEY_ARROW_LEFT))
    //     rigidBody->Translate(Vec2(-runSpeedMax,0)*dt);
    // if (input.IsKeyDown(KEY_ARROW_RIGHT))
    //     rigidBody->Translate(Vec2(runSpeedMax,0)*dt);
    // if (input.IsKeyDown(KEY_ARROW_UP))
    //     rigidBody->Translate(Vec2(0,-runSpeedMax)*dt);
    // if (input.IsKeyDown(KEY_ARROW_DOWN))
    //     rigidBody->Translate(Vec2(0,runSpeedMax)*dt);

    // remover
    if (input.KeyPress(KEY_SPACE)) {
        SDL_Log("camera %f", Camera::pos.y);
        // SDL_Log("offset %f", Camera::offset.y);
        // SDL_Log("scroff %f", Camera::screenOffset.y);
        // SDL_Log("mstoff %f", Camera::masterOffset.y);
        // SDL_Log("distan %f", Camera::GetPosition().y - associated.box.GetPosition().y);
    }
}

bool Kid::Run (float displacement, SDL_RendererFlip flip) {
    rigidBody->Translate(Vec2(displacement,0));
    sprite->textureFlip = flip;
    if (sprite->GetActiveTexture() == IDLE)
        sprite->SetTexture(RUN);
    return true;
}

void Kid::StartJump (float displacement) {
    rigidBody->Translate(Vec2(0,-displacement));
    jumpHeightNow = displacement;
    rigidBody->gravityEnabled = false;
    isJumping = true;
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
        isJumping = false;
    }
    // if it hits the ceiling cancels the jump or the force applied in previous conditions
    if (rigidBody->IsColliding(RigidBody::UP)) {
        rigidBody->CancelForces(RigidBody::VERTICAL);
        rigidBody->gravityEnabled = true;
        isJumping = false;
    }
}

void Kid::CameraCheckTracking (float dt) {
    // resets to default setting if camera loses the track of it
    Vec2 windowHalfSize = Game::GetInstance().GetResolution();
    Vec2 cameraDistance = Camera::GetPosition() - associated.box.GetPosition();
    if ((fabs(cameraDistance.x) > windowHalfSize.x) or (fabs(cameraDistance.y) > windowHalfSize.y)) {
        Camera::Follow(
            &associated, Vec2(CINEMACHINE_LENGTH), CINEMACHINE_SLICES,
            Camera::RIGHT, Camera::UP, Vec2(CINEMACHINE_OFFSET));
        Camera::cinemachine.Setup(CINEMACHINE_SETUP);
    }
}
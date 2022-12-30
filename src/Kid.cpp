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
    runSpeed = 150.0f;
    jumpForce = 200.0f;
    jumpHeightMax = 70.0f;
    jumpHeight = 0.0f;
    isJumping = false;
}

void Kid::Start () {
    Camera::Follow(
        &associated, Vec2(CINEMACHINE_LENGTH), CINEMACHINE_SLICES,
        Camera::RIGHT, Camera::UP, Vec2(CINEMACHINE_OFFSET));
    Camera::cinemachine.Setup(CINEMACHINE_SETUP);
    Camera::offset.y = 0.0f;

    sprite = new Sprite(associated, SPRITE_IDLE);
    sprite->AddTexture(SPRITE_RUN, 8, 0.05f);
    sprite->SetTexture(IDLE);
    associated.AddComponent(sprite);
    
    rigidBody = new RigidBody(associated);
    associated.AddComponent(rigidBody);
    rigidBody->velocityMax.y = RIGIDBODY_VELOCITY_MAX;
    
    collider = new Collider(associated);
    associated.AddComponent(collider);
    collider->SetBox(Vec2(-1, 8), Vec2(14, 30));
}

void Kid::Update (float dt) {
    InputManager& input = InputManager::GetInstance();
    CameraCheckTracking(dt);
    bool isRunning = false;

    if (isJumping)
        HandleJump(input.IsKeyDown(KEY_ARROW_UP), dt);
    if (input.KeyPress(KEY_ARROW_UP) and rigidBody->IsGrounded())
        StartJump(dt);

    if (input.IsKeyDown(KEY_ARROW_LEFT)) {
        rigidBody->Translate(Vec2(-runSpeed,0)*dt);
        sprite->textureFlip = SDL_FLIP_HORIZONTAL;
        collider->offset.x = -2;
        if (sprite->GetActiveTexture() == IDLE)
            sprite->SetTexture(RUN);
        isRunning = true;
    }
    if (input.IsKeyDown(KEY_ARROW_RIGHT)) {
        rigidBody->Translate(Vec2(runSpeed,0)*dt);
        sprite->textureFlip = SDL_FLIP_NONE;
        collider->offset.x = -1;
        if (sprite->GetActiveTexture() == IDLE)
            sprite->SetTexture(RUN);
        isRunning = true;
    }

    if ((sprite->GetActiveTexture() == RUN) and (not isRunning)) {
        sprite->SetTexture(IDLE);
    }

    // // remover
    // rigidBody->gravityEnabled = false;
    // if (input.IsKeyDown(KEY_ARROW_UP))
    //     rigidBody->Translate(Vec2(0,-runSpeed)*dt);
    // if (input.IsKeyDown(KEY_ARROW_DOWN))
    //     rigidBody->Translate(Vec2(0,runSpeed)*dt);

    // remover
    if (input.KeyPress(KEY_SPACE)) {
        SDL_Log("camera %f", Camera::pos.y);
        // SDL_Log("offset %f", Camera::offset.y);
        // SDL_Log("scroff %f", Camera::screenOffset.y);
        // SDL_Log("mstoff %f", Camera::masterOffset.y);
        // SDL_Log("distan %f", Camera::GetPosition().y - associated.box.GetPosition().y);
    }
}

void Kid::StartJump (float dt) {
    float jumpDisplacement = jumpForce * dt;
    rigidBody->Translate(Vec2(0,-jumpDisplacement));
    jumpHeight = jumpDisplacement;
    rigidBody->gravityEnabled = false;
    isJumping = true;
}

void Kid::HandleJump (bool isKeyDown, float dt) {
    // if the button continues to be pressed increases the vertical jump height
    if (isKeyDown and (jumpHeight < jumpHeightMax)) {
        float jumpDisplacement = jumpForce * dt;
        rigidBody->Translate(Vec2(0,-jumpDisplacement));
        jumpHeight += jumpDisplacement;
        if (jumpHeight > jumpHeightMax)
            jumpHeight = jumpHeightMax;
    }
    // applies a constant force that gradually decreases due to the gravity force
    else {
        rigidBody->AddForce(Vec2(0,-((jumpForce*jumpHeight)/jumpHeightMax)));
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
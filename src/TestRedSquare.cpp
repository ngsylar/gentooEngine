#include "GentooEngine.h"
#include "TestObjects.h"

#define CINEMACHINE_LENGTH      25.0f, 175.0f
#define CINEMACHINE_SLICES      8, 32
#define CINEMACHINE_DEADSLICES  2, 28
#define CINEMACHINE_OFFSET      0.0f, 25.0f

Ball::Ball (GameObject& associated): Component(associated) {
    associated.label = "Player";
    runSpeed = 300.0f;
    jumpForce = 400.0f;
    jumpHeightMax = 140.0f;
    jumpHeight = 0.0f;
    isJumping = false;
}

void Ball::Start () {
    Camera::Follow(
        &associated, Vec2(CINEMACHINE_LENGTH), CINEMACHINE_SLICES, CINEMACHINE_DEADSLICES,
        Camera::RIGHT, Camera::UP, Vec2(CINEMACHINE_OFFSET)
    ); Camera::cinemachine.Setup(true, true, true, false, true, true, false, false);

    rigidBody = new RigidBody(associated);
    associated.AddComponent(rigidBody);
    associated.AddComponent(new Collider(associated));
    rigidBody->velocityMax.y = 800.0f;
}

void Ball::Update (float dt) {
    InputManager& input = InputManager::GetInstance();
    CheckTracking();

    if (isJumping)
        HandleJump(input.IsKeyDown(KEY_ARROW_UP), dt);

    if (input.KeyPress(KEY_ARROW_UP) and rigidBody->IsGrounded())
        StartJump(dt);
    if (input.IsKeyDown(KEY_ARROW_LEFT))
        rigidBody->Translate(Vec2(-runSpeed,0)*dt);
    if (input.IsKeyDown(KEY_ARROW_RIGHT))
        rigidBody->Translate(Vec2(runSpeed,0)*dt);

    // // editar: remover
    // rigidBody->gravityEnabled = false;
    // if (input.IsKeyDown(KEY_ARROW_UP))
    //     rigidBody->Translate(Vec2(0,-runSpeed)*dt);
    // if (input.IsKeyDown(KEY_ARROW_DOWN))
    //     rigidBody->Translate(Vec2(0,runSpeed)*dt);
}

void Ball::StartJump (float dt) {
    float jumpDisplacement = jumpForce * dt;
    rigidBody->Translate(Vec2(0,-jumpDisplacement));
    jumpHeight += jumpDisplacement;
    rigidBody->gravityEnabled = false;
    isJumping = true;
}

void Ball::HandleJump (bool isKeyDown, float dt) {
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
        jumpHeight = 0.0f;
        isJumping = false;
    }
    // if it hits the ceiling cancels the jump or the force applied in previous conditions
    if (rigidBody->IsColliding(RigidBody::UP)) {
        rigidBody->CancelForces(RigidBody::VERTICAL);
        rigidBody->gravityEnabled = true;
        jumpHeight = 0.0f;
        isJumping = false;
    }
}

void Ball::CheckTracking () {
    Vec2 cameraDistance = (associated.box.GetPosition() - Camera::GetPosition());
    if ((fabs(cameraDistance.x) > 332.0f) or (fabs(cameraDistance.y) > 332.0f)) {
        Camera::Follow(
            &associated, Vec2(CINEMACHINE_LENGTH), CINEMACHINE_SLICES, CINEMACHINE_DEADSLICES,
            Camera::RIGHT, Camera::UP, Vec2(CINEMACHINE_OFFSET)
        ); Camera::cinemachine.Setup(true, true, true, false, true, true, false, false);
    }
}
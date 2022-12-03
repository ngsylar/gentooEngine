#include "GentooEngine.h"
#include "TestObjects.h"

Ball::Ball (GameObject& associated): Component(associated) {
    associated.label = "Player";
    runSpeed = 300.0f;
    jumpForce = 400.0f;
    jumpHeightMax = 140.0f;
    jumpHeight = 0.0f;
    isJumping = false;
}

void Ball::Start () {
    rigidBody = new RigidBody(associated);
    associated.AddComponent(rigidBody);
    associated.AddComponent(new Collider(associated));
    rigidBody->velocityMax.y = 800.0f;
}

void Ball::Update (float dt) {
    InputManager& input = InputManager::GetInstance();

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

    // // editar: melhorar esse codigo ridiculo
    // if (not rigidBody->IsColliding(RigidBody::DOWN))
    //     Camera::cinemachine.Setup(false, false, true, false, false, false, false, false);
    // else Camera::cinemachine.Setup(true, true, true, false, true, true, false, false);
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
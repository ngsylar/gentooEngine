#include "GentooEngine.h"
#include "TestObjects.h"

Ball::Ball (GameObject& associated): Component(associated) {
    jumpTimer.SetResetTime(120.0f);
    jumpForce = 340.0f;
    isJumping = false;
    runSpeed = 200.0f;
}

void Ball::Start () {
    rigidBody = (RigidBody*)associated.GetComponent("RigidBody");
}

void Ball::Update (float dt) {
    InputManager& input = InputManager::GetInstance();

    if (isJumping) {
        // if the button continues to be pressed increases the vertical jump height
        if (not jumpTimer.IsOver() and input.IsKeyDown(KEY_ARROW_UP)) {
            jumpTimer.Update(jumpForce*dt);
            rigidBody->Translate(Vec2(0,-jumpForce)*dt);
        }
        // applies a constant force that gradually decreases due to the gravity force
        else {
            rigidBody->AddForce(Vec2(0,-jumpForce));
            rigidBody->gravityEnabled = true;
            isJumping = false;
            jumpTimer.Reset();
        }
        // if it hits the ceiling cancels the force applied in the previous condition
        if (rigidBody->collidingFaces[RigidBody::UP]) {
            rigidBody->CancelForces(RigidBody::VERTICAL);
            rigidBody->gravityEnabled = true;
            isJumping = false;
            jumpTimer.Reset();
        }
    }
    if (input.KeyPress(KEY_ARROW_UP) and rigidBody->IsGrounded()) {
        rigidBody->Translate(Vec2(0,-jumpForce)*dt);
        rigidBody->gravityEnabled = false;
        isJumping = true;
    }
    if (input.IsKeyDown(KEY_ARROW_LEFT)) {
        rigidBody->Translate(Vec2(-runSpeed,0)*dt);
    }
    if (input.IsKeyDown(KEY_ARROW_RIGHT)) {
        rigidBody->Translate(Vec2(runSpeed,0)*dt);
    }

    // // editar: remover
    // if (input.IsKeyDown(KEY_ARROW_UP))
    //     rigidBody->Translate(Vec2(0,-150)*dt);
    // if (input.IsKeyDown(KEY_ARROW_DOWN))
    //     rigidBody->Translate(Vec2(0,150)*dt);
}
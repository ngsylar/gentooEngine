#include "GentooEngine.h"
#include "TestObjects.h"

Ball::Ball (GameObject& associated): Component(associated) {
    jumpTimer.SetResetTime(120.0f);
    jumpForce = 340.0f;
    isJumping = false;
}

void Ball::Start () {
    rigidBody = (RigidBody*)associated.GetComponent("RigidBody");
}

void Ball::Update (float dt) {
    InputManager& input = InputManager::GetInstance();

    if (isJumping) {
        if (not jumpTimer.IsOver() and input.IsKeyDown(KEY_ARROW_UP)) {
            jumpTimer.Update(jumpForce*dt);
            rigidBody->Translate(Vec2(0,-jumpForce)*dt);
        }
        else {
            rigidBody->AddForce(Vec2(0,-jumpForce));
            rigidBody->gravityEnabled = true;
            isJumping = false;
            jumpTimer.Reset();
        }
        if (rigidBody->collidingFaces[RigidBody::UP]) {
            // rigidBody->CancelForces(RigidBody::VERTICAL); // editar: essa linha parece estar redundante, remover?
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
        rigidBody->Translate(Vec2(-150,0)*dt);
    }
    if (input.IsKeyDown(KEY_ARROW_RIGHT)) {
        rigidBody->Translate(Vec2(150,0)*dt);
    }
    // if (input.IsKeyDown(KEY_ARROW_UP))
    //     rigidBody->Translate(Vec2(0,-150)*dt);
    // if (input.IsKeyDown(KEY_ARROW_DOWN))
    //     rigidBody->Translate(Vec2(0,150)*dt);
}
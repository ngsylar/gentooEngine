#include "GentooEngine.h"
#include "TestObjects.h"

Ball::Ball (GameObject& associated): Component(associated) {
    jumpForce = 340;
    isJumping = false;
    jumpPseudoTimer = 0.0f;
}

void Ball::Start () {
    rigidBody = (RigidBody*)associated.GetComponent("RigidBody");
}

void Ball::Update (float dt) {
    InputManager& input = InputManager::GetInstance();
    
    if (isJumping) {
        if ((jumpPseudoTimer < 120) and input.IsKeyDown(KEY_ARROW_UP)) {
            jumpPseudoTimer += jumpForce * dt;
            rigidBody->Translate(Vec2(0,-jumpForce)*dt);
            SDL_Log("up");
        } else {
            rigidBody->AddForce(Vec2(0,-jumpForce));
            rigidBody->gravityEnabled = true;
            jumpPseudoTimer = 0.0f;
            isJumping = false;
        }
        if (rigidBody->collidingFaces[RigidBody::UP]) {
            rigidBody->CancelForces(RigidBody::VERTICAL);
            rigidBody->gravityEnabled = true;
            jumpPseudoTimer = 0.0f;
            isJumping = false;
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
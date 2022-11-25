#include "GentooEngine.h"
#include "TestObjects.h"

Ball::Ball (GameObject& associated): Component(associated) {
    runSpeed = 200.0f;
    jumpForce = 340.0f;
    jumpHeightMax = 140.0f;
    jumpHeight = 0.0f;
    isJumping = false;
}

void Ball::Start () {
    rigidBody = (RigidBody*)associated.GetComponent("RigidBody");
}

void Ball::Update (float dt) {
    InputManager& input = InputManager::GetInstance();

    // editar: usar altura maxima ao inves de timer
    if (isJumping) {
        // if the button continues to be pressed increases the vertical jump height
        if ((jumpHeight < jumpHeightMax) and input.IsKeyDown(KEY_ARROW_UP)) {
            float jumpDisplacement = jumpForce * dt;
            rigidBody->Translate(Vec2(0,-jumpDisplacement));
            jumpHeight += jumpDisplacement;
        }
        // applies a constant force that gradually decreases due to the gravity force
        else {
            // SDL_Log("end: %f", associated.box.GetPosition().y);
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
    if (input.KeyPress(KEY_ARROW_UP) and rigidBody->IsGrounded()) {
        // SDL_Log("beg: %f", associated.box.GetPosition().y);
        float jumpDisplacement = jumpForce * dt;
        rigidBody->Translate(Vec2(0,-jumpDisplacement));
        jumpHeight += jumpDisplacement;
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
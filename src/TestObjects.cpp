#include "GentooEngine.h"
#include "TestObjects.h"

Ball::Ball (GameObject& associated): Component(associated) {
    isJumping = false;
}

void Ball::Start () {
    rigidBody = (RigidBody*)associated.GetComponent("RigidBody");
}

void Ball::Update (float dt) {
    InputManager& input = InputManager::GetInstance();
    if (input.IsKeyDown(KEY_ARROW_LEFT))
        rigidBody->Translate(Vec2(-150,0)*dt);
    if (input.IsKeyDown(KEY_ARROW_RIGHT))
        rigidBody->Translate(Vec2(150,0)*dt);
    if (input.KeyPress(KEY_ARROW_UP)/* and rigidBody->IsGrounded()*/) {
        rigidBody->AddForce(Vec2(0,-340));
        // isJumping;
    }
    // if (isJumping) {
    //     if (input.IsKeyDown(KEY_ARROW_UP))
    //         rigidBody->
    // }
    // if (input.IsKeyDown(KEY_ARROW_UP))
    //     rigidBody->Translate(Vec2(0,-150)*dt);
    // if (input.IsKeyDown(KEY_ARROW_DOWN))
    //     rigidBody->Translate(Vec2(0,150)*dt);
}
#include "GentooEngine.h"
#include "TestObjects.h"

Ball::Ball (GameObject& associated): Component(associated) {}

void Ball::Start () {
    rigidBody = (RigidBody*)associated.GetComponent("RigidBody");
}

void Ball::Update (float dt) {
    InputManager& input = InputManager::GetInstance();
    if (input.IsKeyDown(KEY_ARROW_LEFT))
        associated.box.Translate(Vec2(-150,0)*dt);
    if (input.IsKeyDown(KEY_ARROW_RIGHT))
        associated.box.Translate(Vec2(150,0)*dt);
    if (input.KeyPress(KEY_ARROW_UP))
        rigidBody->AddForce(Vec2(0,-12));
}
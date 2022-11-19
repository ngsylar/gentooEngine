#include "GentooEngine.h"

RigidBody::RigidBody (GameObject& associated): Component(associated) {
    pixelScale = 2;
    gravity = 10.0f;
    acceleration = pixelScale * gravity;
    velocity = 0.0f;
    grounded = false;
}

void RigidBody::Start () {
    previousPosition = associated.box.GetPosition();
}

void RigidBody::Update (float dt) {
    Vec2 position = associated.box.GetPosition();
    movementDirection = (position - previousPosition).Normalize();
    previousPosition = position;
    
    HandleGravity(position, dt);
}

void RigidBody::HandleGravity (Vec2 position, float dt) {
    if (grounded) {
        velocity = 0.0f;
        return;
    }
    velocity += acceleration * dt;
    associated.box.SetPosition(position.x, position.y+velocity);
}

void RigidBody::NotifyCollision (GameObject& other) {
    Vec2 position = associated.box.GetPosition();
    float widthHalf = associated.box.w * 0.5f;
    float heightHalf = associated.box.h * 0.5f;

    float upFace = associated.box.y - associated.box.h;
    float downFace = associated.box.y + associated.box.h;
    float leftFace = associated.box.x - associated.box.w;
    float rightFace = associated.box.x + associated.box.w;
    
    float otherUpFace = other.box.y;
    float otherDownFace = other.box.y + other.box.h;
    float otherLeftFace = other.box.x;
    float otherRightFace = other.box.x + other.box.w;

    if ((not grounded) and (movementDirection.y > 0) and (downFace >= otherUpFace)) {
        associated.box.SetPosition(position.x, otherUpFace-heightHalf);
        grounded = true;
    }
    if ((movementDirection.y < 0) and (upFace <= otherDownFace))
        associated.box.SetPosition(position.x, otherDownFace+heightHalf);

    // if (leftFace <= otherRightFace)
    //     associated.box.SetPosition(position.x, otherRightFace+widthHalf);

    // if (rightFace >= otherLeftFace)
    //     associated.box.SetPosition(position.x, otherLeftFace-widthHalf);
}

bool RigidBody::Is (std::string type) {
    return (type == "RigidBody");
}
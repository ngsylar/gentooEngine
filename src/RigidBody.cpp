#include "GentooEngine.h"

RigidBody::RigidBody (GameObject& associated): Component(associated) {
    pixelScale = 2;
    gravity = 10.0f;
    acceleration = pixelScale * gravity;

    grounded = false;
    pastedLeft = false;
    pastedRight = false;
}

void RigidBody::Start () {
    previousPosition = associated.box.GetPosition();
}

void RigidBody::Update (float dt) {
    Vec2 position = associated.box.GetPosition();
    movementDirection = (position - previousPosition).Normalize();

    if (pastedLeft) {
        if (movementDirection.x < 0) {
            associated.box.SetPosition(previousPosition.x, position.y);
            position = Vec2(previousPosition.x, position.y);
            SDL_Log("pastedLeft");
        } else if (movementDirection.x > 0)
            pastedLeft = false;
    }
    if (pastedRight) {
        if (movementDirection.x > 0) {
            associated.box.SetPosition(previousPosition.x, position.y);
            position = Vec2(previousPosition.x, position.y);
            SDL_Log("pastedRight");
        } else if (movementDirection.x < 0)
            pastedRight = false;
    }

    previousPosition = position;
    HandleGravity(position, dt);
}

void RigidBody::HandleGravity (Vec2 position, float dt) {
    if (grounded) {
        velocity.y = 0.0f;
        return;
    }
    velocity.y += acceleration * dt;
    position += velocity;
    associated.box.SetPosition(position);
}

void RigidBody::AddForce (Vec2 force) {
    velocity += force;
    grounded = false; // remover
}

void RigidBody::NotifyCollision (GameObject& other) {
    Vec2 position = associated.box.GetPosition();
    float widthHalf = associated.box.w * 0.5f;
    float heightHalf = associated.box.h * 0.5f;

    float upFace = associated.box.y;
    float downFace = associated.box.y + associated.box.h;
    float leftFace = associated.box.x;
    float rightFace = associated.box.x + associated.box.w;
    
    float otherUpFace = other.box.y;
    float otherDownFace = other.box.y + other.box.h;
    float otherLeftFace = other.box.x;
    float otherRightFace = other.box.x + other.box.w;

    if ((not grounded) and (movementDirection.y > 0) and (downFace >= otherUpFace) and (upFace < otherUpFace)) {
        associated.box.SetPosition(position.x, otherUpFace-heightHalf);
        grounded = true;
        SDL_Log("collisionDown");
    }
    if ((movementDirection.y < 0) and (upFace <= otherDownFace) and (downFace > otherDownFace)) {
        associated.box.SetPosition(position.x, otherDownFace+heightHalf);
        velocity.y = 0.0f;
        SDL_Log("collisionUp");
    }
    if ((movementDirection.x < 0) and (leftFace <= otherRightFace) and (rightFace > otherRightFace)) {
        associated.box.SetPosition(otherRightFace+widthHalf, position.y);
        pastedLeft = true;
        SDL_Log("collisionLeft");
    }
    if ((movementDirection.x > 0) and (rightFace >= otherLeftFace) and (leftFace < otherLeftFace)) {
        associated.box.SetPosition(otherLeftFace-widthHalf, position.y);
        pastedRight = true;
        SDL_Log("collisionRight");
    }
}

bool RigidBody::Is (std::string type) {
    return (type == "RigidBody");
}
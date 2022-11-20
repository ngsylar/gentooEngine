#include "GentooEngine.h"

RigidBody::RigidBody (GameObject& associated): Component(associated) {
    // bodyType = DYNAMIC;

    pixelScale = 2;
    gravity = 10.0f;
    acceleration = pixelScale * gravity;

    collisionFace[UP] = false;
    collisionFace[DOWN] = false;
    collisionFace[LEFT] = false;
    collisionFace[RIGHT] = false;
}

void RigidBody::Start () {
    previousPosition = associated.box.GetPosition();
}

void RigidBody::Update (float dt) {
    Vec2 position = associated.box.GetPosition();
    movementDirection = (position - previousPosition).Normalize();

    // if (pastedLeft) {
    //     if (movementDirection.x < 0) {
    //         associated.box.SetPosition(previousPosition.x, position.y);
    //         position = Vec2(previousPosition.x, position.y);
    //         SDL_Log("pastedLeft");
    //     } else if (movementDirection.x > 0)
    //         pastedLeft = false;
    // }
    // if (pastedRight) {
    //     if (movementDirection.x > 0) {
    //         associated.box.SetPosition(previousPosition.x, position.y);
    //         position = Vec2(previousPosition.x, position.y);
    //         SDL_Log("pastedRight");
    //     } else if (movementDirection.x < 0)
    //         pastedRight = false;
    // }

    previousPosition = position;
    // HandleGravity(position, dt);
}

void RigidBody::HandleGravity (Vec2 position, float dt) {
    // if (grounded) {
    //     velocity.y = 0.0f;
    //     return;
    // }
    velocity.y += acceleration * dt;
    position += velocity;
    associated.box.SetPosition(position);
}

void RigidBody::AddForce (Vec2 force) {
    velocity += force;
    // grounded = false; // remover
}

void RigidBody::Translate (Vec2 displacement) {
    if ((collisionFace[UP] and (displacement.y < 0)) or (collisionFace[DOWN] and (displacement.y > 0)))
        displacement.y = 0;
    if ((collisionFace[LEFT] and (displacement.x < 0)) or (collisionFace[RIGHT] and (displacement.x > 0)))
        displacement.x = 0;
    associated.box.Translate(displacement);
}

void RigidBody::NotifyCollision (GameObject& other) {
    for (int i=0; i < (int)collidingObjects.size(); i++)
        if (collidingObjects[i].first.lock().get() == &other)
            return;

    std::weak_ptr<GameObject> otherPtr = Game::GetInstance().GetCurrentState().GetObjectPtr(&other);
    Vec2 position = associated.box.GetPosition();
    float widthHalf = associated.box.w * 0.5f;
    float heightHalf = associated.box.h * 0.5f;

    float faceUp = associated.box.y;
    float faceDown = associated.box.y + associated.box.h;
    float faceLeft = associated.box.x;
    float faceRight = associated.box.x + associated.box.w;
    
    float otherFaceUp = other.box.y;
    float otherFaceDown = other.box.y + other.box.h;
    float otherFaceLeft = other.box.x;
    float otherFaceRight = other.box.x + other.box.w;

    if ((movementDirection.y < 0) and (faceUp <= otherFaceDown) and (faceDown > otherFaceDown)) {
        associated.box.SetPosition(position.x, otherFaceDown+heightHalf);
        collidingObjects.push_back(std::make_pair(otherPtr, UP));
        collisionFace[UP] = true;
        // velocity.y = 0.0f;
        SDL_Log("collisionUp");
    }
    if ((movementDirection.y > 0) and (faceDown >= otherFaceUp) and (faceUp < otherFaceUp)) {
        associated.box.SetPosition(position.x, otherFaceUp-heightHalf);
        collidingObjects.push_back(std::make_pair(otherPtr, DOWN));
        collisionFace[DOWN] = true;
        SDL_Log("collisionDown");
    }
    if ((movementDirection.x < 0) and (faceLeft <= otherFaceRight) and (faceRight > otherFaceRight)) {
        associated.box.SetPosition(otherFaceRight+widthHalf, position.y);
        collidingObjects.push_back(std::make_pair(otherPtr, LEFT));
        collisionFace[LEFT] = true;
        SDL_Log("collisionLeft");
    }
    if ((movementDirection.x > 0) and (faceRight >= otherFaceLeft) and (faceLeft < otherFaceLeft)) {
        associated.box.SetPosition(otherFaceLeft-widthHalf, position.y);
        collidingObjects.push_back(std::make_pair(otherPtr, RIGHT));
        collisionFace[RIGHT] = true;
        SDL_Log("collisionRight");
    }
}

void RigidBody::NotifyNoCollision (GameObject& other) {
    for (int i=(int)collidingObjects.size()-1; i >= 0; i--)
        if (collidingObjects[i].first.lock().get() == &other) {
            collisionFace[collidingObjects[i].second] = false;
            collidingObjects.erase(collidingObjects.begin()+i);
            break;
        }
}

bool RigidBody::Is (std::string type) {
    return (type == "RigidBody");
}
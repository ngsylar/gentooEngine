#include "GentooEngine.h"

RigidBody::RigidBody (GameObject& associated): Component(associated) {
    // bodyType = DYNAMIC;

    pixelScale = 2;
    gravity = 10.0f;
    acceleration = pixelScale * gravity;

    collidingFaces[UP] = false;
    collidingFaces[DOWN] = false;
    collidingFaces[LEFT] = false;
    collidingFaces[RIGHT] = false;
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
    if ((collidingFaces[UP] and (displacement.y < 0)) or (collidingFaces[DOWN] and (displacement.y > 0)))
        displacement.y = 0;
    if ((collidingFaces[LEFT] and (displacement.x < 0)) or (collidingFaces[RIGHT] and (displacement.x > 0)))
        displacement.x = 0;
    associated.box.Translate(displacement);
}

void RigidBody::NotifyCollision (GameObject& other) {
    /*--------------------------------------------------------------------------------------------------*/
    // check if a collision with the other already exists
    /*--------------------------------------------------------------------------------------------------*/

    for (int i=0; i < (int)collidingOthers.size(); i++)
        if (collidingOthers[i].first.lock().get() == &other)
            return;

    /*--------------------------------------------------------------------------------------------------*/
    // defines the collision contact face on the associated
    /*--------------------------------------------------------------------------------------------------*/

    std::weak_ptr<GameObject> otherPtr = Game::GetInstance().GetCurrentState().GetObjectPtr(&other);
    bool isColliding[4] = {false, false, false, false};

    if (movementDirection.y < 0) {
        isColliding[UP] = true;
        // velocity.y = 0.0f;
    }
    if (movementDirection.y > 0) {
        isColliding[DOWN] = true;
    }
    if (movementDirection.x < 0) {
        isColliding[LEFT] = true;
    }
    if (movementDirection.x > 0) {
        isColliding[RIGHT] = true;
    }

    /*--------------------------------------------------------------------------------------------------*/
    // get information about the objects
    /*--------------------------------------------------------------------------------------------------*/

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

    /*--------------------------------------------------------------------------------------------------*/
    // decides the contact face when there is competition on the edges of the other
    /*--------------------------------------------------------------------------------------------------*/

    if (isColliding[UP]) {
        if (isColliding[LEFT]) {
            if (otherFaceRight-faceLeft < otherFaceDown-faceUp)
                isColliding[UP] = false;
            else
                isColliding[LEFT] = false;
        }
        else if (isColliding[RIGHT]) {
            if (faceRight-otherFaceLeft < otherFaceDown-faceUp)
                isColliding[UP] = false;
            else
                isColliding[RIGHT] = false;
        }
    } else if (isColliding[DOWN]) {
        if (isColliding[LEFT]) {
            if (otherFaceRight-faceLeft < faceDown-otherFaceUp)
                isColliding[DOWN] = false;
            else
                isColliding[LEFT] = false;
        }
        else if (isColliding[RIGHT]) {
            if (faceRight-otherFaceLeft < faceDown-otherFaceUp)
                isColliding[DOWN] = false;
            else
                isColliding[RIGHT] = false;
        }
    }
    
    /*--------------------------------------------------------------------------------------------------*/
    // pushes the associated away from the other
    /*--------------------------------------------------------------------------------------------------*/

    if (isColliding[UP]) {
        associated.box.SetPosition(position.x, otherFaceDown+heightHalf);
        collidingOthers.push_back(std::make_pair(otherPtr, UP));
        collidingFaces[UP] = true;
    }
    if (isColliding[DOWN]) {
        associated.box.SetPosition(position.x, otherFaceUp-heightHalf);
        collidingOthers.push_back(std::make_pair(otherPtr, DOWN));
        collidingFaces[DOWN] = true;
    }
    if (isColliding[LEFT]) {
        associated.box.SetPosition(otherFaceRight+widthHalf, position.y);
        collidingOthers.push_back(std::make_pair(otherPtr, LEFT));
        collidingFaces[LEFT] = true;
    }
    if (isColliding[RIGHT]) {
        associated.box.SetPosition(otherFaceLeft-widthHalf, position.y);
        collidingOthers.push_back(std::make_pair(otherPtr, RIGHT));
        collidingFaces[RIGHT] = true;
    }
    // SDL_Log("%d %d %d %d", isColliding[UP], isColliding[DOWN], isColliding[LEFT], isColliding[RIGHT]);
}

void RigidBody::NotifyNoCollision (GameObject& other) {
    /*--------------------------------------------------------------------------------------------------*/
    // remove movement restriction
    /*--------------------------------------------------------------------------------------------------*/

    bool isColliding[4] = {false, false, false, false};

    for (int i=(int)collidingOthers.size()-1; i >= 0; i--) {
        if (collidingOthers[i].first.lock().get() == &other)
            collidingOthers.erase(collidingOthers.begin()+i);
        else
            isColliding[collidingOthers[i].second] = true;
    }

    /*--------------------------------------------------------------------------------------------------*/
    // prevents the associated from traversing the others
    /*--------------------------------------------------------------------------------------------------*/
    
    collidingFaces[UP] = isColliding[UP];
    collidingFaces[DOWN] = isColliding[DOWN];
    collidingFaces[LEFT] = isColliding[LEFT];
    collidingFaces[RIGHT] = isColliding[RIGHT];
}

bool RigidBody::Is (std::string type) {
    return (type == "RigidBody");
}
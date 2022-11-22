#include "GentooEngine.h"

RigidBody::RigidBody (GameObject& associated): Component(associated) {
    // bodyType = DYNAMIC;
    gravityEnabled = true;
    gravityValue = 20.0f;

    collidingFaces[UP] = false;
    collidingFaces[DOWN] = false;
    collidingFaces[LEFT] = false;
    collidingFaces[RIGHT] = false;
}

void RigidBody::Start () {
    Vec2 position = associated.box.GetPosition();
    previousPosition.emplace(position);
    previousPosition.emplace(position);
    previousPosition.emplace(position);
}

void RigidBody::Update (float dt) {
    Vec2 position = associated.box.GetPosition();
    movementDirection = position - previousPosition.front();
    previousPosition.emplace(position);
    previousPosition.pop();

    // SDL_Log("1: %f\t%f", previousPosition.front().x, previousPosition.front().y);
    // SDL_Log("3: %f\t%f", position.x, position.y);
    
    if (gravityEnabled)
        HandleGravity();
    Translate(velocity*dt);
}

bool RigidBody::IsGrounded () {
    return collidingFaces[DOWN];
}

Vec2 RigidBody::GetVelocity () {
    return velocity;
}

void RigidBody::HandleGravity () {
    float gravitationalAcceleration = 0.0f;

    if (collidingFaces[DOWN])
        gravitationalAcceleration = 0.0f;
    else
        gravitationalAcceleration = gravityValue;

    velocity.y += gravitationalAcceleration;
}

void RigidBody::Translate (Vec2 displacement) {
    if ((collidingFaces[UP] and (displacement.y < 0)) or (collidingFaces[DOWN] and (displacement.y > 0)))
        displacement.y = 0;
    if ((collidingFaces[LEFT] and (displacement.x < 0)) or (collidingFaces[RIGHT] and (displacement.x > 0)))
        displacement.x = 0;
    associated.box.Translate(displacement);
}

void RigidBody::AddForce (Vec2 force) {
    velocity += force;
}

void RigidBody::CancelForces (RigidBody::ForceDirection axis) {
    switch (axis) {
        case ALL:
            velocity = Vec2();
            break;
        case VERTICAL:
            velocity.y = 0.0f;
            break;
        case HORIZONTAL:
            velocity.x = 0.0f;
            break;        
        default: break;
    }
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
    bool isColliding[4] = {false};
    SDL_Log("%f\t%f", movementDirection.x, movementDirection.y);

    if (movementDirection.y < 0) {
        isColliding[UP] = true;
        SDL_Log("UP");
    }
    if (movementDirection.y > 0) {
        isColliding[DOWN] = true;
        SDL_Log("DOWN");
    }
    if (movementDirection.x < 0) {
        isColliding[LEFT] = true;
        SDL_Log("LEFT");
    }
    if (movementDirection.x > 0) {
        isColliding[RIGHT] = true;
        SDL_Log("RIGHT");
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
        velocity.y = 0.0f;
    }
    if (isColliding[DOWN]) {
        associated.box.SetPosition(position.x, otherFaceUp-heightHalf);
        collidingOthers.push_back(std::make_pair(otherPtr, DOWN));
        collidingFaces[DOWN] = true;
        velocity.y = 0.0f;
    }
    if (isColliding[LEFT]) {
        associated.box.SetPosition(otherFaceRight+widthHalf, position.y);
        collidingOthers.push_back(std::make_pair(otherPtr, LEFT));
        collidingFaces[LEFT] = true;
        velocity.x = 0.0f;
    }
    if (isColliding[RIGHT]) {
        associated.box.SetPosition(otherFaceLeft-widthHalf, position.y);
        collidingOthers.push_back(std::make_pair(otherPtr, RIGHT));
        collidingFaces[RIGHT] = true;
        velocity.x = 0.0f;
    }
    // SDL_Log("%d %d %d %d", isColliding[UP], isColliding[DOWN], isColliding[LEFT], isColliding[RIGHT]);
}

void RigidBody::NotifyNoCollision (GameObject& other) {
    /*--------------------------------------------------------------------------------------------------*/
    // remove movement restriction
    /*--------------------------------------------------------------------------------------------------*/

    bool isColliding[4] = {false};

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
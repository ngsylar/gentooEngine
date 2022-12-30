#include "GentooEngine.h"

RigidBody::RigidBody (GameObject& associated, float gravityValue): Component(associated) {
    // bodyType = DYNAMIC;
    gravityEnabled = true;
    this->gravityValue = gravityValue;

    collidingFaces[UP] = false;
    collidingFaces[DOWN] = false;
    collidingFaces[LEFT] = false;
    collidingFaces[RIGHT] = false;
}

RigidBody::~RigidBody () {
    collidingOthers.clear();
}

void RigidBody::Start () {
    Vec2 position = associated.box.GetPosition();
    previousPosition.push(position);
    previousPosition.push(position);
    previousPosition.push(position);
}

void RigidBody::Update (float dt) {
    Vec2 position = associated.box.GetPosition();
    movementDirection = position - previousPosition.front();
    previousPosition.push(position);
    previousPosition.pop();
    
    if (gravityEnabled)
        HandleGravity(dt);
    
    Translate(velocity * dt);
    CheckDeletedColliders();
}

void RigidBody::HandleGravity (float dt) {
    float gravitationalAcceleration = 0.0f;

    if (collidingFaces[DOWN])
        gravitationalAcceleration = 0.0f;
    else
        gravitationalAcceleration = gravityValue;

    if ((velocityMax.y > 0.0f) and (velocity.y >= velocityMax.y))
        velocity.y = velocityMax.y;
    else
        velocity.y += gravitationalAcceleration * dt;
}

bool RigidBody::IsGrounded () {
    return collidingFaces[DOWN];
}

Vec2 RigidBody::GetVelocity () {
    return velocity;
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

void RigidBody::CancelForces (RigidBody::Axis axis) {
    switch (axis) {
        case HORIZONTAL:
            velocity.x = 0.0f;
            break;
        case VERTICAL:
            velocity.y = 0.0f;
            break;
        case ALL:
            velocity = Vec2();
            break;
        default: break;
    }
}

void RigidBody::NotifyCollision (GameObject& other) {
    /*--------------------------------------------------------------------------------------------------*/
    // check if the other collider is trigger or if a collision with the other already exists
    /*--------------------------------------------------------------------------------------------------*/

    Collider* otherCollider = (Collider*)other.GetComponent("Collider");
    if (otherCollider->isTrigger) return;
    
    for (int i=0; i < (int)collidingOthers.size(); i++)
        if (collidingOthers[i].first.lock().get() == &other)
            return;

    /*--------------------------------------------------------------------------------------------------*/
    // defines the collision contact face on the associated
    /*--------------------------------------------------------------------------------------------------*/

    std::weak_ptr<GameObject> otherPtr = Game::GetInstance().GetCurrentState().GetObjectPtr(&other);
    bool isColliding[4] = {false};
    // SDL_Log("%f\t%f", movementDirection.x, movementDirection.y);

    if (movementDirection.y < 0) {
        isColliding[UP] = true;
        // SDL_Log("UP");
    }
    if (movementDirection.y > 0) {
        isColliding[DOWN] = true;
        // SDL_Log("DOWN");
    }
    if (movementDirection.x < 0) {
        isColliding[LEFT] = true;
        // SDL_Log("LEFT");
    }
    if (movementDirection.x > 0) {
        isColliding[RIGHT] = true;
        // SDL_Log("RIGHT");
    }

    /*--------------------------------------------------------------------------------------------------*/
    // get information about the objects
    /*--------------------------------------------------------------------------------------------------*/

    Collider* collider = (Collider*)associated.GetComponent("Collider");
    Vec2 position = associated.box.GetPosition();

    float halfBody[4] = {
        (collider->box.h*0.5f)-collider->offset.y, (collider->box.h*0.5f)+collider->offset.y,
        (collider->box.w*0.5f)-collider->offset.x, (collider->box.w*0.5f)+collider->offset.x
    };
    float faces[4] = {
        collider->box.y, collider->box.y+collider->box.h,
        collider->box.x, collider->box.x+collider->box.w
    };
    float otherFaces[4] = {
        otherCollider->box.y, otherCollider->box.y+otherCollider->box.h,
        otherCollider->box.x, otherCollider->box.x+otherCollider->box.w
    };
    float pushFaceTo[4] = {
        faces[DOWN]-otherFaces[UP], otherFaces[DOWN]-faces[UP],
        faces[RIGHT]-otherFaces[LEFT], otherFaces[RIGHT]-faces[LEFT]
    };

    /*--------------------------------------------------------------------------------------------------*/
    // decides the contact face when there is competition on the edges of the other
    /*--------------------------------------------------------------------------------------------------*/

    if (isColliding[UP]) {
        if (isColliding[LEFT]) {
            if (pushFaceTo[RIGHT] < pushFaceTo[DOWN])
                isColliding[UP] = false;
            else isColliding[LEFT] = false;
        }
        else if (isColliding[RIGHT]) {
            if (pushFaceTo[LEFT] < pushFaceTo[DOWN])
                isColliding[UP] = false;
            else isColliding[RIGHT] = false;
        }
    } else if (isColliding[DOWN]) {
        if (isColliding[LEFT]) {
            if (pushFaceTo[RIGHT] < pushFaceTo[UP])
                isColliding[DOWN] = false;
            else isColliding[LEFT] = false;
        }
        else if (isColliding[RIGHT]) {
            if (pushFaceTo[LEFT] < pushFaceTo[UP])
                isColliding[DOWN] = false;
            else isColliding[RIGHT] = false;
        }
    }
    
    /*--------------------------------------------------------------------------------------------------*/
    // pushes the associated away from the other
    /*--------------------------------------------------------------------------------------------------*/

    if (isColliding[UP]) {
        associated.box.SetPosition(position.x, otherFaces[DOWN]+halfBody[UP]);
        collidingOthers.push_back(std::make_pair(otherPtr, UP));
        collidingFaces[UP] = true;
        velocity.y = 0.0f;
    }
    if (isColliding[DOWN]) {
        associated.box.SetPosition(position.x, otherFaces[UP]-halfBody[DOWN]);
        collidingOthers.push_back(std::make_pair(otherPtr, DOWN));
        collidingFaces[DOWN] = true;
        velocity.y = 0.0f;
    }
    if (isColliding[LEFT]) {
        associated.box.SetPosition(otherFaces[RIGHT]+halfBody[LEFT], position.y);
        collidingOthers.push_back(std::make_pair(otherPtr, LEFT));
        collidingFaces[LEFT] = true;
        velocity.x = 0.0f;
    }
    if (isColliding[RIGHT]) {
        associated.box.SetPosition(otherFaces[LEFT]-halfBody[RIGHT], position.y);
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

bool RigidBody::IsColliding (ColliderFace face) {
    return collidingFaces[face];
}

void RigidBody::CheckDeletedColliders () {
    int collidingCount[4] = {0};

    for (int i=0; i < (int)collidingOthers.size(); i++)
        if (collidingOthers[i].first.expired())
            collidingOthers.erase(collidingOthers.begin()+i);
        else
            collidingCount[collidingOthers[i].second]++;

    for (int i=0; i<4; i++)
        if (collidingCount[i] == 0)
            collidingFaces[i] = false;
}

bool RigidBody::Is (std::string type) {
    return (type == "RigidBody");
}
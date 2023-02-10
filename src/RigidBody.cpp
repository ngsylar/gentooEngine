#include "GentooEngine.h"

#define GRAVITY 900
#define LIMITSPD 400

RigidBody::RigidBody(GameObject& associated)
: Component(associated), 
friction(1,1), 
speed(0,0)
{
    type = ComponentType::_RigidBody;
    gravity = GRAVITY;
    limitspeed = LIMITSPD;
}

RigidBody::~RigidBody() {}

//Gravity
float RigidBody::GetGravity() {
    return gravity;
}
void RigidBody::SetGravity(float gravity) {
    this->gravity = gravity;
}
void RigidBody::ResetGravity() {
    this->gravity = GRAVITY;
}

//Speed
Vec2 RigidBody::GetSpeed() {
    return speed;
}
void RigidBody::SetSpeed(Vec2 force) {
    this->speed = force;
}
void RigidBody::SetSpeedOnX(float x) {
    this->speed.x = x;
}
void RigidBody::SetSpeedOnY(float y) {
    this->speed.y = y;
}
void RigidBody::ResetSpeed() {
    this->speed = Vec2(0,0);
}

//Friction
void RigidBody::ApplyFriction(Vec2 friction) {
    this->friction = friction;
}
void RigidBody::ResetFriction() {
    this->friction = Vec2(1,1);
}

//Inheritance
#define REPULSION_FACTOR 1e-4
void RigidBody::NotifyCollision(GameObject& other) {
    Collider* A = (Collider*)associated.GetComponent(ComponentType::_Collider);
    Collider* B = (Collider*)other.GetComponent(ComponentType::_Collider);

    if (A->isTrigger or B->isTrigger) return;
    for (std::string label : triggerLabels)
        if (other.label == label) return;

    Rect intersection = A->box.GetIntersection(B->box);

    if ((intersection.h > intersection.w) or ((intersection.w < 3.0f) and (intersection.h < 2.0f))) {
        //hit left or right
            right = intersection.x>A->box.x;
            left = !right;
            associated.box.x += (intersection.w + REPULSION_FACTOR)*(right?(-1):1);
            SetSpeedOnX(0);
    } else {
        if(intersection.y > A->box.y)
        {
            //hit floor
            down = true;
            ResetGravity();
            SetSpeedOnY(0);
            associated.box.y-= intersection.h + REPULSION_FACTOR;
        } else {
            //hit top
            up = true;
            associated.box.y += (intersection.h + REPULSION_FACTOR);
        }
    }
    A->Update(0);
}

void RigidBody::NotifyNoCollision(GameObject& other) {
    // RigidBody* rbB = (RigidBody*)other.GetComponent(ComponentType::_RigidBody);
    // if (isTrigger or ((rbB != nullptr) and (rbB->isTrigger))) return;
    // for (std::string label : triggerLabels)
    //     if (other.label == label) return;

    up = false;
    down = false;
    left = false;
    right = false;
}

void RigidBody::Render() {}

void RigidBody::Start() {}

void RigidBody::Update(float dt) {
    speed.y += gravity*dt;
    speed.y = (speed.y>limitspeed?limitspeed:speed.y);
    associated.box.x += speed.x*dt;
    associated.box.y += speed.y*dt;
}

bool RigidBody::ImpactUp() {
    return up;
}

bool RigidBody::ImpactDown() {
    return down;
}

bool RigidBody::ImpactLeft() {
    return left;
}

bool RigidBody::ImpactRight() {
    return right;
}

float RigidBody::GetLimitSpeed() {
    return limitspeed;
}
void RigidBody::SetLimitSpeed(float limit) {
    limitspeed = limit;
}

bool RigidBody::Is(ComponentType type) {
    return type & this->type;
}

bool RigidBody::Is(std::string type){
    return type == "RigidBody";
}
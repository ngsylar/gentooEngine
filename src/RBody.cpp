#include "GentooEngine.h"

#define GRAVITY 600
#define LIMITSPD 400

RBody::RBody(GameObject& associated)
: Component(associated), 
friction(1,1), 
speed(0,0)
{
    gravity = GRAVITY;
    limitspeed = LIMITSPD;
    type = ComponentType::_RBody;
}

RBody::~RBody() {}

//Gravity
float RBody::GetGravity() {
    return gravity;
}
void RBody::SetGravity(float gravity) {
    this->gravity = gravity;
}
void RBody::ResetGravity() {
    this->gravity = GRAVITY;
}

//Speed
Vec2 RBody::GetSpeed() {
    return speed;
}
void RBody::SetSpeed(Vec2 force) {
    this->speed = force;
}
void RBody::SetSpeedOnX(float x) {
    this->speed.x = x;
}
void RBody::SetSpeedOnY(float y) {
    this->speed.y = y;
}
void RBody::ResetSpeed() {
    this->speed = Vec2(0,0);
}

//Friction
void RBody::ApplyFriction(Vec2 friction) {
    this->friction = friction;
}
void RBody::ResetFriction() {
    this->friction = Vec2(1,1);
}

//Inheritance
#define REPULSION_FACTOR 1e-4
void RBody::NotifyCollision(GameObject& other) {
    Collider* A = (Collider*)associated.GetComponent(ComponentType::_Collider);
    Collider* B = (Collider*)other.GetComponent(ComponentType::_Collider);
    Rect intersection = A->box.GetIntersection(B->box);
    
    if (intersection.h>intersection.w) {
        //hit left or right
            left = intersection.x>A->box.x;
            right = !left;
            associated.box.x += (intersection.w + REPULSION_FACTOR)*(left?(-1):1);
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

void RBody::NotifyNoCollision(GameObject& other) {
    up = false;
    down = false;
    left = false;
    right = false;
}

void RBody::Render() {}

void RBody::Start() {}

void RBody::Update(float dt) {
    speed.y += gravity*dt;
    speed.y = (speed.y>limitspeed?limitspeed:speed.y);
    associated.box.x += speed.x*dt;
    associated.box.y += speed.y*dt;
}

bool RBody::ImpactUp() {
    return up;
}

bool RBody::ImpactDown() {
    return down;
}

bool RBody::ImpactLeft() {
    return left;
}

bool RBody::ImpactRight() {
    return right;
}

float RBody::GetLimitSpeed() {
    return limitspeed;
}
void RBody::SetLimitSpeed(float limit) {
    limitspeed = limit;
}

bool RBody::Is(ComponentType type) {
    return type & this->type;
}

bool RBody::Is(std::string type){
    return type == "RBody";
}
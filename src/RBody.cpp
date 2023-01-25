#include "Rbody.h"

RBody::RBody(GameObject& associated)
: Component(associated) {
    mass = 1;
    gravity = 9.3;
    type = ComponentType::_RigidBody;
}

RBody::~RBody() {}

//Mass
void RBody::SetMass(float mass) {
    this->mass = mass;
}

//Gravity
void RBody::SetGravity(float gravity) {
    this->gravity = gravity;
}

//Force
void RBody::ApplyForce(Vec2 force) {
    this->force = force;
}

void RBody::ApplyForceOnX(float x) {
    this->force.x = x;
}

void RBody::ApplyForceOnY(float y) {
    this->force.y = y;
}

void RBody::ResetForce() {
    this->force = Vec2(0,0);
}

//Friction
void RBody::ApplyFriction(Vec2 friction) {
    this->friction = friction;
}

void RBody::ResetFriction() {
    this->friction = Vec2(0,0);
}

//Inheritance

bool RBody::Is(ComponentType type) {
    return type & this->type;
}

bool RBody::Is(std::string type){
    return type == "RBody";
}
void RBody::NotifyCollision(GameObject& other) {
    
}

void RBody::NotifyNoCollision(GameObject& other) {
    
}

void RBody::RBody::Render() {
}

void RBody::Start() {
    
}

void RBody::Update(float dt) {
    acceleration.x = (force.x + friction.x)/mass;
    acceleration.y = gravity + force.y/mass;
    speed = acceleration * dt;
    associated.box.SetPosition((associated.box.GetPosition() + speed));
}

void RBody::LateUpdate(float dt) {
}

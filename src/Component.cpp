#include "GentooEngine.h"

Component::Component (GameObject& associated): associated(associated) {}

Component::~Component () {}

void Component::Start () {}

void Component::Update (float dt) {}

void Component::Render () {}

void Component::NotifyCollision (GameObject& other) {}

bool Component::Is (std::string type) {
    return (type == "Component");
}
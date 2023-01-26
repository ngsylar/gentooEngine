#include "GentooEngine.h"

Component::Component (GameObject& associated): associated(associated) {}

Component::~Component () {}

void Component::Awaken () {}

void Component::Start () {}

void Component::Update (float dt) {}

void Component::LateUpdate (float dt) {}

void Component::Render () {}

void Component::NotifyCollision (GameObject& other) {}

void Component::NotifyNoCollision (GameObject& other) {}

bool Component::Is (std::string type) {
    return (type == "Component");
}

bool Component::Is (ComponentType type) {
    return (type & this->type);
}
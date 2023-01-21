#include "GentooEngine.h"

GameObject::GameObject (int layer, std::string label) {
    isDead = false;
    started = false;
    angleDeg = 0.0f;
    contains = ComponentType::_None;

    this->label = label;
    this->layer = layer;
}

GameObject::~GameObject () {
    components.clear();
}

// sylar's extra layer rendering
bool GameObject::CompareLayers (
    std::weak_ptr<GameObject>& goA,
    std::weak_ptr<GameObject>& goB
) {
    return (goA.lock()->layer < goB.lock()->layer);
}

void GameObject::SetLayer (int layer) {
    this->layer = layer;
    Game::GetInstance().GetCurrentState().ScheduleSort();
}

void GameObject::Start () {
    for (int i=0; i < (int)components.size(); i++)
        components[i]->Start();
    started = true;
}

void GameObject::Update (float dt) {
    for (int i=0; i < (int)components.size(); i++) {
        components[i]->Update(dt);
    }
}

void GameObject::LateUpdate (float dt) {
    for (int i=0; i < (int)components.size(); i++) {
        components[i]->LateUpdate(dt);
    }
}

void GameObject::Render () {
    for (int i=0; i < (int)components.size(); i++) {
        components[i]->Render();
    }
}

bool GameObject::Contains (ComponentType type) {
    return (contains & type);
}

bool GameObject::IsDead () {
    return isDead;
}

void GameObject::RequestDelete () {
    isDead = true;
}

void GameObject::AddComponent (Component* cpt) {
    if (cpt == nullptr)
        return;

    components.emplace_back(cpt);
    contains = contains | cpt->type; //Bitwise flag of contained object registered

    if (started)
        cpt->Start();
}

void GameObject::RemoveComponent (Component* cpt) {
    if (cpt == nullptr)
        return;

    for (int i=((int)components.size())-1; i >= 0; i--) {
        if (components[i].get() == cpt) {
            contains = contains ^ cpt->type;
            components.erase(components.begin()+i);
        }
    }
}

Component* GameObject::GetComponent (std::string type) {
    for (int i=0; i < (int)components.size(); i++) {
        if (components[i]->Is(type))
            return components[i].get(); 
    }
    return nullptr;
}

Component* GameObject::GetComponent (ComponentType type) {
    
    if(contains & type) {
        for (int i=0; i < (int)components.size(); i++) {
            if (components[i]->Is(type))
                return components[i].get();
        }
    }
    return nullptr;
}

void GameObject::NotifyCollision (GameObject& other) {
    for (int i=0; i < (int)components.size(); i++) {
        components[i]->NotifyCollision(other);
    }
}

void GameObject::NotifyNoCollision (GameObject& other) {
    for (int i=0; i < (int)components.size(); i++) {
        components[i]->NotifyNoCollision(other);
    }
}
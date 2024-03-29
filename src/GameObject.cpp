#include "GentooEngine.h"

GameObject::GameObject (int layer, std::string label) {
    enabled = true;
    isDead = false;
    awake = false;
    angleDeg = 0.0f;
    contains = ComponentType::_None;

    this->label = label;
    this->layer = layer;

    // melius colliders' pixel correction
    pixelColliderFix0 = 0;
    pixelColliderFix1 = 0;
}

GameObject::~GameObject () {
    components.clear();
}

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

void GameObject::Awaken () {
    for (int i=0; i < (int)components.size(); i++)
        components[i]->Awaken();
    awake = true;
}

void GameObject::Start () {
    for (int i=0; i < (int)components.size(); i++)
        components[i]->Start();
}

void GameObject::Update (float dt) {
    if (not enabled) return;
    for (int i=0; i < (int)components.size(); i++)
        components[i]->Update(dt);
}

void GameObject::LateUpdate (float dt) {
    if (not enabled) return;
    for (int i=0; i < (int)components.size(); i++)
        components[i]->LateUpdate(dt);
}

void GameObject::Render () {
    if (not enabled) return;
    for (int i=0; i < (int)components.size(); i++)
        components[i]->Render();
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

    if (awake) {
        cpt->Awaken();
        cpt->Start();
    }
}

void GameObject::RemoveComponent (Component* cpt) {
    if (cpt == nullptr)
        return;

    for (int i=((int)components.size())-1; i >= 0; i--)
        if (components[i].get() == cpt) {
            contains = contains ^ cpt->type;
            components.erase(components.begin()+i);
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
    for (int i=0; i < (int)components.size(); i++)
        components[i]->NotifyCollision(other);
}

void GameObject::NotifyNoCollision (GameObject& other) {
    for (int i=0; i < (int)components.size(); i++)
        components[i]->NotifyNoCollision(other);
}

void GameObject::SignalTerrain () {
    contains = contains | ComponentType::_Terrain;
}
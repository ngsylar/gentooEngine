#include "GentooEngine.h"
#include "EntityMachine.h"

EntityMachine::EntityMachine (GameObject& associated): Component(associated) {
    type = ComponentType::_EntityMachine;
    state = EntityState::None;
    
    sprites.resize(EntityState::__TOTAL);
    for(int i=0; i < EntityState::__TOTAL; i++)
        sprites[i] = nullptr;

    textureFlip = SDL_FLIP_NONE;
}

EntityMachine::~EntityMachine () {
    sprites.clear();
}

void EntityMachine::AddSpriteState (EntityState newState, Sprite* newSheet) {
    if (sprites[newState].get() != nullptr) {
        SDL_Log("EntityMachine::AddEntityState: EntityState already exists");
        return;
    } sprites[newState] = std::unique_ptr<Sprite>(newSheet);
}

void EntityMachine::RemoveState (EntityState state) {
    if(sprites[state] == nullptr) {
        SDL_Log("EntityMachine::RemoveEntityState: Inexistent EntityState");
        return;
    } sprites[state].reset();
}

bool EntityMachine::HasState (EntityState compare) {
    return (sprites[compare].get() != nullptr);
}

void EntityMachine::SetState (EntityState newState) {
    state = newState;
}

EntityState EntityMachine::GetCurrentState () {
    return state;
}

void EntityMachine::Awaken () {
    AwakenEntity();
}

void EntityMachine::Start () {
    StartEntity();
}

void EntityMachine::Update (float dt) {
    if ((state == EntityState::None) or (sprites[state].get() == nullptr))
        return;
    sprites[state].get()->Update(dt);
    UpdateEntity(dt);
}

void EntityMachine::LateUpdate (float dt) {
    LateUpdateEntity(dt);
}

void EntityMachine::Render () {
    if ((state == EntityState::None) or (sprites[state].get() == nullptr))
        return;
    Sprite* spriteState = sprites[state].get();
    spriteState->textureFlip = textureFlip;
    spriteState->Render();
    RenderEntity();
}

//Inheritance Functions
void EntityMachine::AwakenEntity () {}

void EntityMachine::StartEntity () {}

void EntityMachine::UpdateEntity (float dt) {}

void EntityMachine::LateUpdateEntity (float dt) {}

void EntityMachine::RenderEntity () {}

void EntityMachine::NotifyCollision (GameObject& other) {}

void EntityMachine::NotifyNoCollision (GameObject& other) {}

bool EntityMachine::Is (ComponentType type) {
    return (type & this->type);
}
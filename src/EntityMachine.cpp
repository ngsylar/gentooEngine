#include "GentooEngine.h"
#include "EntityMachine.h"

EntityMachine::EntityMachine (GameObject& associated): Component(associated) {
    type = ComponentType::_EntityMachine;
    state = EntityState::None;
    
    sprites.resize(EntityState::_statesCount);
    for(int i=0; i < EntityState::_statesCount; i++)
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

bool EntityMachine::FormatState (EntityState newState, int argsc, float argsv[]) {
    if (NewStateRule(newState, argsc, argsv)) {
        state = newState;
        return true;
    } return false;
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
    EntityState previousState = state;

    if ((state == EntityState::None) or (sprites[state].get() == nullptr))
        return;
    sprites[state].get()->Update(dt);

    UpdateEntity(dt);

    if (state != previousState)
        sprites[state].get()->SetFrame(0);
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

bool EntityMachine::NewStateRule (EntityState newState, int argsc, float argsv[]) {
    return true;
}

void EntityMachine::FlipSprite (Sprite::Axis axis) {
    SDL_RendererFlip flip = (axis == Sprite::HORIZONTAL) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_VERTICAL;
    textureFlip = SDL_RendererFlip(textureFlip ^ flip);
}

void EntityMachine::NotifyCollision (GameObject& other) {}

void EntityMachine::NotifyNoCollision (GameObject& other) {}

bool EntityMachine::Is (ComponentType type) {
    return (type & this->type);
}
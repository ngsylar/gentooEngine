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
        sprites[state].get()->SetFrame(0);
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
    // EntityState previousState = state;

    if ((state == EntityState::None) or (sprites[state].get() == nullptr))
        return;

    UpdateEntity(dt);

    sprites[state].get()->Update(dt);

    // if (state != previousState)
    //     sprites[state].get()->SetFrame(0);

    int directionX = (textureFlip ^ SDL_FLIP_HORIZONTAL)? 0 : 1;
    int directionY = (textureFlip ^ SDL_FLIP_VERTICAL)? 0 : 1;

    Collider* collider = (Collider*)associated.GetComponent(ComponentType::_Collider);
    if (collider != nullptr) {

        if (directionX != lastDirection.x) {
            collider->offset.x = -collider->offset.x;
            associated.box.x -= collider->offset.x * 2.0f;
            lastDirection.x = directionX;
        }
        if (directionY != lastDirection.y) {
            collider->offset.y = -collider->offset.y;
            associated.box.y -= collider->offset.y * 2.0f;
            lastDirection.y = directionY;
        }
    }
    // melius colliders' pixel correction
    associated.pixelColliderFix0 = lastDirection.x;
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

bool EntityMachine::SpriteIsFlipped () {
    return ((textureFlip & SDL_FLIP_HORIZONTAL)? true : false);
}

void EntityMachine::NotifyCollision (GameObject& other) {}

void EntityMachine::NotifyNoCollision (GameObject& other) {}

bool EntityMachine::Is (ComponentType type) {
    return (type & this->type);
}
#include "GentooEngine.h"
#include "EntityMachine.h"

EntityMachine::EntityMachine (GameObject& associated) 
: Component(associated) {
    entitySprite.reserve(EntityState::__TOTAL);
    entitySprite.reserve(EntityState::__TOTAL);
    for(int i = 0; i<EntityState::__TOTAL; i++)
    {
        entitySprite[i] = nullptr;
    }
    currState = EntityState::None;
    this->type = ComponentType::_EntityMachine;
}

EntityMachine::~EntityMachine () {
    for(int i = 0; i<EntityState::__TOTAL; i++)
    {
        if(entitySprite[i] != nullptr) {
            delete entitySprite[i];
        }
    }
    entitySprite.clear();
}

void EntityMachine::AddEntityStateSprite (EntityState NewState, Sprite* NewSheet) {
    if (entitySprite[NewState] != nullptr) {
        return;
        SDL_Log("EntityMachine::AddEntityState: State already exists");
    }
    entitySprite[NewState] = NewSheet;
}

void EntityMachine::RemoveEntityState (EntityState DelState) {
    if(entitySprite[DelState] == nullptr) {
        return;
    }
    delete entitySprite[DelState];
    entitySprite[DelState] = nullptr;
}

bool EntityMachine::HasEntityState(EntityState Compare) {
    return (entitySprite[Compare] != nullptr);
}

void EntityMachine::SetEntityState (EntityState NewState) {
    currState  = NewState;
}
EntityState EntityMachine::GetCurrEntityState () {
    return currState;
}

//Inheritance Functions
void EntityMachine::StartEntity() {}

void EntityMachine::UpdateEntity(float dt) {}

void EntityMachine::LateUpdateEntity(float dt) {}

void EntityMachine::RenderEntity() {}

void EntityMachine::Start() {
    for(int i = 0; i<EntityState::__TOTAL; i++) {
        if(entitySprite[i] != nullptr) {
            entitySprite[i]->Start();
        }
    }
    StartEntity();
}

void EntityMachine::Update(float dt) {
    if (currState == EntityState::None or entitySprite[currState] == nullptr) {
        return;
    }
    entitySprite[currState]->Update(dt);
    UpdateEntity(dt);
}

void EntityMachine::LateUpdate(float dt) {
    if (currState == EntityState::None or entitySprite[currState] == nullptr) {
        return;
    }
    entitySprite[currState]->LateUpdate(dt);
    LateUpdateEntity(dt);
}

void EntityMachine::Render() {
    if (currState == EntityState::None or entitySprite[currState] == nullptr) {
        return;
    }
    entitySprite[currState]->Render();
    RenderEntity();
}

void EntityMachine::NotifyCollision(GameObject& other) {    SDL_Log("Pipa3\n");}

void EntityMachine::NotifyNoCollision(GameObject& other) {    SDL_Log("Pipa4\n");}

bool EntityMachine::Is(ComponentType type) {
    return type & this->type;
}
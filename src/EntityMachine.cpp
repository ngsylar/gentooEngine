#include "GentooEngine.h"
#include "EntityMachine.h"

EntityMachine::EntityMachine (GameObject& associated) 
: Component(associated) {
    entitySprite.resize(EntityState::__TOTAL);
    for(int i = 0; i<EntityState::__TOTAL; i++)
    {
        entitySprite[i] = nullptr;
    }
    currState = EntityState::None;
    this->type = ComponentType::_EntityMachine;
}

EntityMachine::~EntityMachine () {
    entitySprite.clear();
}

void EntityMachine::AddEntityStateSprite (EntityState NewState, Sprite* NewSheet) {
    if (entitySprite[NewState].get() != nullptr) {
        return;
        SDL_Log("EntityMachine::AddEntityState: EntityState already exists");
    }
    entitySprite[NewState] = std::unique_ptr<Sprite>(NewSheet);
}

void EntityMachine::RemoveEntityState (EntityState DelState) {
    if(entitySprite[DelState] == nullptr) {
        SDL_Log("EntityMachine::RemoveEntityState: Inexistent EntityState");
        return;
    }
    entitySprite[DelState].reset();
}

bool EntityMachine::HasEntityState(EntityState Compare) {
    return (entitySprite[Compare].get() != nullptr);
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
        if(entitySprite[i].get() != nullptr) {
            entitySprite[i].get()->Start();
        }
    }
    StartEntity();
}

void EntityMachine::Update(float dt) {
    if (currState == EntityState::None or entitySprite[currState].get() == nullptr) {
        return;
    }
    entitySprite[currState].get()->Update(dt);
    UpdateEntity(dt);
}

void EntityMachine::LateUpdate(float dt) {
    if (currState == EntityState::None or entitySprite[currState].get() == nullptr) {
        return;
    }
    entitySprite[currState].get()->LateUpdate(dt);
    LateUpdateEntity(dt);
}

void EntityMachine::Render() {
    if (currState == EntityState::None or entitySprite[currState].get() == nullptr) {
        return;
    }
    entitySprite[currState].get()->Render();
    RenderEntity();
}

void EntityMachine::NotifyCollision(GameObject& other) { }

void EntityMachine::NotifyNoCollision(GameObject& other) { }

bool EntityMachine::Is(ComponentType type) {
    return type & this->type;
}
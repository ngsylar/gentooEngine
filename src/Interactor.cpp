#include "Interactor.h"
#include "DialogueBox.h"

Interactor::Interactor(GameObject& associated, bool selfDestruct, bool notSolid, ComponentType interactWith)
: Component(associated) {
    type = ComponentType::_Interactor;
    Condition = nullptr;
    Result = nullptr;
    this->selfDestruct = selfDestruct;
    this->interactWith = interactWith;
    Collider* interact = new Collider(associated, notSolid);
    associated.AddComponent(interact);
}

Interactor::~Interactor() {}

void Interactor::SetCondition(bool(*Condition)()) {
    this->Condition = Condition;
}

void Interactor::SetResult(void(*Result)()) {
    this->Result = Result;
}

bool Interactor::Is(ComponentType type) {
    return (this->type & type);
}

void Interactor::NotifyCollision(GameObject& other) {
    if(!other.Contains(interactWith) or (Condition != nullptr and !Condition()) 
        or DialogueBox::dialogueOngoing) {
        return;
    }
    if(Result != nullptr) {
        Result();
    } else {
        SDL_Log("Interactor: Result function not set!");
    }
    if(selfDestruct) {
        associated.RequestDelete();
    }
}
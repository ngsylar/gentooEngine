#include "Dummy.h"
#include "GentooEngine.h"

#define DUMMY_IDLE "assets/img/Dummy/DummyIdle.png"
#define DUMMY_WALK "assets/img/Dummy/DummyWalk.png"
#define DUMMY_RUN "assets/img/Dummy/DummyRun.png"


Dummy::Dummy (GameObject& associated) 
: EntityMachine(associated) {
    HP = 100;
}
// Dummy::~Dummy () {}

void Dummy::StartEntity() {

    Sprite* Idle = new Sprite(associated, DUMMY_IDLE, 4, 0.2);
    Sprite* Walk = new Sprite(associated, DUMMY_WALK, 5, 0.2);
    Sprite* Run = new Sprite(associated, DUMMY_RUN, 5, 0.2);

    AddEntityStateSprite(EntityState::Idle, Idle);
    AddEntityStateSprite(EntityState::Walking, Walk);
    AddEntityStateSprite(EntityState::Running, Run);
    currState = EntityState::Idle;
    associated.box.SetSize(Idle->GetWidth(), Idle->GetHeight());
}

void Dummy::UpdateEntity(float dt) {

    InputManager& input = InputManager::GetInstance();

    switch (currState)
    {
        case EntityState::Idle:
            if(input.IsKeyDown(KEY_S)) {
                currState = EntityState::Running;
            }
            else if(input.IsKeyDown(KEY_A)) {
                currState = EntityState::Walking;
            }

            break;
        case EntityState::Walking:
            if(input.KeyRelease(KEY_A)){
                currState = EntityState::Idle;
            }

            break;
        case EntityState::Running:
            if(input.KeyRelease(KEY_S)){
                currState = EntityState::Idle;
            }

            break;
        default:
            break;
    }
}

// void Dummy::LateUpdate(float dt) {}

// void Dummy::NotifyCollision(GameObject& other) {}

// void Dummy::NotifyNoCollision(GameObject& other) {}

// bool Dummy::Is(ComponentType type){
//     return type & this->type;
// }
#include "Dummy.h"
#include "RBody.h"
#include "Collider.h"
#include "GentooEngine.h"

#define DUMMY_IDLE "assets/img/Dummy/idle.png"
#define DUMMY_WALK "assets/img/Dummy/walk.png"
#define DUMMY_RUN "assets/img/Dummy/run.png"
#define DUMMY_JUMP "assets/img/Dummy/jump.png"
#define DUMMY_FALL "assets/img/Dummy/fall.png"
#define DUMMY_ATTACK "assets/img/Dummy/attack.png"


Dummy::Dummy (GameObject& associated) 
: EntityMachine(associated) {
    HP = 100;
    lastDirection = 1;
    jumpDecrease = 0;
    jumpLimit.SetResetTime(.8); 
    ceiling = false;
}

void Dummy::StartEntity() {
    Sprite* Idle = new Sprite(associated, DUMMY_IDLE, 4, 0.2);
    Sprite* Walk = new Sprite(associated, DUMMY_WALK, 6, 0.15);
    Sprite* Run = new Sprite(associated, DUMMY_RUN, 6, 0.08);
    Sprite* Jump = new Sprite(associated, DUMMY_JUMP, 4, 0.08, true);
    Sprite* Fall = new Sprite(associated, DUMMY_FALL, 2, 0.3, true);
    Sprite* Attack = new Sprite(associated, DUMMY_ATTACK, 5, 0.12, true);

    AddEntityStateSprite(EntityState::Idle, Idle);
    AddEntityStateSprite(EntityState::Walking, Walk);
    AddEntityStateSprite(EntityState::Running, Run);
    AddEntityStateSprite(EntityState::Jumping, Jump);
    AddEntityStateSprite(EntityState::Falling, Fall);
    AddEntityStateSprite(EntityState::Attacking, Attack);

    RBody* Rig = new RBody(associated);
    Collider* Col = new Collider(associated);

    GameObject* CamBoxObj = new GameObject(associated.layer);
    CameraBox* CamBox = new CameraBox(*CamBoxObj, &associated);

    associated.AddComponent(Rig);
    associated.AddComponent(Col);
    CamBoxObj->AddComponent(CamBox);
    Game::GetInstance().GetCurrentState().AddObject(CamBoxObj);
    Camera::Follow(CamBoxObj);

    currState = EntityState::Idle;
    associated.box.SetSize(Idle->GetWidth(), Idle->GetHeight());
    Col->box.SetPosition(associated.box.GetGlobalCenter());
    Col->SetBox(Vec2(0,4), Vec2(16,26));
}

#define JUMP_SPD 200
#define JUMPDEC_FACTOR 200
#define WALK_SPD 60
#define RUN_SPD 120
#define AIR_SPD 100

void Dummy::UpdateEntity(float dt) {
    jumpLimit.Update(dt);
    InputManager& input = InputManager::GetInstance();
    RBody* Rigid = (RBody*)associated.GetComponent(ComponentType::_RBody);
    int XDirection  = input.IsKeyDown(KEY_D) - input.IsKeyDown(KEY_A);
    
    if(Rigid->GetSpeed().y > 100) { //Fall always starts from certain speed downwards
        currState = EntityState::Falling;
    }
    switch (currState) {
        case EntityState::Idle:
            if(XDirection != 0) { //Changing to walk or run
                if(input.IsKeyDown(SDLK_LSHIFT)){
                    currState = EntityState::Running;
                    entitySprite[currState].get()->SetFrame(0);
                } else {
                    currState = EntityState::Walking;
                    entitySprite[currState].get()->SetFrame(0);
                }                
            }

            //Jump trigger
            if (input.KeyPress(KEY_SPACE)){
                currState = EntityState::Jumping;
                Rigid->SetGravity(0);
                jumpLimit.Reset();
            }
            Rigid->SetSpeedOnX(0);
            break;

        case EntityState::Walking:
            if(input.KeyPress(SDLK_LSHIFT)) {//Start to run
                currState = EntityState::Running;

            } else if(XDirection == 0){//go to idle
                Rigid->SetSpeedOnX(0);
                currState = EntityState::Idle;

            } else {//Keep walking
                Rigid->SetSpeedOnX(XDirection*WALK_SPD);
            }

            //Jump trigger
            if (input.KeyPress(KEY_SPACE)){
                currState = EntityState::Jumping;
                Rigid->SetGravity(0);
                jumpLimit.Reset();
            }
            break;

        case EntityState::Running:
            if(XDirection == 0){//Back to idle
                Rigid->SetSpeedOnX(0);
                currState = EntityState::Idle;

            } else if(input.KeyRelease(SDLK_LSHIFT)) {//Start to walk
                currState = EntityState::Walking;
            } else {
                    Rigid->SetSpeedOnX(XDirection*RUN_SPD);
            }
            
            //Jump trigger
            if (input.KeyPress(KEY_SPACE)){
                currState = EntityState::Jumping;
                Rigid->SetGravity(0);
                jumpLimit.Reset();
            }
            break;

        case EntityState::Jumping:
            Rigid->SetSpeedOnY(-JUMP_SPD+jumpDecrease);//Jump base speed
            jumpDecrease+= JUMPDEC_FACTOR*dt;//decrease rate grows

            //Limitant factors to jump
            if(input.KeyRelease(KEY_SPACE) or jumpLimit.IsOver() or ceiling){
                jumpDecrease = 0;
                Rigid->ResetGravity();
                Rigid->SetSpeedOnY(0);
                jumpLimit.Reset();
                ceiling = false;
                currState = EntityState::Falling;
            }

            if(XDirection != 0) { //Movement while jumping
                Rigid->SetSpeedOnX(XDirection*AIR_SPD);
            }
            break;
        
        //The fall of man
        case EntityState::Falling:
            if(XDirection != 0) { //Movement while on air
                Rigid->SetSpeedOnX(XDirection*AIR_SPD);
            } else { //Breaks on x axis
                Rigid->SetSpeedOnX(0);
            }
            break;

        default:
            break;
    }

    lastDirection = (XDirection!=0?XDirection:lastDirection);
    if((lastDirection == -1 and entitySprite[currState].get()->textureFlip == SDL_FLIP_NONE)
        or (lastDirection == 1 and entitySprite[currState].get()->textureFlip == SDL_FLIP_HORIZONTAL)) {
        entitySprite[currState].get()->FlipTexture(Sprite::TextureFlipper::HORIZONTAL);
    }
}

// void Dummy::LateUpdate(float dt) {}

void Dummy::NotifyCollision(GameObject& other) {
    RBody* Rigid = (RBody*)associated.GetComponent(ComponentType::_RBody);
    Rigid->NotifyCollision(other);
        
    if(Rigid->ImpactDown() and currState == EntityState::Falling) {
        currState = EntityState::Idle;
    } 
    else if(Rigid->ImpactUp() and currState == EntityState::Jumping) {
        ceiling = true;
    }
}

// void Dummy::NotifyNoCollision(GameObject& other) {}

// bool Dummy::Is(ComponentType type){
//     return type & this->type;
// }

// void Dummy::Attack() {
// }

// void Dummy::Run() {
// }

// void Dummy::Walk() {
// }

// void Dummy::Idle() {
// }

// void Dummy::Jump() {
// }

// void Dummy::Fall() {
// }

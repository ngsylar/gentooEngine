#include "GentooEngine.h"
#include "Dummy.h"

#define DUMMY_IDLE "assets/img/Dummy/idle.png"
#define DUMMY_WALK "assets/img/Dummy/walk.png"
#define DUMMY_RUN "assets/img/Dummy/run.png"
#define DUMMY_JUMP "assets/img/Dummy/jump.png"
#define DUMMY_FALL "assets/img/Dummy/fall.png"
#define DUMMY_ATTACK "assets/img/Dummy/attack.png"

Dummy::Dummy (GameObject& associated): EntityMachine(associated) {
    type = ComponentType::_Dummy;
    HP = 100;
    lastDirection = 1;
    increaseSpd = 0;
    jumpDecrease = 0;
    jumpLimit.SetResetTime(.8); 
    ceiling = false;
}

void Dummy::StartEntity() {
    Sprite* Idle = new Sprite(associated, DUMMY_IDLE, 4, 0.2);
    Sprite* Walk = new Sprite(associated, DUMMY_WALK, 6, 0.15);
    Sprite* Run = new Sprite(associated, DUMMY_RUN, 6, 0.08);
    Sprite* Jump = new Sprite(associated, DUMMY_JUMP, 4, 0.15, true);
    Sprite* Fall = new Sprite(associated, DUMMY_FALL, 2, 0.2, true);
    Sprite* Attack = new Sprite(associated, DUMMY_ATTACK, 5, 0.12, true);

    AddSpriteState(EntityState::Idle, Idle);
    AddSpriteState(EntityState::Walking, Walk);
    AddSpriteState(EntityState::Running, Run);
    AddSpriteState(EntityState::Jumping, Jump);
    AddSpriteState(EntityState::Falling, Fall);
    AddSpriteState(EntityState::Attacking, Attack);

    RigidBody* Rig = new RigidBody(associated);
    Collider* Col = new Collider(associated);

    GameObject* CamBoxObj = new GameObject(associated.layer);
    CameraBox* CamBox = new CameraBox(*CamBoxObj, &associated);

    associated.AddComponent(Rig);
    associated.AddComponent(Col);
    CamBoxObj->AddComponent(CamBox);
    Game::GetInstance().GetCurrentState().AddObject(CamBoxObj);
    Camera::Follow(CamBoxObj);

    state = EntityState::Idle;
    associated.box.SetSize(Idle->GetWidth(), Idle->GetHeight());
    Col->box.SetPosition(associated.box.GetGlobalCenter());
    Col->SetBox(Vec2(0,3), Vec2(16,26));
}

#define JUMP_SPD 220
#define JUMPDEC_FACTOR 200
#define WALK_SPD 80
#define RUN_SPD 150
#define AIR_SPD 120

#define WALK_ACCEL 100
#define RUN_ACCEL 200

void Dummy::UpdateEntity(float dt) {
    EntityState now = state;

    jumpLimit.Update(dt);
    InputManager& input = InputManager::GetInstance();
    RigidBody* Rigid = (RigidBody*)associated.GetComponent(ComponentType::_RigidBody);
    
    //now taking controller and keyboard commands
    int XDirection  = input.GetAxisMotion(STICK_AXIS_LX);
    XDirection == 0 ? XDirection = input.IsControllerDown(JOY_RIGHT) - input.IsControllerDown(JOY_LEFT): XDirection;
    XDirection == 0 ? XDirection = input.IsKeyDown(KEY_D) - input.IsKeyDown(KEY_A) : XDirection;

    if((state != EntityState::Falling) and Rigid->GetSpeed().y > 100) { //Fall always starts from certain speed downwards
        state = EntityState::Falling;
    }
    switch (state) {
        case EntityState::Idle:
            if(XDirection != 0) { //Changing to walk or run
                if(input.IsControllerDown(JOY_R1) or input.IsKeyDown(SDLK_LSHIFT)){
                    state = EntityState::Running;
                    sprites[state].get()->SetFrame(0);
                } else {
                    state = EntityState::Walking;
                    sprites[state].get()->SetFrame(0);
                }                
            }

            //Jump trigger
            if (input.ControllerPress(JOY_A) or input.KeyPress(KEY_SPACE)){
                state = EntityState::Jumping;
                Rigid->SetGravity(0);
                jumpLimit.Reset();
            }
            Rigid->SetSpeedOnX(0);
            break;

        case EntityState::Walking:
            if(input.ControllerPress(JOY_R1) or input.KeyPress(SDLK_LSHIFT)) {//Start to run
                state = EntityState::Running;

            } else if(XDirection == 0){//go to idle
                Rigid->SetSpeedOnX(0);
                state = EntityState::Idle;
                increaseSpd = 0;
            } else {//Keep walking
                if(increaseSpd<WALK_SPD) {
                    increaseSpd+=WALK_ACCEL*dt;
                }
                Rigid->SetSpeedOnX(XDirection* SmoothStep(0, WALK_SPD, increaseSpd) *WALK_SPD);
            }

            //Jump trigger
            if (input.ControllerPress(JOY_A) or input.KeyPress(KEY_SPACE)){
                state = EntityState::Jumping;
                Rigid->SetGravity(0);
                jumpLimit.Reset();
            }
            break;

        case EntityState::Running:
            if(XDirection == 0){//Back to idle
                Rigid->SetSpeedOnX(0);
                state = EntityState::Idle;
                increaseSpd = 0;

            } else if(input.ControllerRelease(JOY_R1) or input.KeyRelease(SDLK_LSHIFT)) {//Start to walk
                state = EntityState::Walking;
            } else {
                    Rigid->SetSpeedOnX(XDirection* SmoothStep(0, RUN_SPD, increaseSpd) * RUN_SPD);
                    if(increaseSpd<RUN_SPD) {
                        increaseSpd+=RUN_ACCEL*dt;
                    }
            }
            
            //Jump trigger
            if (input.ControllerPress(JOY_A) or input.KeyPress(KEY_SPACE)){
                state = EntityState::Jumping;
                Rigid->SetGravity(0);
                jumpLimit.Reset();
            }
            break;

        case EntityState::Jumping:
            Rigid->SetSpeedOnY(-JUMP_SPD+jumpDecrease);//Jump base speed
            jumpDecrease+= JUMPDEC_FACTOR*dt;//decrease rate grows

            //Limitant factors to jump
            if(input.ControllerRelease(JOY_A) or input.KeyRelease(KEY_SPACE) or jumpLimit.IsOver() or ceiling){
                jumpDecrease = 0;
                Rigid->ResetGravity();
                Rigid->SetSpeedOnY(0);
                jumpLimit.Reset();
                ceiling = false;
                state = EntityState::Falling;
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

    if(now != state) {
        sprites[state].get()->SetFrame(0);
    }

    lastDirection = (XDirection!=0?XDirection:lastDirection);
    if((lastDirection == -1 and sprites[state].get()->textureFlip == SDL_FLIP_NONE)
        or (lastDirection == 1 and sprites[state].get()->textureFlip == SDL_FLIP_HORIZONTAL)) {
        FlipSprite(Sprite::HORIZONTAL);
    }
}

// void Dummy::LateUpdate(float dt) {}

void Dummy::NotifyCollision(GameObject& other) {
    RigidBody* Rigid = (RigidBody*)associated.GetComponent(ComponentType::_RigidBody);
    Rigid->NotifyCollision(other);
        
    if(Rigid->ImpactDown() and state == EntityState::Falling) {
        state = EntityState::Idle;
    } 
    else if(Rigid->ImpactUp() and state == EntityState::Jumping) {
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

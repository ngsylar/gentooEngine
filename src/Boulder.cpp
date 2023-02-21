#include "GentooEngine.h"
#include "Boulder.h"
#include "GameData.h"
#include "Kid.h"

#define SPRITE_RUN_FRAMES       6, 0.1f
#define SPRITE_DAMAGE_FRAMES    2, 0.1f
#define SPRITE_DEATH_FRAMES     8, 0.1f

#define SPEED_RUN               60.0f
#define IMPULSE_MASS            10.0f

#define ATTACK_FORCE            400.0f, 140.0f
#define ATTACK_IMPULSE          50.0f
#define ATTACK_DAMAGE           1

#define COLLIDER_POSITION       0.0f, 14.0f
#define COLLIDER_BOX_SIZE       21.0f, 19.0f

Boulder::Boulder (GameObject& associated, std::string file, Vec2 position): EntityMachine(associated) {
    type = ComponentType::_Boulder;
    Sprite* sprite = new Sprite(associated, file);
    AddSpriteState(EntityState::Idle, sprite);
    sprite = new Sprite(associated, file);
    AddSpriteState(EntityState::Running, sprite);
    sprite = new Sprite(associated, file);
    AddSpriteState(EntityState::Falling, sprite);
    sprite = new Sprite(associated, file);
    AddSpriteState(EntityState::Dead, sprite);
    associated.box.SetSize(sprites[EntityState::Idle].get()->GetWidth(), sprites[EntityState::Idle].get()->GetHeight());
    associated.box.SetPurePosition(position*TILE_SIZE);
    movementDirection = 0;
}

void Boulder::Awaken () {
    rigidBody = new RigidBody(associated);
    associated.AddComponent(rigidBody);
    // rigidBody->triggerLabels.push_back("Player");
    // rigidBody->triggerLabels.push_back("Enemy");
    // rigidBody->triggerLabels.push_back("Boss");

    collider = new Collider(associated);
    associated.AddComponent(collider);
    collider->SetBox(Vec2(0,0), Vec2(associated.box.w-10, associated.box.h-5));
}

void Boulder::Start () {
    state = EntityState::Idle;
    // sprites[state].get()->SetFrame(0);
    // rigidBody->SetSpeedOnX(SPEED_RUN * movementDirection);
}

void Boulder::LateUpdate (float dt) {}

#define FALL_XSPEED 10
#define FALL_TRIGGER 100
#define THRUST_XSPEED 80
void Boulder::UpdateEntity (float dt) {
    if (state == EntityState::Dead) {
        return;

    } else if(rigidBody->GetSpeed().y>FALL_TRIGGER) {
        state = EntityState::Falling;
            rigidBody->SetSpeedOnX(FALL_XSPEED*movementDirection);       
    } else if (state == EntityState::Running) {
        rigidBody->SetSpeedOnX(THRUST_XSPEED*movementDirection);
    }
}


void Boulder::NotifyCollision (GameObject& other) {
    
    if(state != EntityState::Dead and GameData::canUseChargedAttack) {
        rigidBody->NotifyCollision(other);
        Collider* A = (Collider*)associated.GetComponent(ComponentType::_Collider);
        // Collider* B = (Collider*)other.GetComponent(ComponentType::_Collider);
        // Rect intersection = A->box.GetIntersection(B->box);
        if(other.Contains(ComponentType::_Attack) and Kid::self->GetCurrentState() == EntityState::AttackingSwordStrong_Perform){
            int direction = (Kid::GetInstance()->box.x > A->box.x?(-1):1);
            state = EntityState::Running;
            rigidBody->SetSpeedOnX(THRUST_XSPEED*direction);
            movementDirection = direction;
        } else if(other.Contains(ComponentType::_Interactor)) {
            state= EntityState::Dead;
            rigidBody->SetSpeedOnX(0);
        } 
    }
}
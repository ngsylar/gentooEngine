#include "GentooEngine.h"
#include "EnemyRunner.h"
#include "AttackGeneric.h"

#define SPRITE_IDLE                 "assets/img/monster1/idle.png"
#define SPRITE_WALK                 "assets/img/monster1/walk.png"
#define SPRITE_RUN_ATTACK           "assets/img/monster1/run_attack.png"
#define SPRITE_DAMAGE               "assets/img/monster1/damage.png"
#define SPRITE_DEATH                "assets/img/monster1/death.png"

#define SPRITE_IDLE_FRAMES          12, 0.1f
#define SPRITE_WALK_FRAMES          8, 0.1f
#define SPRITE_RUN_ATTACK_FRAMES    6, 0.1f
#define SPRITE_DAMAGE_FRAMES        4, 0.1f
#define SPRITE_DEATH_FRAMES         13, 0.1f

#define SPEED_WALK                  60.0f
#define IMPULSE_MASS                10.0f

#define ATTACK_FORCE                400.0f, 140.0f
#define ATTACK_IMPULSE              50.0f
#define ATTACK_DAMAGE               1

#define COLLIDER_POSITION           -1.0f, 2.0f
#define COLLIDER_BOX_SIZE           16.0f, 39.0f

EnemyRunner::EnemyRunner (GameObject& associated): EntityMachine(associated) {
    type = type | ComponentType::_EnemyRunner;
    associated.label = "Enemy";
    movementDirection = 1;
    hp = 3;

    turnTimer.SetResetTime(0.4f);
    isGrounded = false;
    hitWall = false;

    // melius colliders' pixel correction
    associated.pixelColliderFix1 = true;
}

void EnemyRunner::Awaken () {
    Sprite* spriteIdle = new Sprite(associated, SPRITE_IDLE, SPRITE_IDLE_FRAMES);
    Sprite* spriteWalk = new Sprite(associated, SPRITE_WALK, SPRITE_WALK_FRAMES);
    Sprite* spriteRun = new Sprite(associated, SPRITE_RUN_ATTACK, SPRITE_RUN_ATTACK_FRAMES);
    Sprite* spriteFall = new Sprite(associated, SPRITE_DAMAGE, SPRITE_DAMAGE_FRAMES, true);
    Sprite* spriteDamage = new Sprite(associated, SPRITE_DAMAGE, SPRITE_DAMAGE_FRAMES, true);
    Sprite* spriteDeath = new Sprite(associated, SPRITE_DEATH, SPRITE_DEATH_FRAMES, true);

    AddSpriteState(EntityState::Idle, spriteIdle);
    AddSpriteState(EntityState::Walking, spriteWalk);
    AddSpriteState(EntityState::Running, spriteRun);
    AddSpriteState(EntityState::Falling, spriteFall);
    AddSpriteState(EntityState::Injured, spriteDamage);
    AddSpriteState(EntityState::Dead, spriteDeath);

    rigidBody = new RigidBody(associated);
    associated.AddComponent(rigidBody);
    rigidBody->triggerLabels.push_back("Player");
    rigidBody->triggerLabels.push_back("Enemy");

    collider = new Collider(associated);
    associated.AddComponent(collider);
    collider->SetBox(Vec2(COLLIDER_POSITION), Vec2(COLLIDER_BOX_SIZE));

    AttackGeneric* attack = new AttackGeneric(associated);
    attack->SetProperties(Vec2(ATTACK_FORCE), ATTACK_IMPULSE, ATTACK_DAMAGE);
    associated.AddComponent(attack);
}

void EnemyRunner::Start () {
    state = EntityState::Walking;
    sprites[state].get()->SetFrame(0);
    rigidBody->SetSpeedOnX(SPEED_WALK * movementDirection);
}

void EnemyRunner::LateUpdate (float dt) {}

void EnemyRunner::UpdateEntity (float dt) {
    if (isGrounded and (rigidBody->GetSpeed().y > 100))
        isGrounded = false;

    switch (state) {
        case EntityState::Walking:
            if (isGrounded) {
                bool foundEdgeLeft = collider->box.x < currentRoute.x;
                bool foundEdgeRight = collider->box.x+collider->box.w > currentRoute.y;
                turnTimer.Update(dt);

                if ((foundEdgeLeft or foundEdgeRight or hitWall) and turnTimer.IsOver()) {
                    movementDirection *= -1;
                    rigidBody->SetSpeedOnX(SPEED_WALK * movementDirection);
                    FlipSprite(Sprite::HORIZONTAL);
                    
                    turnTimer.Reset();
                    hitWall = false;
                }
            } break;

        case EntityState::Injured:
            if (fabs(associated.box.x - damageOriginX) >= damageImpulse)
                FormatState(EntityState::Walking);
            break;

        case EntityState::Dead:
            break;

        default: break;
    }
}

bool EnemyRunner::NewStateRule (EntityState newState, int argsc, float argsv[]) {
    if (newState == state)
        return false;

    // auxiliar variables
    std::weak_ptr<GameObject> player;

    switch (newState) {
        case EntityState::Walking:
            rigidBody->SetSpeedOnX(SPEED_WALK * movementDirection);
            return true;

        case EntityState::Injured:
            player = Game::GetInstance().GetCurrentState().GetObjectPtr("Player");
            if (player.expired()) return false;
            damageOriginX = associated.box.x;
            damageImpulse = argsv[AttackGeneric::_Impulse] - IMPULSE_MASS;
            damageDirectionX = (player.lock()->box.x < associated.box.x)? 1 : -1;
            rigidBody->SetSpeedOnX(argsv[AttackGeneric::_ForceX] * damageDirectionX);
            hp -= argsv[AttackGeneric::_Damage];
            return true;

        case EntityState::Falling:
            rigidBody->SetSpeedOnX(rigidBody->GetSpeed().x * 0.4f);
            return true;

        default: return true;
    }
}

void EnemyRunner::NotifyCollision (GameObject& other) {
    rigidBody->NotifyCollision(other);

    if (rigidBody->ImpactDown() and (other.label == "Ground")) {
        if (state == EntityState::Falling)
            FormatState(EntityState::Walking);
        Collider* groundCollider = (Collider*)other.GetComponent(ComponentType::_Collider);
        currentRoute = Vec2(groundCollider->box.x, groundCollider->box.x+groundCollider->box.w);
        isGrounded = true;
    }
    if (rigidBody->ImpactLeft() or rigidBody->ImpactRight()) {
        if (state == EntityState::Injured)
            FormatState(EntityState::Walking);
        hitWall = true;
    }
}
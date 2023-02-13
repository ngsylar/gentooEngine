#include "GentooEngine.h"
#include "EnemyRunner.h"
#include "AttackGeneric.h"

#define SPRITE_IDLE                 "assets/img/monster1/idle.png"
#define SPRITE_WALK                 "assets/img/monster1/walk.png"
#define SPRITE_RUN_ATTACK           "assets/img/monster1/run_attack.png"
#define SPRITE_RECOVER              "assets/img/monster1/recovering.png"
#define SPRITE_DAMAGE               "assets/img/monster1/damage.png"
#define SPRITE_DEATH                "assets/img/monster1/death.png"

#define SPRITE_IDLE_FRAMES          12, 0.1f
#define SPRITE_WALK_FRAMES          8, 0.1f
#define SPRITE_RUN_ATTACK_FRAMES    6, 0.1f
#define SPRITE_DAMAGE_FRAMES        2, 0.1f
#define SPRITE_DEATH_FRAMES         13, 0.1f

#define SPEED_WALK                  30.0f
#define IMPULSE_MASS                20.0f

#define ATTACK_FORCE                400.0f, 140.0f
#define ATTACK_IMPULSE              50.0f
#define ATTACK_DAMAGE               1

#define TOGGLE_TIME_MIN_t10         15
#define TOGGLE_TIME_RANGE_t10       15

#define COLLIDER_POSITION           0.0f, 2.0f
#define COLLIDER_BOX_SIZE           18.0f, 39.0f

EnemyRunner::EnemyRunner (GameObject& associated): EntityMachine(associated) {
    type = type | ComponentType::_EnemyRunner;
    associated.label = "Enemy";
    movementDirection = 1;
    hp = 3;

    edgeTimer.SetResetTime(0.4f);
    recoverTimer.SetResetTime(0.4f);
    isGrounded = false;
    hitWall = false;

    // melius colliders' pixel correction
    associated.pixelColliderFix1 = true;
}

void EnemyRunner::Awaken () {
    Sprite* spriteIdle = new Sprite(associated, SPRITE_IDLE, SPRITE_IDLE_FRAMES);
    Sprite* spriteWalk = new Sprite(associated, SPRITE_WALK, SPRITE_WALK_FRAMES);
    Sprite* spriteRun = new Sprite(associated, SPRITE_RUN_ATTACK, SPRITE_RUN_ATTACK_FRAMES);
    Sprite* spriteFall = new Sprite(associated, SPRITE_RECOVER);
    Sprite* spriteRecover = new Sprite(associated, SPRITE_RECOVER);
    Sprite* spriteDamage = new Sprite(associated, SPRITE_DAMAGE, SPRITE_DAMAGE_FRAMES, true);
    Sprite* spriteDeath = new Sprite(associated, SPRITE_DEATH, SPRITE_DEATH_FRAMES, true);

    AddSpriteState(EntityState::Idle, spriteIdle);
    AddSpriteState(EntityState::Walking, spriteWalk);
    AddSpriteState(EntityState::Running, spriteRun);
    AddSpriteState(EntityState::Falling, spriteFall);
    AddSpriteState(EntityState::Recovering, spriteRecover);
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
    state = EntityState::Idle;
    float toggleTime = 0.1f * (rand() % TOGGLE_TIME_RANGE_t10 + TOGGLE_TIME_MIN_t10);
    toggleTimer.SetResetTime(toggleTime);
}

void EnemyRunner::LateUpdate (float dt) {}

void EnemyRunner::UpdateEntity (float dt) {
    if (isGrounded and (rigidBody->GetSpeed().y > 100)) {
        FormatState(EntityState::Falling);
        isGrounded = false;
    }

    switch (state) {
        case EntityState::Idle:
            toggleTimer.Update(dt);
            if (toggleTimer.IsOver())
                FormatState(EntityState::Walking);
            break;

        case EntityState::Walking:
            toggleTimer.Update(dt);
            if (toggleTimer.IsOver()) {
                FormatState(EntityState::Idle);
                break;
            }
            if (isGrounded) {
                bool foundEdgeLeft = collider->box.x < currentRoute.x;
                bool foundEdgeRight = collider->box.x+collider->box.w > currentRoute.y;
                edgeTimer.Update(dt);

                if ((foundEdgeLeft or foundEdgeRight or hitWall) and edgeTimer.IsOver()) {
                    movementDirection *= -1;
                    rigidBody->SetSpeedOnX(SPEED_WALK * movementDirection);
                    FlipSprite(Sprite::HORIZONTAL);

                    edgeTimer.Reset();
                    hitWall = false;
                }
            } break;

        case EntityState::Recovering:
            recoverTimer.Update(dt);
            if (recoverTimer.IsOver())
                FormatState(EntityState::Walking);
            break;

        case EntityState::Injured:
            if (fabs(associated.box.x - damageOriginX) >= damageImpulse)
                FormatState(EntityState::Recovering);
            break;

        case EntityState::Dead:
            break;

        default: break;
    }
}

bool EnemyRunner::NewStateRule (EntityState newState, int argsc, float argsv[]) {
    if (newState == state)
        return false;
    
    float toggleTime;

    switch (newState) {
        case EntityState::Idle:
            rigidBody->SetSpeedOnX(0.0f);
            toggleTime = 0.1f * (rand() % TOGGLE_TIME_RANGE_t10 + TOGGLE_TIME_MIN_t10);
            toggleTimer.SetResetTime(toggleTime);
            toggleTimer.Reset();
            return true;

        case EntityState::Walking:
            toggleTime = 0.1f * (rand() % TOGGLE_TIME_RANGE_t10 + TOGGLE_TIME_MIN_t10);
            toggleTimer.SetResetTime(toggleTime);
            toggleTimer.Reset();
            rigidBody->SetSpeedOnX(SPEED_WALK * movementDirection);
            edgeTimer.FalseStart();
            return true;

        case EntityState::Recovering:
            rigidBody->SetSpeedOnX(0.0f);
            recoverTimer.Reset();
            return true;

        case EntityState::Injured:
            damageOriginX = associated.box.x;
            damageImpulse = argsv[AttackGeneric::_Impulse]+argsv[AttackGeneric::_Displacement];
            damageImpulse -= (damageImpulse * IMPULSE_MASS) / argsv[AttackGeneric::_Impulse];
            damageDirectionX = (argsv[AttackGeneric::_OriginX] < associated.box.x)? 1 : -1;
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

    if (rigidBody->ImpactDown()) {
        if (state == EntityState::Falling)
            FormatState(EntityState::Idle);
        Collider* groundCollider = (Collider*)other.GetComponent(ComponentType::_Collider);
        currentRoute = Vec2(groundCollider->box.x, groundCollider->box.x+groundCollider->box.w);
        isGrounded = true;
    }
    if (rigidBody->ImpactLeft() or rigidBody->ImpactRight()) {
        if (state == EntityState::Injured)
            FormatState(EntityState::Idle);
        hitWall = true;
    }
}
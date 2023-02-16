#include "GentooEngine.h"
#include "EnemyRunner.h"
#include "AttackGeneric.h"
#include "Kid.h"

#define SPRITE_IDLE                     "assets/img/monster1/idle.png"
#define SPRITE_WALK                     "assets/img/monster1/walk.png"
#define SPRITE_RUN_ATTACK               "assets/img/monster1/run_attack.png"
#define SPRITE_DAMAGE                   "assets/img/monster1/damage.png"
#define SPRITE_DEATH                    "assets/img/monster1/death.png"

#define SPRITE_IDLE_FRAMES              12, 0.1f
#define SPRITE_WALK_FRAMES              8, 0.1f
#define SPRITE_RUN_ATTACK_FRAMES        6, 0.075f
#define SPRITE_DAMAGE_FRAMES            2, 0.1f
#define SPRITE_DEATH_FRAMES             13, 0.1f

#define SPEED_WALK                      30.0f
#define SPEED_RUN                       110.0f
#define IMPULSE_MASS                    20.0f

#define ATTACK_FORCE                    400.0f, 140.0f
#define ATTACK_IMPULSE                  50.0f
#define ATTACK_DAMAGE                   1

#define RECOVER_ACTIVATION_RECORD       2
#define RECOVER_ACTIVATION_TIME         0.8f

#define TOGGLE_TIME_MINt10              15
#define TOGGLE_TIME_RANGEt10            15

#define COLLIDER_POSITION               0.0f, 2.0f
#define COLLIDER_BOX_SIZE               18.0f, 39.0f

#define PERCEPTION_RANGE                125.0f, 50.0f, 75.0f, 30.0f

EnemyRunner::EnemyRunner (GameObject& associated): EntityMachine(associated) {
    type = type | ComponentType::_EnemyRunner;
    associated.layer = LayerDistance::_NPC;
    associated.label = "Enemy";
    movementDirection = 1;
    hp = 3;

    edgeTimer.SetResetTime(0.4f);
    attackTimer.SetResetTime(1.0f);
    recoverTimer.SetResetTime(0.4f);
    damageTaken = 0;
    isGrounded = false;
    hitWall = false;

    // melius colliders' pixel correction
    associated.pixelColliderFix1 = true;
}

void EnemyRunner::Awaken () {
    Sprite* spriteIdle = new Sprite(associated, SPRITE_IDLE, SPRITE_IDLE_FRAMES);
    Sprite* spriteWalk = new Sprite(associated, SPRITE_WALK, SPRITE_WALK_FRAMES);
    Sprite* spriteFall = new Sprite(associated, SPRITE_DAMAGE, SPRITE_DAMAGE_FRAMES, true);
    Sprite* spriteAttack = new Sprite(associated, SPRITE_RUN_ATTACK, SPRITE_RUN_ATTACK_FRAMES);
    Sprite* spriteRecover = new Sprite(associated, SPRITE_DAMAGE, SPRITE_DAMAGE_FRAMES);
    Sprite* spriteDamage = new Sprite(associated, SPRITE_DAMAGE, SPRITE_DAMAGE_FRAMES, true);
    Sprite* spriteDeath = new Sprite(associated, SPRITE_DEATH, SPRITE_DEATH_FRAMES, true);

    AddSpriteState(EntityState::Idle, spriteIdle);
    AddSpriteState(EntityState::Walking, spriteWalk);
    AddSpriteState(EntityState::Falling, spriteFall);
    AddSpriteState(EntityState::Attacking, spriteAttack);
    AddSpriteState(EntityState::Recovering, spriteRecover);
    AddSpriteState(EntityState::Injured, spriteDamage);
    AddSpriteState(EntityState::Dead, spriteDeath);

    rigidBody = new RigidBody(associated);
    associated.AddComponent(rigidBody);
    rigidBody->triggerLabels.push_back("Player");
    rigidBody->triggerLabels.push_back("Enemy");
    rigidBody->triggerLabels.push_back("Boss");

    collider = new Collider(associated);
    associated.AddComponent(collider);
    collider->SetBox(Vec2(COLLIDER_POSITION), Vec2(COLLIDER_BOX_SIZE));
    perceptionRange = Rect(PERCEPTION_RANGE);

    AttackGeneric* attack = new AttackGeneric(associated);
    attack->SetProperties(Vec2(ATTACK_FORCE), ATTACK_IMPULSE, ATTACK_DAMAGE);
    associated.AddComponent(attack);
}

void EnemyRunner::Start () {
    state = EntityState::Idle;
    float toggleTime = 0.1f * (rand() % TOGGLE_TIME_RANGEt10 + TOGGLE_TIME_MINt10);
    toggleTimer.SetResetTime(toggleTime);
}

void EnemyRunner::LateUpdate (float dt) {}

void EnemyRunner::UpdateEntity (float dt) {
    if (state == EntityState::Dead) {
        if (sprites[state]->OneshotIsOver())
            associated.RequestDelete();
        return;
    }
    if (isGrounded and (rigidBody->GetSpeed().y > 100)) {
        FormatState(EntityState::Falling);
        isGrounded = false;
    }
    else if (hp <= 0) {
        associated.RemoveComponent(associated.GetComponent(ComponentType::_Attack));
        associated.RemoveComponent(rigidBody);
        associated.RemoveComponent(collider);
        FormatState(EntityState::Dead);
        return;
    }

    GameObject* kid = Kid::GetInstance();
    if (kid and (state != EntityState::Attacking) and (state != EntityState::Injured)
    and (damageTaken < RECOVER_ACTIVATION_RECORD)) {
        Rect perception = (movementDirection > 0)? perceptionRange :
            Rect(perceptionRange.w, perceptionRange.y, perceptionRange.x, perceptionRange.h);
        Vec2 center = collider->box.GetPosition();
        attackTarget = kid->box.GetPosition();

        if ((attackTarget.x < center.x+perception.x) and (attackTarget.x > center.x-perception.w)
        and (attackTarget.y < center.y+perception.y) and (attackTarget.y > center.y-perception.h))
            FormatState(EntityState::Attacking);
    }

    switch (state) {
        case EntityState::Idle:
            toggleTimer.Update(dt);
            if (toggleTimer.IsOver()) {
                damageTaken = 0;
                FormatState(EntityState::Walking);
            } break;

        case EntityState::Walking:
            toggleTimer.Update(dt);
            if (toggleTimer.IsOver()) {
                damageTaken = 0;
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
        
        case EntityState::Attacking:
            attackTimer.Update(dt);
            if (attackTimer.IsOver()) {
                FormatState(EntityState::Idle);
            } else if (hitWall) {
                hitWall = false;
                FormatState(EntityState::Idle);
            } break;

        case EntityState::Recovering:
            recoverTimer.Update(dt);
            if (recoverTimer.IsOver()) {
                damageTaken = 0;
                FormatState(EntityState::Walking);
            } break;

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
            toggleTime = 0.1f * (rand() % TOGGLE_TIME_RANGEt10 + TOGGLE_TIME_MINt10);
            toggleTimer.SetResetTime(toggleTime);
            toggleTimer.Reset();
            return true;

        case EntityState::Walking:
            toggleTime = 0.1f * (rand() % TOGGLE_TIME_RANGEt10 + TOGGLE_TIME_MINt10);
            toggleTimer.SetResetTime(toggleTime);
            toggleTimer.Reset();
            rigidBody->SetSpeedOnX(SPEED_WALK * movementDirection);
            edgeTimer.FalseStart();
            return true;

        case EntityState::Attacking:
            if (((attackTarget.x < collider->box.x) and (movementDirection == 1))
            or ((attackTarget.x > collider->box.x) and (movementDirection == -1))) {
                FlipSprite(Sprite::HORIZONTAL);
                movementDirection *= -1;
            } rigidBody->SetSpeedOnX(SPEED_RUN * movementDirection);
            attackTimer.Reset();
            return true;

        case EntityState::Recovering:
            rigidBody->SetSpeedOnX(0.0f);
            recoverTimer.Reset();
            if (damageTaken >= RECOVER_ACTIVATION_RECORD)
                recoverTimer.Update(-RECOVER_ACTIVATION_TIME);
            return true;

        case EntityState::Injured:
            damageOriginX = associated.box.x;
            damageImpulse = argsv[AttackGeneric::_Impulse]+argsv[AttackGeneric::_Displacement];
            damageImpulse -= (damageImpulse * IMPULSE_MASS) / argsv[AttackGeneric::_Impulse];
            damageDirectionX = (argsv[AttackGeneric::_OriginX] < associated.box.x)? 1 : -1;
            rigidBody->SetSpeedOnX(argsv[AttackGeneric::_ForceX] * damageDirectionX);
            hp -= argsv[AttackGeneric::_Damage];
            damageTaken++;
            return true;

        case EntityState::Falling:
            rigidBody->SetSpeedOnX(rigidBody->GetSpeed().x * 0.4f);
            return true;

        default: return true;
    }
}

// DEBUG
void EnemyRunner::RenderEntity () {
    if (not Game::GetInstance().GetCurrentState().Debugging()) return;

    Rect box = (movementDirection > 0)? perceptionRange :
        Rect(perceptionRange.w, perceptionRange.y, perceptionRange.x, perceptionRange.h);
    Vec2 center( collider->box.GetPosition() );
    SDL_Point points[5];

    Vec2 point = center + Vec2(-box.w, -box.y) - Camera::pos;
    points[4] = points[0] = {(int)point.x, (int)point.y};

    point = center + Vec2(box.x, -box.y) - Camera::pos;
    points[1] = {(int)point.x, (int)point.y};
    
    point = center + Vec2(box.x, box.h) - Camera::pos;
    points[2] = {(int)point.x, (int)point.y};
    
    point = center + Vec2(-box.w, box.h) - Camera::pos;
    points[3] = {(int)point.x, (int)point.y};

    SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 0, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLines(Game::GetInstance().GetRenderer(), points, 5);
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
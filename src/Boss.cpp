#include "GentooEngine.h"
#include "Boss.h"
#include "AttackGeneric.h"
#include "Kid.h"

#define SPRITE_IDLE                     "assets/img/boss/idle.png"
#define SPRITE_WALK                     "assets/img/boss/walk.png"
#define SPRITE_RUN                      "assets/img/boss/walk.png"
#define SPRITE_ATTACK0                  "assets/img/boss/attack1.png"
#define SPRITE_ATTACK1                  "assets/img/boss/attack2.png"
#define SPRITE_ATTACK2                  "assets/img/boss/attack3.png"
#define SPRITE_DAMAGE                   "assets/img/boss/damage.png"
#define SPRITE_DEATH                    "assets/img/boss/death.png"

#define SPRITE_IDLE_FRAMES              18, 0.05f
#define SPRITE_WALK_FRAMES              16, 0.05f
#define SPRITE_ATTACK0_FRAMES           12, 0.1f
#define SPRITE_ATTACK1_FRAMES           16, 0.1f
#define SPRITE_ATTACK2_FRAMES           11, 0.1f
#define SPRITE_DAMAGE_FRAMES            6, 0.1f
#define SPRITE_DEATH_FRAMES             22, 0.1f

#define SPEED_WALK                      16.0f
#define SPEED_WALKt2                    32.0f
#define SPEED_ATTACK                    600.0f
#define IMPULSE_ATTACK_X                20

#define WALK_FAST_DISTANCE              400.0f

#define ATTACK_MELEE0_DISTANCE          60.0f
#define ATTACK_MELEE0_TIME_START        0.5f
#define ATTACK_MELEE0_TIME_DURING       0.3f
#define ATTACK_MELEE0_TIME_END          0.7f

#define ATTACK_MELEE1_DISTANCE          100.0f
#define ATTACK_MELEE1_TIME_START        0.5f
#define ATTACK_MELEE1_TIME_DURING       0.2f
#define ATTACK_MELEE1_TIME_END          0.4f

#define ATTACK_RANGED_DISTANCE          400

#define COLLIDER_POSITION               3.0f, 10.0f
#define COLLIDER_BOX_SIZE               31.0f, 59.0f

Boss::Boss (GameObject& associated): EntityMachine(associated) {
    type = type | ComponentType::_Boss;
    associated.label = "Boss";
    movementDirection = -1;
    hp = 70;

    restTimer.SetResetTime(0.9f);
    damageTimer.SetResetTime(0.6f);
    recoverTimer.SetResetTime(0.15f);
    isAttacking = false;
    damageTaken = 0;
}

void Boss::Awaken () {
    Sprite* spriteIdle = new Sprite(associated, SPRITE_IDLE, SPRITE_IDLE_FRAMES);
    Sprite* spriteWalk = new Sprite(associated, SPRITE_WALK, SPRITE_WALK_FRAMES);
    Sprite* spriteAttack0 = new Sprite(associated, SPRITE_ATTACK0, SPRITE_ATTACK0_FRAMES, true);
    Sprite* spriteAttack1 = new Sprite(associated, SPRITE_ATTACK1, SPRITE_ATTACK1_FRAMES);
    // Sprite* spriteAttack3 = new Sprite(associated, SPRITE_ATTACK3, SPRITE_ATTACK3_FRAMES);
    Sprite* spriteRecover = new Sprite(associated, SPRITE_IDLE, SPRITE_IDLE_FRAMES);
    Sprite* spriteDamage = new Sprite(associated, SPRITE_DAMAGE, SPRITE_DAMAGE_FRAMES, true);
    Sprite* spriteDeath = new Sprite(associated, SPRITE_DEATH, SPRITE_DEATH_FRAMES, true);

    AddSpriteState(EntityState::Idle, spriteIdle);
    AddSpriteState(EntityState::Walking, spriteWalk);
    AddSpriteState(EntityState::AttackingMelee_0, spriteAttack0);
    AddSpriteState(EntityState::AttackingMelee_1, spriteAttack1);
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
}

void Boss::Start () {
    state = Idle;
}

void Boss::LateUpdate (float dt) {}

void Boss::UpdateEntity (float dt) {
    if (state == Dead) {
        if (sprites[state]->OneshotIsOver())
            associated.RequestDelete();
        return;
    }

    GameObject* kid = Kid::GetInstance();
    Vec2 position = collider->box.GetPosition();
    Vec2 kidPosition = kid->box.GetPosition();
    RigidBody* kidRigidBody;

    int direction = ((position.x - kidPosition.x) < 0.0f)? 1 : -1;
    if ((direction != movementDirection) and (not isAttacking)) {
        movementDirection = direction;
        FlipSprite(Sprite::HORIZONTAL);
    }

    switch (state) {
        case Idle:
            restTimer.Update(dt);
            if (restTimer.IsOver()) {
                restTimer.Reset();

                if (position.DistanceTo(kidPosition) < ATTACK_MELEE0_DISTANCE)
                    FormatState(AttackingMelee_0);
                else if (position.DistanceTo(kidPosition) < ATTACK_MELEE1_DISTANCE)
                    FormatState(AttackingMelee_1);

                else FormatState(Walking);
            } break;

        case Walking:
            if (position.DistanceTo(kidPosition) < ATTACK_MELEE0_DISTANCE)
                FormatState(AttackingMelee_0);
            else if (position.DistanceTo(kidPosition) < ATTACK_MELEE1_DISTANCE)
                FormatState(AttackingMelee_1);

            else if (position.DistanceTo(kidPosition) < WALK_FAST_DISTANCE)
                rigidBody->SetSpeedOnX(SPEED_WALK * movementDirection);
            else rigidBody->SetSpeedOnX(SPEED_WALKt2 * movementDirection);

            break;

        case Injured:
            kidRigidBody = (RigidBody*)kid->GetComponent(ComponentType::_RigidBody);
            if ((kidRigidBody == nullptr) or (kidRigidBody->GetSpeed().x == 0.0f))
                rigidBody->SetSpeedOnX(0.0f);

            damageTimer.Update(dt);
            if (damageTimer.IsOver())
                FormatState(Recovering);

            break;

        case Recovering:
            recoverTimer.Update(dt);
            if (recoverTimer.IsOver()) {
                damageTaken = 2;

                if (position.DistanceTo(kidPosition) < ATTACK_MELEE0_DISTANCE)
                    FormatState(AttackingMelee_0);
                else if (position.DistanceTo(kidPosition) < ATTACK_MELEE1_DISTANCE)
                    FormatState(AttackingMelee_1);

                else FormatState(Walking);
            } break;

        case AttackingMelee_0: case AttackingMelee_1:
            AttackMeleeUpdate(dt);
            break;

        default: break;
    }
}

bool Boss::NewStateRule (EntityState newState, int argsc, float argsv[]) {
    if (newState == state)
        return false;

    switch (state) {
        case AttackingMelee_0: case AttackingMelee_1:
            if ((newState == Injured) and (damageTaken >= 2)) {
                hp -= argsv[AttackGeneric::_Damage];
                return false;
            }
            else isAttacking = false;
            break;

        case Recovering:
            if ((newState == Injured) and (damageTaken >= 2)) {
                hp -= argsv[AttackGeneric::_Damage];
                return false;
            } break;

        default: break;
    }

    switch (newState) {        
        case AttackingMelee_0:
            rigidBody->SetSpeedOnX(0.0f);
            attackState = _Stage0;
            attackOriginX = associated.box.x;
            attackTimer.SetResetTime(ATTACK_MELEE0_TIME_START);
            attackTimer.Reset();
            isAttacking = true;
            return true;
        
        case AttackingMelee_1:
            rigidBody->SetSpeedOnX(0.0f);
            attackState = _Stage2;
            attackOriginX = associated.box.x;
            attackTimer.SetResetTime(ATTACK_MELEE1_TIME_START);
            attackTimer.Reset();
            isAttacking = true;
            return true;
        
        case Recovering:
            recoverTimer.Reset();
            return true;

        case Injured:
            hp -= argsv[AttackGeneric::_Damage];
            rigidBody->SetSpeedOnX(0.0f);
            damageTimer.Reset();
            damageTaken++;
            return true;

        default: return true;
    }
}

void Boss::AttackMeleeUpdate (float dt) {
    attackTimer.Update(dt);

    switch (attackState) {
        case _Stage0:
            if (attackTimer.IsOver()) {
                // editar: ativar classe ataque aqui
                rigidBody->SetSpeedOnX(SPEED_ATTACK * movementDirection);
                attackTimer.SetResetTime(ATTACK_MELEE0_TIME_END);
                attackTimer.Reset();
                attackState = _Stage1;
            } break;

        case _Stage1:
            if (fabs(associated.box.x - attackOriginX) >= IMPULSE_ATTACK_X)
                rigidBody->SetSpeedOnX(0.0f);
            if (attackTimer.IsOver()) {
                if (damageTaken > 0)
                    damageTaken = 0;
                FormatState(Idle);
            } break;

        case _Stage2:
            if (attackTimer.IsOver()) {
                // editar: ativar classe ataque aqui
                rigidBody->SetSpeedOnX(SPEED_ATTACK * movementDirection);
                attackTimer.SetResetTime(ATTACK_MELEE1_TIME_END);
                attackTimer.Reset();
                attackState = _Stage3;
            } break;

        case _Stage3:
            if (fabs(associated.box.x - attackOriginX) >= IMPULSE_ATTACK_X) {
                rigidBody->SetSpeedOnX(0.0f);
                attackOriginX = associated.box.x;
            } if (attackTimer.IsOver()) {
                rigidBody->SetSpeedOnX(SPEED_ATTACK * movementDirection);
                attackTimer.SetResetTime(ATTACK_MELEE0_TIME_END);
                attackTimer.Reset();
                attackState = _Stage1;
            } break;

        default: break;
    }
}

// DEBUG
void Boss::RenderEntity () {
    if (not Game::GetInstance().GetCurrentState().Debugging())
        return;

    float attackMeleeDistances[3] = {
        ATTACK_MELEE0_DISTANCE, ATTACK_MELEE1_DISTANCE, WALK_FAST_DISTANCE
    };
    for (auto& distance: attackMeleeDistances) {
        Rect colliderBox(COLLIDER_BOX_SIZE, COLLIDER_POSITION);
        Rect perceptionRange(
            distance, (colliderBox.y * 0.5f) + 5.0f,
            (colliderBox.x * 0.5f) + 5.0f, (colliderBox.y * 0.5f) + 5.0f
        );
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
}

void Boss::NotifyCollision (GameObject& other) {}
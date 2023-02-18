#include "GentooEngine.h"
#include "Boss.h"
#include "AttackGeneric.h"
#include "Kid.h"

#define SPRITE_IDLE                 "assets/img/boss/idle.png"
#define SPRITE_WALK                 "assets/img/boss/walk.png"
#define SPRITE_RUN                  "assets/img/boss/walk.png"
#define SPRITE_ATTACK0              "assets/img/boss/attack1.png"
#define SPRITE_ATTACK1              "assets/img/boss/attack2.png"
#define SPRITE_ATTACK2              "assets/img/boss/attack3.png"
#define SPRITE_DAMAGE               "assets/img/boss/damage.png"
#define SPRITE_DEATH                "assets/img/boss/death.png"

#define SPRITE_IDLE_FRAMES          9, 0.1f
#define SPRITE_WALK_FRAMES          8, 0.1f
#define SPRITE_ATTACK0_FRAMES       12, 0.1f
#define SPRITE_ATTACK1_FRAMES       16, 0.1f
#define SPRITE_ATTACK2_FRAMES       11, 0.1f
#define SPRITE_DAMAGE_FRAMES        6, 0.1f
#define SPRITE_DEATH_FRAMES         22, 0.1f

#define SPEED_WALK                  16.0f
#define SPEED_WALKt2                32.0f
#define SPEED_ATTACK                600.0f
#define IMPULSE_ATTACK_X            20.0f
#define IMPULSE_MASS                50.0f

#define ATTACK_FORCE                400.0f, 140.0f
#define ATTACK_IMPULSE              50.0f
#define ATTACK_DAMAGE               1

#define WALK_FAST_DISTANCE          400.0f

#define ATTACK_MELEE0_DISTANCE      60.0f
#define ATTACK_MELEE0_TIME_START    0.5f
#define ATTACK_MELEE0_TIME_DURING   0.3f
#define ATTACK_MELEE0_TIME_END      0.7f

#define ATTACK_MELEE1_DISTANCE      100.0f
#define ATTACK_MELEE1_TIME_START    0.5f
#define ATTACK_MELEE1_TIME_DURING   0.2f
#define ATTACK_MELEE1_TIME_END      0.4f
#include "GameScene.h"
#define ATTACK_RANGED_DISTANCE      400

#define COLLIDER_POSITION           3.0f, 10.0f
#define COLLIDER_BOX_SIZE           31.0f, 59.0f
Boss* Boss::self = nullptr;
Boss::Boss (GameObject& associated): EntityMachine(associated) {
    type = type | ComponentType::_Boss;
    associated.layer = LayerDistance::_NPC;
    associated.label = "Boss";
    movementDirection = -1;
    hp = 15;

    restTimer.SetResetTime(0.9f);
    damageTimer.SetResetTime(0.6f);
    recoverTimer.SetResetTime(0.15f);
    isAttacking = false;
    barrierIsBroken = false;
    damageTaken = 0;
    self = this;
    GameObject* attack = new GameObject(LayerDistance::_NPC_Close);
    meleeAttack = new BossAttackMelee(*attack, &associated);
    meleeAttack->SetProperties(Vec2(ATTACK_FORCE), ATTACK_IMPULSE, ATTACK_DAMAGE, IMPULSE_ATTACK_X);
    attack->AddComponent(meleeAttack);
    Game::GetInstance().GetCurrentState().AddObject(attack);
    deathFade = false;
    deathSequence.SetResetTime(3.5);
    deathSequence.Reset();
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

    AttackGeneric* attack = new AttackGeneric(associated);
    attack->SetProperties(Vec2(ATTACK_FORCE), ATTACK_IMPULSE, ATTACK_DAMAGE);
    associated.AddComponent(attack);
}

void Boss::Start () {
    state = Idle;
}

void Boss::LateUpdate (float dt) {}

void Boss::UpdateEntity (float dt) {
    if (state == Dead) {
        if (sprites[state]->OneshotIsOver())
            // associated.RequestDelete();
        deathSequence.Update(dt);
        if(deathSequence.GetTime()>1 and !deathFade){
            associated.SetLayer(LayerDistance::_DeathLayer);
            GameObject* transitionGO = new GameObject(LayerDistance::_FadingLayer);
            ScreenFade* transitionFilter = new ScreenFade(*transitionGO, Color("#000000"), 0, 1, STATE_FADE_TIME);
            transitionGO->AddComponent(transitionFilter);
            Game::GetInstance().GetCurrentState().AddObject(transitionGO);
            Game::GetInstance().GetCurrentState().GetStateMusic()->Stop(STATE_FADE_TIME*1000);//To stop current music
            deathFade = true;
        } else if(deathSequence.IsOver()) {
            Game::GetInstance().AddState(new End());
            Game::GetInstance().GetCurrentState().RequestPop();

        }
        return;// HERE! FADE AND CALL 
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

    if ((state != Dead) and (hp <= 0)) {
        Die();
        return;
    }

    if (barrierIsBroken) {
        barrierBrokenTimer.Update(dt);
        if (barrierBrokenTimer.IsOver())
            barrierIsBroken = false;
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
                // hp -= argsv[AttackGeneric::_Damage];
                return false;
            }
            else isAttacking = false;
            break;

        case Recovering:
            if ((newState == Injured) and (damageTaken >= 2)) {
                // hp -= argsv[AttackGeneric::_Damage];
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

            meleeAttack->PushAttack(Vec2(1.0f, 14.0f), Vec2(39, 22), 0.5f, 0.6f);
            meleeAttack->PushAttack(Vec2(1.0f, 24.0f), Vec2(39, 33), 0.0f, 0.1f);
            meleeAttack->PushAttack(Vec2(4.0f, 48.0f), Vec2(36, 13), 0.0f, 0.1f);
            meleeAttack->Perform(
                (movementDirection < 0)? BossAttackMelee::LEFT : BossAttackMelee::RIGHT);
            return true;
        
        case AttackingMelee_1:
            rigidBody->SetSpeedOnX(0.0f);
            attackState = _Stage2;
            attackOriginX = associated.box.x;
            attackTimer.SetResetTime(ATTACK_MELEE1_TIME_START);
            attackTimer.Reset();
            isAttacking = true;

            meleeAttack->PushAttack(Vec2(4.0f, 13.0f), Vec2(36, 38), 0.5f, 0.6f);
            meleeAttack->PushAttack(Vec2(7.0f, 28.0f), Vec2(33, 45), 0.0f, 0.1f);
            meleeAttack->PushAttack(Vec2(1.0f, 14.0f), Vec2(39, 22), 0.2f, 0.3f);
            meleeAttack->PushAttack(Vec2(1.0f, 24.0f), Vec2(39, 33), 0.0f, 0.1f);
            meleeAttack->PushAttack(Vec2(4.0f, 48.0f), Vec2(36, 13), 0.0f, 0.1f);
            meleeAttack->Perform(
                (movementDirection < 0)? BossAttackMelee::LEFT : BossAttackMelee::RIGHT);
            return true;

        case Recovering:
            recoverTimer.Reset();
            return true;

        case Injured:
            meleeAttack->CancelAttack();
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

void Boss::Die () {
    self = nullptr;
    associated.RemoveComponent(rigidBody);
    associated.RemoveComponent(collider);
    FormatState(EntityState::Dead);
}

bool Boss::BreakBarrier (float brokenTime) {
    if (barrierIsBroken) return false;
    barrierBrokenTimer.SetResetTime(brokenTime + 0.6f);
    barrierBrokenTimer.Reset();
    barrierIsBroken = true;
    damageTaken = 0;
    return true;
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
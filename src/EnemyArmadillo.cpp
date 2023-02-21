#include "GentooEngine.h"
#include "EnemyArmadillo.h"
#include "AttackGeneric.h"

#define SPRITE_RUN              "assets/img/armadillo/run.png"
#define SPRITE_DAMAGE           "assets/img/armadillo/damage.png"
#define SPRITE_DEATH            "assets/img/armadillo/death.png"

#define SPRITE_RUN_FRAMES       6, 0.125f
#define SPRITE_DAMAGE_FRAMES    2, 0.1f
#define SPRITE_DEATH_FRAMES     8, 0.1f

#define SPEED_RUN               50.0f
#define IMPULSE_MASS            10.0f

#define ATTACK_FORCE            400.0f, 140.0f
#define ATTACK_IMPULSE          50.0f
#define ATTACK_DAMAGE           1

#define COLLIDER_POSITION       0.0f, 13.0f
#define COLLIDER_BOX_SIZE       21.0f, 19.0f

EnemyArmadillo::EnemyArmadillo (GameObject& associated): EntityMachine(associated) {
    type = type | ComponentType::_EnemyArmadillo;
    associated.layer = LayerDistance::_NPC;
    associated.label = "Enemy";
    movementDirection = -1;
    hp = 2;

    turnTimer.SetResetTime(0.4f);
    isGrounded = false;
    hitWall = false;
}

void EnemyArmadillo::Awaken () {
    Sprite* spriteRun = new Sprite(associated, SPRITE_RUN, SPRITE_RUN_FRAMES);
    Sprite* spriteDamage = new Sprite(associated, SPRITE_DAMAGE, SPRITE_DAMAGE_FRAMES);
    Sprite* spriteFall = new Sprite(associated, SPRITE_DAMAGE, SPRITE_DAMAGE_FRAMES);
    Sprite* spriteDie = new Sprite(associated, SPRITE_DEATH, SPRITE_DEATH_FRAMES, true);

    AddSpriteState(EntityState::Running, spriteRun);
    AddSpriteState(EntityState::Injured, spriteDamage);
    AddSpriteState(EntityState::Falling, spriteFall);
    AddSpriteState(EntityState::Dead, spriteDie);

    rigidBody = new RigidBody(associated);
    associated.AddComponent(rigidBody);
    rigidBody->triggerLabels.push_back("Player");
    rigidBody->triggerLabels.push_back("Enemy");
    rigidBody->triggerLabels.push_back("Boss");

    collider = new Collider(associated);
    associated.AddComponent(collider);
    collider->SetBox(Vec2(COLLIDER_POSITION), Vec2(COLLIDER_BOX_SIZE));

    AttackGeneric* attack = new AttackGeneric(associated);
    attack->SetProperties(Vec2(ATTACK_FORCE), ATTACK_IMPULSE, ATTACK_DAMAGE);
    associated.AddComponent(attack);
}

void EnemyArmadillo::Start () {
    state = EntityState::Running;
    sprites[state].get()->SetFrame(0);
    rigidBody->SetSpeedOnX(SPEED_RUN * movementDirection);
}

void EnemyArmadillo::LateUpdate (float dt) {}

void EnemyArmadillo::UpdateEntity (float dt) {
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

    switch (state) {
        case EntityState::Running:
            if (isGrounded) {
                bool foundEdgeLeft = collider->box.x < currentRoute.x;
                bool foundEdgeRight = collider->box.x+collider->box.w > currentRoute.y;
                turnTimer.Update(dt);

                if ((foundEdgeLeft or foundEdgeRight or hitWall) and turnTimer.IsOver()) {
                    movementDirection *= -1;
                    rigidBody->SetSpeedOnX(SPEED_RUN * movementDirection);
                    FlipSprite(Sprite::HORIZONTAL);

                    turnTimer.Reset();
                    hitWall = false;
                }
            } break;

        case EntityState::Injured:
            if (fabs(associated.box.x - damageOriginX) >= damageImpulse)
                FormatState(EntityState::Falling);
            break;

        case EntityState::Dead:
            break;

        default: break;
    }
}

bool EnemyArmadillo::NewStateRule (EntityState newState, int argsc, float argsv[]) {
    if (newState == state)
        return false;

    switch (newState) {
        case EntityState::Running:
            rigidBody->SetSpeedOnX(SPEED_RUN * movementDirection);
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

void EnemyArmadillo::FlipEntity () {
    movementDirection *= -1;
    FlipSprite(Sprite::HORIZONTAL);
}

void EnemyArmadillo::NotifyCollision (GameObject& other) {
    rigidBody->NotifyCollision(other);

    if (rigidBody->ImpactDown()) {
        if (state == EntityState::Falling)
            FormatState(EntityState::Running);
        Collider* groundCollider = (Collider*)other.GetComponent(ComponentType::_Collider);
        currentRoute = Vec2(
            groundCollider->box.x + (SPEED_RUN * 0.1f),
            groundCollider->box.x + groundCollider->box.w - (SPEED_RUN * 0.1f));
        isGrounded = true;
    }
    // if (rigidBody->ImpactLeft() or rigidBody->ImpactRight()) {
    //     if (state == EntityState::Injured)
    //         FormatState(EntityState::Running);
    //     hitWall = true;
    // }
    if (other.GetComponent(ComponentType::_ZoneTransition))
        hitWall = true;

    /*--------------------------------------------------------------------------------------------------*/
    // editar: iniciando tecnicas de gambiarras
    /*--------------------------------------------------------------------------------------------------*/

    if (rigidBody->ImpactLeft()) {
        if (state == EntityState::Injured)
            FormatState(EntityState::Running);
        associated.box.SetPosition(associated.box.GetPosition()+2.0f);
        hitWall = true;
    }
    if (rigidBody->ImpactRight()) {
        if (state == EntityState::Injured)
            FormatState(EntityState::Running);
        associated.box.SetPosition(associated.box.GetPosition()-2.0f);
        hitWall = true;
    }
}
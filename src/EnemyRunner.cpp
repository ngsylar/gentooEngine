#include "GentooEngine.h"
#include "EnemyRunner.h"
#include "AttackGeneric.h"

#define SPRITE_RUN          "assets/img/monster1/idle.png"

#define SPRITE_IDLE_FRAMES  12, 0.1f

#define SPEED_RUN           60.0f
#define IMPULSE_MASS        10.0f

#define ATTACK_FORCE        400.0f, 140.0f
#define ATTACK_IMPULSE      50.0f
#define ATTACK_DAMAGE       1

#define COLLIDER_POSITION   -1.0f, 2.0f
#define COLLIDER_BOX_SIZE   16.0f, 39.0f

EnemyRunner::EnemyRunner (GameObject& associated): EntityMachine(associated) {
    type = type | ComponentType::_EnemyRunner;
    associated.label = "Enemy";
    movementDirection = 1;
    hp = 3;

    turnTimer.SetResetTime(0.4f);
    isGrounded = false;
    hitWall = false;

    associated.pixelColliderFix1 = true;
}

void EnemyRunner::Awaken () {
    Sprite* spriteRun = new Sprite(associated, SPRITE_RUN, SPRITE_IDLE_FRAMES);
    Sprite* spriteDamage = new Sprite(associated, SPRITE_RUN, SPRITE_IDLE_FRAMES);
    Sprite* spriteFall = new Sprite(associated, SPRITE_RUN, SPRITE_IDLE_FRAMES);
    Sprite* spriteDie = new Sprite(associated, SPRITE_RUN, SPRITE_IDLE_FRAMES);

    AddSpriteState(EntityState::Running, spriteRun);
    AddSpriteState(EntityState::Injured, spriteDamage);
    AddSpriteState(EntityState::Falling, spriteFall);
    AddSpriteState(EntityState::Dying, spriteDie);

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
    state = EntityState::Running;
    sprites[state].get()->SetFrame(0);
    rigidBody->SetSpeedOnX(SPEED_RUN * movementDirection);
}

void EnemyRunner::LateUpdate (float dt) {}

void EnemyRunner::UpdateEntity (float dt) {
    if (isGrounded and (rigidBody->GetSpeed().y > 100))
        isGrounded = false;

    switch (state) {
        case EntityState::Running:
            if (isGrounded) {
                bool foundEdgeLeft = collider->box.x < currentRoute.x;
                bool foundEdgeRight = collider->box.x+collider->box.w > currentRoute.y;
                turnTimer.Update(dt);

                SDL_Log("%d", movementDirection);
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

        case EntityState::Dying:
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
        case EntityState::Running:
            rigidBody->SetSpeedOnX(SPEED_RUN * movementDirection);
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
            FormatState(EntityState::Running);
        Collider* groundCollider = (Collider*)other.GetComponent(ComponentType::_Collider);
        currentRoute = Vec2(groundCollider->box.x, groundCollider->box.x+groundCollider->box.w);
        isGrounded = true;
    }
    if (rigidBody->ImpactLeft() or rigidBody->ImpactRight()) {
        if (state == EntityState::Injured)
            FormatState(EntityState::Running);
        hitWall = true;
    }
}
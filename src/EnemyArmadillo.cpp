#include "GentooEngine.h"
#include "EnemyArmadillo.h"
#include "Attack.h"

#define SPRITE_RUN          "assets/img/enemy1.png"

#define SPEED_RUN           60.0f
#define IMPULSE_MASS        10.0f

#define ATTACK_FORCE        400.0f, 140.0f
#define ATTACK_IMPULSE      70.0f
#define ATTACK_DAMAGE       1

#define COLLIDER_POSITION   0.0f, 14.0f
#define COLLIDER_BOX_SIZE   21.0f, 19.0f

EnemyArmadillo::EnemyArmadillo (GameObject& associated): EntityMachine(associated) {
    type = type | ComponentType::_EnemyArmadillo;
    associated.label = "Enemy";
    movementDirection = -1;
    hp = 2;

    isGrounded = false;
    hitWall = false;
}

void EnemyArmadillo::Awaken () {
    Sprite* spriteRun = new Sprite(associated, SPRITE_RUN);
    Sprite* spriteDamage = new Sprite(associated, SPRITE_RUN);
    Sprite* spriteDie = new Sprite(associated, SPRITE_RUN);

    AddSpriteState(EntityState::Running, spriteRun);
    AddSpriteState(EntityState::Injured, spriteDamage);
    AddSpriteState(EntityState::Dying, spriteDie);

    rigidBody = new RigidBody(associated);
    associated.AddComponent(rigidBody);
    rigidBody->triggerLabels.push_back("Player");
    rigidBody->triggerLabels.push_back("Enemy");

    collider = new Collider(associated);
    associated.AddComponent(collider);
    collider->SetBox(Vec2(COLLIDER_POSITION), Vec2(COLLIDER_BOX_SIZE));

    Attack* attack = new Attack(associated);
    attack->SetProperties(Vec2(ATTACK_FORCE), ATTACK_IMPULSE, ATTACK_DAMAGE);
    associated.AddComponent(attack);
}

void EnemyArmadillo::Start () {
    state = EntityState::Running;
    rigidBody->SetSpeedOnX(SPEED_RUN * movementDirection);
}

void EnemyArmadillo::LateUpdate (float dt) {}

void EnemyArmadillo::UpdateEntity (float dt) {
    if (isGrounded and (rigidBody->GetSpeed().y > 100))
        isGrounded = false;

    switch (state) {
        case EntityState::Running:
            if (isGrounded) {
                bool foundEdgeLeft = collider->box.x < currentRoute.x;
                bool foundEdgeRight = collider->box.x+collider->box.w > currentRoute.y;
                if (foundEdgeLeft or foundEdgeRight or hitWall) {
                    movementDirection *= -1;
                    rigidBody->SetSpeedOnX(SPEED_RUN * movementDirection);
                    FlipSprite(Sprite::HORIZONTAL);
                    hitWall = false;
                }
            } break;

        case EntityState::Injured:
            if (fabs(associated.box.x - damageOriginX) >= damageImpulse)
                FormatState(EntityState::Running);
            break;

        case EntityState::Dying:
            break;

        default: break;
    }
}

bool EnemyArmadillo::NewStateRule (EntityState newState, int argsc, float argsv[]) {
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
            damageImpulse = argsv[Attack::_Impulse] - IMPULSE_MASS;
            damageDirectionX = (player.lock()->box.x < associated.box.x)? 1 : -1;
            rigidBody->SetSpeedOnX(argsv[Attack::_ForceX] * damageDirectionX);
            hp -= argsv[Attack::_Damage];
            return true;

        default: return true;
    }
}

void EnemyArmadillo::NotifyCollision (GameObject& other) {
    rigidBody->NotifyCollision(other);

    if (rigidBody->ImpactDown() and (other.label == "Ground")) {
        Collider* groundCollider = (Collider*)other.GetComponent(ComponentType::_Collider);
        currentRoute = Vec2(groundCollider->box.x, groundCollider->box.x+groundCollider->box.w);
        isGrounded = true;
    }
    if (rigidBody->ImpactLeft() or rigidBody->ImpactRight()) {
        hitWall = true;
        if (state == EntityState::Injured)
            FormatState(EntityState::Running);
    }
}
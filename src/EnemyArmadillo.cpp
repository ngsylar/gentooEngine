#include "GentooEngine.h"
#include "EnemyArmadillo.h"
#include "Attack.h"

#define SPRITE_RUN          "assets/img/enemy1.png"

#define SPEED_RUN           75.0f

#define COLLIDER_POSITION   0.0f, 14.0f
#define COLLIDER_BOX_SIZE   29.0f, 19.0f

EnemyArmadillo::EnemyArmadillo (GameObject& associated): EntityMachine(associated) {
    type = type | ComponentType::_EnemyArmadillo;
    associated.label = "Enemy";
    movementDirection = -1;
    flipIsReady = true;

    isGrounded = false;
    hitWall = false;
}

void EnemyArmadillo::Awaken () {
    Sprite* spriteRun = new Sprite(associated, SPRITE_RUN);
    Sprite* spriteDamage = new Sprite(associated, SPRITE_RUN);
    Sprite* spriteDie = new Sprite(associated, SPRITE_RUN);

    AddSpriteState(Running, spriteRun);
    AddSpriteState(Injured, spriteDamage);
    AddSpriteState(Dying, spriteDie);

    rigidBody = new RigidBody(associated);
    associated.AddComponent(rigidBody);
    // rigidBody->noInteractionLabels.push_back("Player");

    collider = new Collider(associated);
    associated.AddComponent(collider);
    collider->SetBox(Vec2(COLLIDER_POSITION), Vec2(COLLIDER_BOX_SIZE));

    Attack* attack = new Attack(associated);
    associated.AddComponent(attack);
}

void EnemyArmadillo::Start () {
    state = Running;
    rigidBody->SetSpeedOnX(SPEED_RUN * movementDirection);
}

void EnemyArmadillo::LateUpdate (float dt) {}

void EnemyArmadillo::UpdateEntity (float dt) {
    if (isGrounded and (rigidBody->GetSpeed().y > 100))
        isGrounded = false;

    switch (state) {
        case Running:
            if (isGrounded) {
                bool foundEdgeLeft = collider->box.x < currentRoute.x;
                bool foundEdgeRight = collider->box.x+collider->box.w > currentRoute.y;
                if (foundEdgeLeft or foundEdgeRight or hitWall) {
                    if (flipIsReady) {
                        movementDirection *= -1;
                        rigidBody->SetSpeedOnX(SPEED_RUN * movementDirection);
                        FlipSprite(Sprite::HORIZONTAL);
                        flipIsReady = false;
                        hitWall = false;
                    }
                } else if (not flipIsReady)
                    flipIsReady = true;
            } break;

        case Injured:
            break;

        case Dying:
            break;

        default: break;
    }
}

void EnemyArmadillo::NotifyCollision (GameObject& other) {
    rigidBody->NotifyCollision(other);

    if (rigidBody->ImpactDown() and (other.label == "Ground")) {
        Collider* groundCollider = (Collider*)other.GetComponent(ComponentType::_Collider);
        currentRoute = Vec2(groundCollider->box.x, groundCollider->box.x+groundCollider->box.w);
        isGrounded = true;
    }
    if (rigidBody->ImpactLeft() or rigidBody->ImpactRight())
        hitWall = true;
}
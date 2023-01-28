#include "GentooEngine.h"
#include "EnemyArmadillo.h"

#define SPRITE_RUN                  "assets/img/enemy1.png"
#define RIGIDBODY_VELOCITY_MAX      400.0f

EnemyArmadillo::EnemyArmadillo (GameObject& associated): Component(associated) {
    associated.label = "Enemy";
    runSpeed = 75.0f;
    type = ComponentType::_Enemy;
    movementDirection = -1;
    flipIsReady = true;
}

void EnemyArmadillo::Start () {
    sprite = new Sprite(associated, SPRITE_RUN);
    associated.AddComponent(sprite);

    rigidBody = new RigidBody(associated);
    associated.AddComponent(rigidBody);
    rigidBody->velocityMax.y = RIGIDBODY_VELOCITY_MAX;
    rigidBody->noInteractionLabels.push_back("Player");
    
    collider = new Collider(associated);
    associated.AddComponent(collider);
    collider->SetBox(Vec2(0, 14), Vec2(29, 20));
}

void EnemyArmadillo::Update (float dt) {
    rigidBody->Translate(Vec2(runSpeed * movementDirection, 0.0f) * dt);
    if (rigidBody->IsGrounded() and (not currentGround.expired())) {
        if ((collider->box.x < currentRoute.x) or (collider->box.x+collider->box.w > currentRoute.y)) {
            if (flipIsReady) {
                sprite->Flip(Sprite::HORIZONTAL);
                movementDirection *= -1;
                flipIsReady = false;
            }
        } else if (not flipIsReady)
            flipIsReady = true;
    }
}

void EnemyArmadillo::NotifyCollision (GameObject& other) {
    if ((not currentGround.expired()) and (currentGround.lock().get() == &other))
        return;

    if (rigidBody->IsGrounded() and (other.label == "Ground")) {
        currentGround = Game::GetInstance().GetCurrentState().GetObjectPtr(&other);
        Collider* groundCollider = (Collider*)currentGround.lock()->GetComponent(ComponentType::_Collider);
        currentRoute = Vec2(groundCollider->box.x, groundCollider->box.x+groundCollider->box.w);
    }
}

void EnemyArmadillo::NotifyNoCollision (GameObject& other) {
    if ((not currentGround.expired()) and (currentGround.lock().get() == &other))
        currentGround.reset();
}
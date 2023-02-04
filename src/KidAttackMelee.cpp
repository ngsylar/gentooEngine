#include "GentooEngine.h"
#include "KidAttackMelee.h"

#define COLLIDER_OFFSET_X   24.0f
#define COLLIDER_OFFSET_Y   6.0f
#define COLLIDER_SIZE       26.0f, 44.0f

KidAttackMelee::KidAttackMelee (
    GameObject& associated, GameObject* externalAssociated
): Attack(associated, externalAssociated) {
    this->externalAssociated = Game::GetInstance().GetCurrentState().GetObjectPtr(externalAssociated);
    this->originPositionX = 0.0f;
    lifetime.SetResetTime(0.16f);
    associated.enabled = false;
}

void KidAttackMelee::Awaken () {
    SetupCollider(Vec2(), Vec2(COLLIDER_SIZE));
}

void KidAttackMelee::Perform (float originPositionX) {
    this->originPositionX = originPositionX;
    associated.enabled = true;
}

void KidAttackMelee::UpdateAttack (float dt) {
    Rect externalBox = (Rect)(externalAssociated.lock()->box);

    switch (direction) {
        case LEFT:
            if (sprite != nullptr) sprite->textureFlip = SDL_FLIP_HORIZONTAL;
            associated.box.x = externalBox.x + externalBox.w - COLLIDER_OFFSET_X - associated.box.w;
            associated.box.y = externalBox.y + COLLIDER_OFFSET_Y;
            break;

        case RIGHT:
            if (sprite != nullptr) sprite->textureFlip = SDL_FLIP_NONE;
            associated.box.x = externalBox.x + COLLIDER_OFFSET_X;
            associated.box.y = externalBox.y + COLLIDER_OFFSET_Y;
            break;

        case UP: break;

        case DOWN: break;

        default: break;
    }
}
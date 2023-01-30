#include "GentooEngine.h"
#include "KidAttackMelee.h"

#define SPRITE "assets/img/attackPHoffset28_2.png"

#define OFFSET_X_DEFAULT 28.0f
#define OFFSET_Y_DEFAULT 2.0f

KidAttackMelee::KidAttackMelee (
    GameObject& associated, GameObject* externalAssociated
): Attack(associated, externalAssociated) {
    this->externalAssociated = Game::GetInstance().GetCurrentState().GetObjectPtr(externalAssociated);
    lifetime.SetResetTime(0.16f);
    enabled = false;
}

void KidAttackMelee::Awaken () {
    OpenSprite(SPRITE);
}

void KidAttackMelee::UpdateAttack (float dt) {
    Rect externalBox = (Rect)(externalAssociated.lock()->box);

    switch (direction) {
        case LEFT:
            sprite->textureFlip = SDL_FLIP_HORIZONTAL;
            associated.box.x = externalBox.x + externalBox.w - OFFSET_X_DEFAULT - associated.box.w;
            associated.box.y = externalBox.y + OFFSET_Y_DEFAULT;
            break;

        case RIGHT:
            sprite->textureFlip = SDL_FLIP_NONE;
            associated.box.x = externalBox.x + OFFSET_X_DEFAULT;
            associated.box.y = externalBox.y + OFFSET_Y_DEFAULT;
            break;

        case UP: break;

        case DOWN: break;

        default: break;
    }
}
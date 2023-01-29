#include "GentooEngine.h"
#include "KidAttackMelee.h"

#define SPRITE "assets/img/attackPHoffset28_2.png"

#define OFFSET_X 28.0f
#define OFFSET_Y 2.0f

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
    associated.box.x = externalAssociated.lock()->box.x + OFFSET_X;
    associated.box.y = externalAssociated.lock()->box.y + OFFSET_Y;
}
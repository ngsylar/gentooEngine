#ifndef KID_ATTACK_MELEE_H
#define KID_ATTACK_MELEE_H

#include "Attack.h"

class KidAttackMelee: public Attack {
    public:
        KidAttackMelee(GameObject& associated, GameObject* externalAssociated);
        void Awaken();
        void UpdateAttack(float dt);
};

#endif
#ifndef KID_ATTACK_MELEE_H
#define KID_ATTACK_MELEE_H

#include "Attack.h"

class KidAttackMelee: public Attack {
    public:
        enum AttackDirection {LEFT, RIGHT, UP, DOWN};
        AttackDirection direction;

        KidAttackMelee(GameObject& associated, GameObject* externalAssociated);
        void Awaken();
        void UpdateAttack(float dt);
};

#endif
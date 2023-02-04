#ifndef KID_ATTACK_MELEE_H
#define KID_ATTACK_MELEE_H

#include "Attack.h"

class KidAttackMelee: public Attack {
    public:
        enum AttackDirection {LEFT, RIGHT, UP, DOWN};
        AttackDirection direction;
        float originPositionX;

        KidAttackMelee(
            GameObject& associated,
            GameObject* externalAssociated
        );
        void Awaken();
        void Perform(float originPositionX);
        void UpdateAttack(float dt);
};

#endif
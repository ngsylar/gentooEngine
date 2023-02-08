#ifndef KID_ATTACK_MELEE_H
#define KID_ATTACK_MELEE_H

#include "Timer.h"
#include "AttackGeneric.h"

class KidAttackMelee: public AttackGeneric {
    private:
        Timer cameraShakeTimer;
        std::queue<Vec2> cameraShakeQueue;
        Vec2 cameraShakeReset;

        // automatic factors
        float displacement;

        void* CameraShake();

    public:
        enum AttackDirection {LEFT, RIGHT, UP, DOWN};
        AttackDirection direction;
        float originPositionX;

        KidAttackMelee(
            GameObject& associated,
            GameObject* externalAssociated
        );
        void SetProperties(Vec2 force, float impulse, int damage, float displacement);
        void Awaken();
        void Start();
        void Perform(float originPositionX);
        void UpdateAttack(float dt);
        void NotifyCollision(GameObject& other);
};

#endif
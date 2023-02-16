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
        bool impulseCancel;
        float displacement;

        void* CameraShake();

    public:
        enum AttackDirection {LEFT, RIGHT, UP, DOWN};
        AttackDirection direction;

        KidAttackMelee(
            GameObject& associated,
            GameObject* externalAssociated
        );
        void SetProperties(Vec2 force, float impulse, int damage, float displacement);
        void Awaken();
        void Start();
        void Perform();
        void Update(float dt);
        void NotifyCollision(GameObject& other);
        bool ImpulseIsCanceled();
};

#endif
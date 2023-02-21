#ifndef KID_ATTACK_MELEE_H
#define KID_ATTACK_MELEE_H

#include <queue>

#include "Timer.h"
#include "Sound.h"
#include "AttackGeneric.h"

class KidAttackMelee: public AttackGeneric {
    private:
        // camera auxiliaries
        Timer cameraShakeTimer;
        std::queue<Vec2> cameraShakeQueue;
        Vec2 cameraShakeReset;
        Sound* sound;

        // automatic signals
        bool impulseCancel, repulsionEnabled;
        float displacement;

        // automatic factors
        float repulsionOriginX, repulsionIncrease;

        // camera auxiliaries
        void* CameraShake();

    public:
        enum AttackDirection {LEFT, RIGHT, UP, DOWN};
        AttackDirection direction;

        KidAttackMelee(
            GameObject& associated,
            GameObject* externalAssociated
        );
        void Awaken();
        void Start();
        void Perform(AttackDirection direction);
        void Update(float dt);
        void NotifyCollision(GameObject& other);
        bool ImpulseIsCanceled();
};

#endif
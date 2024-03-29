#ifndef KID_ATTACK_STRONG_H
#define KID_ATTACK_STRONG_H

#include <queue>

#include "Timer.h"
#include "AttackGeneric.h"

class KidAttackStrong: public AttackGeneric {
    private:
        struct AttackType {
            Vec2 offset_i;
            Vec2 size_i;
            float lifetimeStart_i;
            float lifetimeEnd_i;
        };
        std::queue<AttackType> attackTypes;

        // automatic signals
        bool impulseCancel, repulsionEnabled;
        float displacement;

        // automatic factors
        float repulsionOriginX, repulsionIncrease;

        // timer auxiliaries
        float lifetimeStart;
        bool isOver;

        // camera auxiliaries
        Timer cameraShakeTimer;
        std::queue<Vec2> cameraShakeQueue;
        Vec2 cameraShakeReset;

        // camera auxiliaries
        void* CameraShake();

    public:
        enum AttackDirection {LEFT, RIGHT, UP, DOWN};
        AttackDirection direction;

        KidAttackStrong(GameObject& associated, GameObject* externalAssociated);
        void PushAttack(Vec2 offset, Vec2 size, float lifetimeStart, float lifetimeEnd);
        void Start();
        void Perform(AttackDirection direction);
        void Update(float dt);
        void NotifyCollision(GameObject& other);
        bool ImpulseIsCanceled();
};

#endif
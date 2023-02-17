#ifndef BOSS_ATTACK_MELEE_H
#define BOSS_ATTACK_MELEE_H

#include <queue>

#include "Timer.h"
#include "AttackGeneric.h"

class BossAttackMelee: public AttackGeneric {
    private:
        struct AttackType {
            Vec2 pureOffset_i;
            Vec2 size_i;
            float lifetimeStart_i;
            float lifetimeEnd_i;
        };
        std::queue<AttackType> attackTypes;

        // camera auxiliaries
        Timer cameraShakeTimer;
        std::queue<Vec2> cameraShakeQueue;
        Vec2 cameraShakeReset;

        // collider auxiliaries
        Vec2 pureOffset;

        // timer auxiliaries
        float lifetimeStart;

        // camera auxiliaries
        void* CameraShake();

    public:
        enum AttackDirection {LEFT, RIGHT, UP, DOWN};
        AttackDirection direction;

        BossAttackMelee(
            GameObject& associated,
            GameObject* externalAssociated
        );
        void PushAttack(Vec2 pureOffset, Vec2 size, float lifetimeStart, float lifetimeEnd);
        void Start();
        void Perform(AttackDirection direction);
        void Update(float dt);
        void CancelAttack();
        void NotifyCollision(GameObject& other);
};

#endif
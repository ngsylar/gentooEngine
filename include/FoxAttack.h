#ifndef FOX_ATTACK_H
#define FOX_ATTACK_H

#include <queue>

#include "Timer.h"
#include "AttackGeneric.h"

class FoxAttack: public AttackGeneric {
    private:
        struct AttackType {
            Vec2 offset_i;
            Vec2 size_i;
            float lifetimeStart_i;
            float lifetimeEnd_i;
        };
        std::queue<AttackType> attackTypes;

        // camera auxiliaries
        Timer cameraShakeTimer;
        std::queue<Vec2> cameraShakeQueue;
        Vec2 cameraShakeReset;
        Sound* sound;

        // timer auxiliaries
        float lifetimeStart;
        bool isOver;

        // camera auxiliaries
        void* CameraShake();

    public:
        enum AttackDirection {LEFT, RIGHT, UP, DOWN};
        AttackDirection direction;

        FoxAttack(GameObject& associated, GameObject* externalAssociated);
        void PushAttack(Vec2 offset, Vec2 size, float lifetimeStart, float lifetimeEnd);
        void Start();
        void Perform(AttackDirection direction);
        void Update(float dt);
        void NotifyCollision(GameObject& other);
};

#endif
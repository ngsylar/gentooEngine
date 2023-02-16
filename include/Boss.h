#ifndef BOSS_H
#define BOSS_H

#include "EntityMachine.h"
#include "RigidBody.h"
#include "Collider.h"
#include "Sprite.h"
#include "Timer.h"

class Boss: public EntityMachine {
    private:
        enum SubState {_Stage0, _Stage1, _Stage2, _Stage3, _Stage4};
        RigidBody* rigidBody;
        Collider* collider;
        int movementDirection;
        Vec2 currentRoute;
        int hp;

        // Behaviour
        SubState generalState, attackState;
        Timer restTimer, attackTimer, damageTimer, recoverTimer;

        // Automatic Signals
        bool isAttacking;

        // Automatic Factors
        int attackDirectionX, damageTaken;
        float attackOriginX;

        bool NewStateRule(EntityState newState, int argsc, float argsv[]);
        void AttackMeleeUpdate(float dt);

    public:
        Boss(GameObject& associated);
        void Awaken();
        void Start();
        void LateUpdate(float dt);
        void UpdateEntity(float dt);
        void RenderEntity();
        void NotifyCollision(GameObject& other);
};

#endif
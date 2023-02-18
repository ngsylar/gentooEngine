#ifndef BOSS_H
#define BOSS_H

#include "EntityMachine.h"
#include "RigidBody.h"
#include "Collider.h"
#include "Sprite.h"
#include "Timer.h"
#include "BossAttackMelee.h"

class Boss: public EntityMachine {
    private:
        enum SubState {_Stage0, _Stage1, _Stage2, _Stage3, _Stage4};
        RigidBody* rigidBody;
        Collider* collider;
        int movementDirection;
        int hp;

        // Attack Types
        BossAttackMelee* meleeAttack;

        // Behaviour
        SubState generalState, attackState;
        Timer restTimer, attackTimer, damageTimer, recoverTimer, barrierBrokenTimer;

        // Automatic Signals
        bool isAttacking, barrierIsBroken;

        // Automatic Factors
        float attackOriginX;
        int damageTaken;

        bool NewStateRule(EntityState newState, int argsc, float argsv[]);
        void AttackMeleeUpdate(float dt);
        void Die();
        Timer deathSequence;
        bool deathFade;

    public:
        static Boss* self;
        Boss(GameObject& associated);
        void Awaken();
        void Start();
        void LateUpdate(float dt);
        void UpdateEntity(float dt);
        bool BreakBarrier(float brokenTime);
        void RenderEntity();
};

#endif
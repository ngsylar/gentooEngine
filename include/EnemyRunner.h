#ifndef ENEMY_RUNNER_H
#define ENEMY_RUNNER_H

#include "EntityMachine.h"
#include "RigidBody.h"
#include "Collider.h"
#include "Sprite.h"
#include "Timer.h"

class EnemyRunner: public EntityMachine {
    private:
        RigidBody* rigidBody;
        Collider* collider;
        int movementDirection;
        Vec2 currentRoute;
        int hp;

        // Automatic Factors
        Timer edgeTimer, toggleTimer, recoverTimer;
        float damageOriginX, damageImpulse;
        int damageDirectionX;

        // Collision Faces
        bool isGrounded, hitWall;

        bool NewStateRule(EntityState newState, int argsc, float argsv[]);

    public:
        EnemyRunner(GameObject& associated);
        void Awaken();
        void Start();
        void LateUpdate(float dt);
        void UpdateEntity(float dt);
        void NotifyCollision(GameObject& other);
};

#endif
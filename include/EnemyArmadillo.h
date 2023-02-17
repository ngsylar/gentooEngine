#ifndef ENEMY_ARMADILLO_H
#define ENEMY_ARMADILLO_H

#include "EntityMachine.h"
#include "RigidBody.h"
#include "Collider.h"
#include "Sprite.h"

class EnemyArmadillo: public EntityMachine {
    private:
        RigidBody* rigidBody;
        Collider* collider;
        int movementDirection;
        Vec2 currentRoute;
        int hp;

        // Automatic Factors
        Timer turnTimer;
        float damageOriginX, damageImpulse;
        int damageDirectionX;

        // Collision Faces
        bool isGrounded, hitWall;

        bool NewStateRule(EntityState newState, int argsc, float argsv[]);

    public:
        EnemyArmadillo(GameObject& associated);
        void Awaken();
        void Start();
        void LateUpdate(float dt);
        void UpdateEntity(float dt);
        void FlipEntity();
        void NotifyCollision(GameObject& other);
};

#endif
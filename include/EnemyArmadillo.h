#ifndef ENEMY_ARMADILLO_H
#define ENEMY_ARMADILLO_H

#include "Component.h"
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
        float damageOriginX;
        int damageDirectionX;

        // Collision Faces
        bool isGrounded, hitWall;

        bool NewStateRule(EntityState newState, int& argument);

    public:
        EnemyArmadillo(GameObject& associated);
        void Awaken();
        void Start();
        void LateUpdate(float dt);
        void UpdateEntity(float dt);
        void NotifyCollision(GameObject& other);
};

#endif
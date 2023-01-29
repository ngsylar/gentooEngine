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
        bool flipIsReady;
        Vec2 currentRoute;

        // Collision Faces
        bool isGrounded, hitWall;

    public:
        EnemyArmadillo(GameObject& associated);
        void Awaken();
        void Start();
        void LateUpdate(float dt);
        void UpdateEntity(float dt);
        void NotifyCollision(GameObject& other);
};

#endif
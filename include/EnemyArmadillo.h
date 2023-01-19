#ifndef ENEMY_ARMADILLO_H
#define ENEMY_ARMADILLO_H

#include "Component.h"
#include "RigidBody.h"
#include "Collider.h"
#include "Sprite.h"

class EnemyArmadillo: public Component {
    private:
        Sprite* sprite;
        RigidBody* rigidBody;
        Collider* collider;
        float runSpeed;
        int movementDirection;
        bool flipIsReady;

        std::weak_ptr<GameObject> currentGround;
        Vec2 currentRoute;

    public:
        EnemyArmadillo(GameObject& associated);
        void Start();
        void Update(float dt);
        void NotifyCollision(GameObject& other);
        void NotifyNoCollision(GameObject& other);
};

#endif
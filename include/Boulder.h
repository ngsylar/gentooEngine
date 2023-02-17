#ifndef MOVABLE_BOULDER_H
#define MOVABLE_BOULDER_H

#include "EntityMachine.h"
#include "RigidBody.h"
#include "Collider.h"
#include "Sprite.h"

class Boulder: public EntityMachine {
    private:
        RigidBody* rigidBody;
        Collider* collider;
        int movementDirection;

    public:
        Boulder(GameObject& associated, std::string file, Vec2 position);
        void Awaken();
        void Start();
        void LateUpdate(float dt);
        void UpdateEntity(float dt);
        void NotifyCollision(GameObject& other);
};

#endif//MOVABLE_BOULDER_H
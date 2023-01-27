#ifndef ATTACK_H
#define ATTACK_H

#include "Component.h"
#include "Collider.h"
#include "Sprite.h"

class Attack: public Component {
    private:
        Sprite* sprite;
        Collider* collider;

    public:
        Attack(GameObject& associated);
        void Awaken();
        void Start();
        void Update(float dt);
        void NotifyCollision(GameObject& other);
};

#endif
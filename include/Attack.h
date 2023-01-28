#ifndef ATTACK_H
#define ATTACK_H

#include "Component.h"
#include "Collider.h"
#include "Sprite.h"

class Attack: public Component {
    private:
        std::weak_ptr<GameObject> externalAssociated;
        bool usingExternalAssociated;
        Collider* collider;
        Sprite* sprite;

    public:
        Attack(
            GameObject& associated,
            GameObject* externalAssociated=nullptr
        );
        void OpenSprite(
            std::string file,
            int frameCount=1,
            float frameTime=0.0f,
            bool frameOneshot=false,
            bool selfDestruction=false
        );
        void SetupCollider(Vec2 offset, Vec2 size);
        void Update(float dt);
        void NotifyCollision(GameObject& other);
        bool UsingInternalAssociated();
        bool UsingExternalAssociated();
        bool Is(std::string type);
        bool Is(ComponentType type);
};

#endif
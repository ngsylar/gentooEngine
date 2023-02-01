#ifndef ATTACK_H
#define ATTACK_H

#include "Component.h"
#include "Collider.h"
#include "Sprite.h"
#include "Timer.h"

class Attack: public Component {
    protected:
        std::weak_ptr<GameObject> externalAssociated;
        bool usingExternalAssociated;
        Collider* collider;
        Sprite* sprite;

    public:
        Timer lifetime;

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
        virtual void Awaken();
        virtual void Start();
        void Update(float dt);
        virtual void UpdateAttack(float dt);
        void NotifyCollision(GameObject& other);
        bool UsingInternalAssociated();
        bool UsingExternalAssociated();
        bool Is(std::string type);
        bool Is(ComponentType type);
};

#endif
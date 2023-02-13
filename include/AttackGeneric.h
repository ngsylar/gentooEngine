#ifndef ATTACK_GENERIC_H
#define ATTACK_GENERIC_H

#include "Component.h"
#include "Collider.h"
#include "Sprite.h"
#include "Timer.h"

class AttackGeneric: public Component {
    protected:
        std::weak_ptr<GameObject> externalAssociated;
        bool usingExternalAssociated;
        Collider* collider;
        Sprite* sprite;
        Vec2 force;
        float impulse;
        int damage;

    public:
        enum Argument {_ForceX, _ForceY, _Impulse, _Damage, _OriginX, _OriginY};
        Timer lifetime;

        AttackGeneric(
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
        void SetProperties(Vec2 force, float impulse, int damage);
        virtual void Awaken();
        virtual void Start();
        virtual void Perform();
        virtual void PerformAttack();
        virtual void Update(float dt);
        virtual void UpdateAttack(float dt);
        virtual void NotifyCollision(GameObject& other);
        bool UsingInternalAssociated();
        bool UsingExternalAssociated();
        bool Is(std::string type);
        bool Is(ComponentType type);
};

#endif
#ifndef ENTITYMACHINE
#define ENTITYMACHINE

#include "Component.h"
#include "Sprite.h"
#include "GlobalDefinitions.h"


class EntityMachine : public Component
{
    protected:
        EntityState currState;
        std::vector<Sprite*> entitySprite;

    public:
        EntityMachine(GameObject& associated);
        virtual ~EntityMachine();
        void AddEntityStateSprite(EntityState NewState, Sprite* NewSheet);
        void RemoveEntityState(EntityState NewState);
        bool HasEntityState(EntityState Compare);
        void SetEntityState(EntityState NewState);
        EntityState GetCurrEntityState();
        
        //Use these function bodies to describe your entity actions
        virtual void StartEntity();
        virtual void UpdateEntity(float dt);
        virtual void LateUpdateEntity(float dt);
        virtual void RenderEntity();
    
        //Inheritance
        virtual void Start();
        virtual void Update(float dt);
        virtual void LateUpdate(float dt);
        virtual void Render();

        virtual void NotifyCollision(GameObject& other);
        virtual void NotifyNoCollision(GameObject& other);
        virtual bool Is(ComponentType type);

};

#endif// ENTITYMACHINE
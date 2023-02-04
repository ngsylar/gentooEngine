#ifndef ENTITY_MACHINE_H
#define ENTITY_MACHINE_H

#include "Component.h"
#include "Sprite.h"

class EntityMachine: public Component {
    protected:
        EntityState state;
        std::vector<std::unique_ptr<Sprite>> sprites;
        SDL_RendererFlip textureFlip;

        virtual bool NewStateRule(EntityState newState, int argsc, float argsv[]);
        void FlipSprite(Sprite::Axis axis);

    public:
        EntityMachine(GameObject& associated);
        ~EntityMachine();
        void AddSpriteState(EntityState newState, Sprite* newSheet);
        void RemoveState(EntityState state);
        bool HasState(EntityState compare);
        bool FormatState(EntityState newState, int argsc=0, float argsv[]=nullptr);
        EntityState GetCurrentState();

        // Inheritance
        virtual void Awaken();
        virtual void Start();
        virtual void Update(float dt);
        virtual void LateUpdate(float dt);
        virtual void Render();

        // Use these function bodies to describe your entity actions
        virtual void AwakenEntity();
        virtual void StartEntity();
        virtual void UpdateEntity(float dt);
        virtual void LateUpdateEntity(float dt);
        virtual void RenderEntity();

        // Collision and Type
        virtual void NotifyCollision(GameObject& other);
        virtual void NotifyNoCollision(GameObject& other);
        virtual bool Is(ComponentType type);
};

#endif
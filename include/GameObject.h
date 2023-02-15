#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <memory>
#include <string>
#include <vector>

#include "GlobalDefinitions.h"
#include "Rect.h"

class Component;

class GameObject {
    private:
        std::vector<std::unique_ptr<Component>> components;
        ComponentType contains;
        bool awake, isDead;

    public:
        bool enabled;
        std::string label;
        int layer;
        Rect box;
        float angleDeg;

        GameObject(int layer=0, std::string label="");
        ~GameObject();
        void SetLayer(int layer);
        void Awaken();
        void Start();
        void Update(float dt);
        void LateUpdate(float dt);
        void Render();
        bool Contains(ComponentType type);
        bool IsDead();
        void RequestDelete();
        void AddComponent(Component* cpt);
        void RemoveComponent(Component* cpt);
        Component* GetComponent(std::string type);
        Component* GetComponent(ComponentType type);
        void NotifyCollision(GameObject& other);
        void NotifyNoCollision(GameObject& other);
        
        static bool CompareLayers(
            std::weak_ptr<GameObject>& goA,
            std::weak_ptr<GameObject>& goB
        );

        // Magnific gambiarra
        void SignalTerrain();

        // melius colliders' pixel correction
        // note: if pcf0 is true then (collider->box.x - 1) is applied
        //       if pcf1 is true then pcf1 is inverted (set true when the sprite is facing right)
        int pixelColliderFix0, pixelColliderFix1;
};

#endif
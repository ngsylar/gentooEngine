#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <memory>
#include <string>
#include <vector>

#include "Rect.h"
#include "GlobalDefinitions.h"

class Component;

class GameObject {
    private:
        std::vector<std::unique_ptr<Component>> components;
        bool awake, isDead;
        ComponentType contains;

    public:
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
        
        // sylar's extra layer rendering
        static bool CompareLayers(
            std::weak_ptr<GameObject>& goA,
            std::weak_ptr<GameObject>& goB
        );

        //Magnific gambiarra
        void SignalTerrain();
};

#endif
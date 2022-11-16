#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <memory>
#include <string>
#include <vector>

#include "Rect.h"

class Component;

class GameObject {
    private:
        std::vector<std::unique_ptr<Component>> components;
        bool started, isDead;
    
    public:
        std::string label;
        int layer;          // sylar's extra layer rendering
        Rect box;
        float angleDeg;

        GameObject(int layer=0, std::string label="");
        ~GameObject();
        void Start();
        void Update(float dt);
        void Render();
        bool IsDead();
        void RequestDelete();
        void AddComponent(Component* cpt);
        void RemoveComponent(Component* cpt);
        Component* GetComponent(std::string type);
        void NotifyCollision(GameObject& other);
        
        // sylar's extra layer rendering
        static bool CompareLayers(
            std::weak_ptr<GameObject>& goA,
            std::weak_ptr<GameObject>& goB
        );
};

#endif
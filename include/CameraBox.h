#ifndef CAMERA_BOX_H
#define CAMERA_BOX_H

#include <array>
#include <utility>

#include "Component.h"

class CameraBox: public Component {
    public:
        enum Axis {X, Y};
        enum ContactSide {NONE, NEGATIVE, POSITIVE};
        std::array<ContactSide, 2> contacts;
        bool isDynamic;
        Vec2 lastDisplacement;
        Vec2 dynamicFactor;

        CameraBox(
            GameObject& associated,
            GameObject* focus,
            float spacingX=20.0f,
            float spacingY=20.0f
        );
        void Follow(
            GameObject* focus,
            float spacingX=20.0f,
            float spacingY=20.0f
        );
        void Setup();
        void Start();
        void Update(float dt);
        void Render();
        bool Is(std::string type); 
        void AddMethod(Component* component, std::function<void*()> method);
        void RemoveMethod(Component* component);
        void ClearMethods();
        ~CameraBox();

    private:
        std::weak_ptr<GameObject> focus;
        std::vector<std::pair<Component*, std::function<void*()>>> foreignMethods;

        std::pair<ContactSide, float> DetectMotion(
            float focusPoint, float focusSize,
            float selfPoint, float selfSize
        );
        void MoveDynamically(
            float& focusPoint, float& focusSize, float& selfSize,
            float& selfPoint, float& dynamicPoint,
            float& lastDisplacement, float& destination
        );
};

#endif
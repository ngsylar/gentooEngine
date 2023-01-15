#ifndef CAMERA_BOX_H
#define CAMERA_BOX_H

#include <array>
#include <utility>

#include "Component.h"

class CameraBox: public Component {
    public:
        enum Axis {X, Y};
        enum ContactSide {NONE, NEGATIVE, POSITIVE};
        Rect focusBoxOffset;
        bool isDynamic;
        Vec2 dynamicFactor;

        CameraBox(
            GameObject& associated,
            GameObject* focus,
            float spacingX=20.0f,
            float spacingY=20.0f
        );
        void Start();
        void Follow(
            GameObject* focus,
            float spacingX=20.0f,
            float spacingY=20.0f
        );
        void Update(float dt);
        ContactSide GetContact(Axis axis);
        void Render();
        bool Is(std::string type); 
        bool Is(GameObjID type); 
        void AddMethod(Component* component, std::function<void*()> method);
        void RemoveMethod(Component* component);
        void ClearMethods();
        ~CameraBox();

    private:
        std::vector<std::pair<Component*, std::function<void*()>>> foreignMethods;
        std::weak_ptr<GameObject> focus;
        Rect focusBox;
        std::array<ContactSide, 2> contacts;
        Vec2 lastDisplacement;

        std::pair<ContactSide, float> DetectContact(
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
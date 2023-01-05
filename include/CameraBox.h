#ifndef CAMERA_BOX_H
#define CAMERA_BOX_H

#include <utility>

#include "Component.h"

class CameraBox: public Component {
    private:
        std::weak_ptr<GameObject> focus;
        Vec2 lastDisplacement;

        std::pair<bool, float> DetectCollision(
            float focusPoint, float focusSize,
            float selfPoint, float selfSize
        );
        void MoveDynamically(
            float& focusPoint, float& focusSize, float& selfSize,
            float& selfPoint, float& dynamicPoint,
            float& lastDisplacement, float& destination
        );

    public:
        bool isDynamic;

        CameraBox(
            GameObject& associated,
            GameObject* focus,
            float spacingX=20.0f,
            float spacingY=20.0f,
            bool isDynamic=false
        );
        void Follow(
            GameObject* focus,
            float spacingX=20.0f,
            float spacingY=20.0f,
            bool isDynamic=false
        );
        void Start();
        void Update(float dt);
        void Render();
};

#endif
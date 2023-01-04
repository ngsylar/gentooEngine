#ifndef CAMERA_SQUARE_H
#define CAMERA_SQUARE_H

#include "Component.h"

class CameraSquare: public Component {
    private:
        std::weak_ptr<GameObject> focus;

    public:
        CameraSquare(
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
        void Start();
        void Update(float dt);
        void Render();
};

#endif
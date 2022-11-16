#ifndef CAMERA_FOLLOWER_H
#define CAMERA_FOLLOWER_H

#include "Component.h"

class CameraFollower: public Component {
    public:
        Vec2 offset;
        
        CameraFollower(GameObject& go);
        void Update(float dt);
        void Render();
        bool Is(std::string type);
};

#endif
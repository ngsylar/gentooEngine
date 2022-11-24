#ifndef CAMERA_FOLLOWER_H
#define CAMERA_FOLLOWER_H

#include "Component.h"

class CameraFollower: public Component {
    public:
        Vec2 offset;
        
        CameraFollower(GameObject& go);
        void LateUpdate(float dt);
        bool Is(std::string type);
};

#endif
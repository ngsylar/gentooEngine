#ifndef KID_H
#define KID_H

#include "Component.h"
#include "RigidBody.h"

class Kid: public Component {
    private:
        enum Action {IDLE, RUN};
        Sprite* sprite;
        RigidBody* rigidBody;
        Collider* collider;
        float runSpeed, jumpForce, jumpHeight, jumpHeightMax;
        bool isJumping;

    public:
        Kid(GameObject& associated);
        void Start();
        void Update(float dt);
        void StartJump(float dt);
        void HandleJump(bool isKeyDown, float dt);
        void CameraCheckTracking(float dt);
};

#endif
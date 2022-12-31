#ifndef KID_H
#define KID_H

#include "Component.h"
#include "RigidBody.h"
#include "Collider.h"
#include "Sprite.h"

class Kid: public Component {
    private:
        enum Action {IDLE, RUN};
        Sprite* sprite;
        RigidBody* rigidBody;
        Collider* collider;
        float runSpeedMax, runAcceleration, jumpForce, jumpHeightMax;
        float runSpeedNow, jumpHeightNow;
        bool isJumping;


    public:
        Kid(GameObject& associated);
        void Start();
        void Update(float dt);
        bool Run(float displacement, SDL_RendererFlip flip=SDL_FLIP_NONE);
        void StartJump(float displacement);
        void HandleJump(bool isKeyDown, float dt);
        void CameraCheckTracking(float dt);
};

#endif
#ifndef KID_H
#define KID_H

#include "Timer.h"
#include "Component.h"
#include "RigidBody.h"
#include "Collider.h"
#include "Sprite.h"

class Kid: public Component {
    private:
        enum Action {IDLE, RUN, JUMP, FALL};
        Sprite* sprite;
        RigidBody* rigidBody;
        Collider* collider;
        GameObject* cameraBox;
        Timer cameraGroundedTimer;
        Action status;
        float runSpeedMax, runAcceleration, jumpForce, jumpHeightMax;
        float runSpeedNow, jumpHeightNow;
        bool jumpingIsConstant;

    public:
        Kid(GameObject& associated);
        bool Is(GameObjID type);
        void Start();
        void Update(float dt);
        void Run(float displacement, SDL_RendererFlip flip=SDL_FLIP_NONE);
        void StartJump(float displacement);
        void HandleJump(bool isKeyDown, float dt);
        void* CameraCheckGrounded();
        void DebugScript(float dt);
};

#endif
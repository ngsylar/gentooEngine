#ifndef KID_H
#define KID_H

#include "Timer.h"
#include "Component.h"
#include "RigidBody.h"
#include "Collider.h"
#include "Sprite.h"

class Kid: public Component {
    private:
        enum Action {IDLE, RUN, JUMP, FALL, DAMAGE};
        Sprite* sprite;
        RigidBody* rigidBody;
        Collider* collider;
        Action status;

        float runSpeedMax, runAcceleration, jumpForce, jumpHeightMax;
        float runSpeedNow, jumpHeightNow;
        bool jumpingIsConstant;

        Vec2 damageOrigin, damageDisplacement;
        Timer invincibilityIimer;
        bool isInvincible;

        // Camera Assistants
        GameObject* cameraBox;
        Timer cameraGroundedTimer, cameraShakeTimer;
        std::queue<Vec2> cameraShakeQueue;
        Vec2 cameraShakeReset;

    public:
        Kid(GameObject& associated);
        void Awaken();
        void Start();
        void Update(float dt);
        void Run(float displacement, SDL_RendererFlip flip=SDL_FLIP_NONE);
        void StartJump(float displacement);
        void HandleJump(bool isKeyDown, float dt);
        void TakeDamage(float dt);
        void NotifyCollision(GameObject& other);
        void AnimationShake();
        bool Is(ComponentType type);
        void* CameraEffects();

        // remover
        void DebugScript(InputManager& input, float dt);
};

#endif
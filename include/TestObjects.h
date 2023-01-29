#ifndef TEST_REDSQUARE_H
#define TEST_REDSQUARE_H

#include "Timer.h"
#include "InputManager.h"
#include "RigidBodyLegacy.h"
#include "Component.h"

class Ball: public Component {
    private:
        RigidBodyLegacy* rigidBody;
        float runSpeed, jumpForce, jumpHeight, jumpHeightMax;
        bool isJumping;

        // cinemachine assistant
        bool isFalling, isClimbing;
        Timer cameraDelay;
        Vec2 cameraDistance;
        float cameraAcceleration, cameraOffset;

    public:
        Ball(GameObject& associated);
        void Start();
        void Update(float dt);
        void StartJump(float dt);
        void HandleJump(bool isKeyDown, float dt);
        void CameraCheckTracking(float dt);

        bool Is(ComponentType type);

        // cinemachine assistant
        void CameraHandleFall(float dt);
};

#endif
#ifndef TEST_OBJECTS_H
#define TEST_OBJECTS_H

#include "Timer.h"
#include "InputManager.h"
#include "Component.h"

class Ball: public Component {
    private:
        RigidBody* rigidBody;
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

        // cinemachine assistant
        void CameraHandleFall(float dt);
};

#endif
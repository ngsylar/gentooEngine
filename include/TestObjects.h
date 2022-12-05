#ifndef TEST_OBJECTS_H
#define TEST_OBJECTS_H

#include <queue>

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

class Platform: public Component {
    private:
        bool bodied, isActive;
        Vec2 displacement;
    
    public:
        enum Direction {LEFT, RIGHT, UP, DOWN, NONE};
        Direction direction;
        float positionLimit;

        Platform(GameObject& associated);
        Platform(GameObject& associated, Direction direction, float positionLimit, bool bodied=false);
        ~Platform();
        void SetBody(bool body);
        void Start();
        void NotifyCollision(GameObject& other);
        void NotifyNoCollision(GameObject& other);
        void* HandleCameraWithBody();
        void* HandleCamera();
};

#endif
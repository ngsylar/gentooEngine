#ifndef TEST_OBJECTS_H
#define TEST_OBJECTS_H

#include <array>
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
        Vec2 displacement;
        float relativeOffset;
        
    public:
        enum TriggerAction {MOVE, UNDO};
        TriggerAction activeRect;
        std::array<Rect, 2> colliderRects;
        
        enum Direction {LEFT, RIGHT, UP, DOWN};
        Direction direction;
        float positionLimit;
        bool movementIsOver;
        bool bodied;

        Platform(GameObject& associated, Direction direction, float positionLimit, bool body=false);
        ~Platform();
        void SetTrigger(int rectId);
        void Start();
        void NotifyCollision(GameObject& other);
        void* MoveCamera();
        void* UndoCameraMovement();
        void* LimitCamera();
};

#endif
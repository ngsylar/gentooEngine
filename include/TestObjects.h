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

    public:
        Ball(GameObject& associated);
        void Start();
        void Update(float dt);
        void StartJump(float dt);
        void HandleJump(bool isKeyDown, float dt);
        void HandleFall();
        void CheckTracking();
};

class Ground: public Component {
    private:
        Timer delay;

    public:
        // float relativeOffset;

        Ground(GameObject& associated, float relativeOffset=0.0f);
        ~Ground();
        void Start();
        void* HandleCamera();
};

#endif
#ifndef TEST_OBJECTS_H
#define TEST_OBJECTS_H

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
};

class LoopedBackground: public Component {
    private:
        Sprite* sprite;
        int halfSize, renderingCount, middlePositionId;
    
    public:
        LoopedBackground(
            GameObject& associated,
            std::string fileName,
            int layerCount=2
        );
        ~LoopedBackground();
        void SetLayerCount(int layerCount);
        void Start();
        void LateUpdate(float dt);
        void Render();
};

#endif
#ifndef TEST_OBJECTS_H
#define TEST_OBJECTS_H

#include "Component.h"

class Ball: public Component {
    private:
        RigidBody* rigidBody;
        Timer jumpTimer;
        float runSpeed, jumpForce;
        bool isJumping;

    public:
        Ball (GameObject& associated);
        void Start ();
        void Update (float dt);
};

#endif
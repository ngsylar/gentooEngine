#ifndef TEST_OBJECTS_H
#define TEST_OBJECTS_H

#include "Component.h"

class Ball: public Component {
    private:
        RigidBody* rigidBody;
        float jumpForce, jumpPseudoTimer;
        bool isJumping;

    public:
        Ball (GameObject& associated);
        void Start ();
        void Update (float dt);
};

#endif
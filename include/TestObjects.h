#ifndef TEST_OBJECTS_H
#define TEST_OBJECTS_H

#include "Component.h"

class Ball: public Component {
    private:
        RigidBody* rigidBody;
    public:
        Ball (GameObject& associated);
        void Start ();
        void Update (float dt);
};

#endif
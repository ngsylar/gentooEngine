#ifndef CAMERA_H
#define CAMERA_H

#include "GameObject.h"

#define CAMERA_SPEED 200.0f

class Camera {
    private:
        enum MovementDirection {NONE, UP, DOWN, LEFT, RIGHT};

        static GameObject* focus;
        static Vec2 focusPreviousPosition;
        static float acceleration;
    
    public:
        static Vec2 pos, speed;
        static Vec2 deadZoneLength;
        static bool freeCamera;
        static Vec2 focusLastDirection;

        static void Follow(GameObject* newFocus);
        static void Unfollow();
        static void EnableFree();
        static void DisableFree();
        static void Cinemachine(float dt);
        static void Update(float dt);
        static void Reset();
        static Vec2 GetPosition();
};

#endif
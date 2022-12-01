#ifndef CAMERA_H
#define CAMERA_H

#include "GameObject.h"

#define CAMERA_SPEED 200.0f

class Camera {
    private:
        class Cinemachine;

        enum Axis {HORIZONTAL, VERTICAL};
        static GameObject* focus;
        static Vec2 posAdjustment;

        struct Player {
            enum FaceDirection {NONE, UP, DOWN, LEFT, RIGHT};
            Vec2 position, previousPosition;
            Vec2 lastDirection;
        };

    public:
        static Vec2 pos, velocity, offset, screenOffset;
        static bool isLocked[2];

        static Cinemachine cinemachine;
        static Player player;

        static void Follow(GameObject* newFocus);
        static void Unfollow();
        static void EnableFree();
        static void DisableFree();
        static void Update(float dt);
        static void Reset();
        static Vec2 GetPosition();
};

class Camera::Cinemachine {
    public:
        Vec2 length;
        std::array<int, 2> slices, deadSlices;

        // void SetValues();
        void Accelerate(float focusVelocity, float displacement);
        void Decelerate(float focusVelocity, float displacement);
        void Chase(
            float length, float centerDistance,
            float safeZone, float slicedLength,
            float playerVelocity, float playerDirection
        );
        void Update(float dt);
};

#endif
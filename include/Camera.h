#ifndef CAMERA_H
#define CAMERA_H

#include "GameObject.h"

#define CAMERA_SPEED 200.0f

class Camera {
    private:
        enum Axis {HORIZONTAL, VERTICAL};
        enum FaceDirection {NONE, UP, DOWN, LEFT, RIGHT};

        static GameObject* focus;
        static Vec2 posAdjustment;

        class Cinemachine;
        struct Player {
            Vec2 position, previousPosition;
            std::array<int, 2> lastDirection;
            Vec2 lastVelocity;
            std::array<bool, 2> isStopping;
        };

    public:
        static float tolerance;
        static Vec2 pos, velocity, offset, screenOffset;
        static std::array<bool, 2> isLocked;

        static Cinemachine cinemachine;
        static Player player;

        static void Follow(
            GameObject* newFocus,
            Vec2 cinemachineLength=Vec2(),
            int slicesX=8, int slicesY=0,
            int deadSlicesX=2, int deadSlicesY=0,
            int focusDirectionX=NONE, int focusDirectionY=NONE
        );
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
        void Accelerate(float* velocity, float focusVelocity, float displacement);
        void Decelerate(float* velocity, float focusVelocity, float displacement);
        void Chase(
            float* velocity, float* offset,
            float length, float centerDistance,
            float safeZone, float slicedLength,
            float playerVelocity, float playerDirection
        );
        void StopChasing(
            bool* flag, float* offset,
            float length, float centerDistance,
            float playerVelocity, float playerDirection
        );
        void Update(float dt);
};

#endif
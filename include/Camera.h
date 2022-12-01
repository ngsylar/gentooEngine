#ifndef CAMERA_H
#define CAMERA_H

#include "GameObject.h"

#define CAMERA_SPEED 200.0f

class Camera {
    private:
        enum Axis {X, Y};
        enum FaceDirection {LEFT, RIGHT, UP, DOWN, NONE};

        static GameObject* focus;
        static Vec2 posAdjustment;

        class Cinemachine;
        struct Player {
            Vec2 position, previousPosition;
            std::array<int, 2> lastDirection;
            Vec2 lastVelocity;
            std::array<bool, 2> isStopping;
            std::array<Timer, 2> stopwatch;
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
            int slicesX=8, int slicesY=32,
            int deadSlicesX=2, int deadSlicesY=28,
            int focusDirectionX=NONE, int focusDirectionY=NONE
        );
        static void Unfollow();
        static void Update(float dt);
        static void Reset();
        static Vec2 GetPosition();
};

class Camera::Cinemachine {
    public:
        Vec2 length;
        std::array<int, 2> slices, deadSlices;
        std::array<bool, 4> isIgnored;

        // void SetUp(
        //     bool isIgnoredLeft, bool isIgnoredRight,
        //     bool isIgnoredUp, bool isIgnoredDown
        // );
        void Accelerate(float* velocity, float focusVelocity, float displacement);
        void Decelerate(float* velocity, float focusVelocity, float displacement);
        void Chase(
            float* velocity, float* offset,
            float length, float centerDistance,
            float undeadZone, float slicedLength,
            float playerVelocity, int axis, float playerDirection
        );
        void StopChasing(
            float* offset, float length, float centerDistance,
            float playerVelocity, int axis, float playerDirection, float dt
        );
        void Update(float dt);
};

#endif
#ifndef CAMERA_H
#define CAMERA_H

#include <array>

#include "GameObject.h"

class Camera {
    private:
        static std::vector<std::pair<Component*, std::function<void*()>>> foreignMethods;
        static Vec2 posAdjustment;
        static GameObject* focus;

        class Cinemachine;
        struct Player {
            Vec2 position, previousPosition;
            std::array<int, 2> lastDirection;
            Vec2 lastVelocity;
            
            std::array<bool, 2> isStopping;
            std::array<Timer, 2> stopwatch;
        };

    public:
        enum Axis {X, Y};
        enum FocusDirection {LEFT, RIGHT, UP, DOWN, NONE};

        static float tolerance;
        static Vec2 pos, velocity, offset, screenOffset, masterOffset;
        static std::array<bool, 2> isLocked;

        static Cinemachine cinemachine;
        static Player player;

        static void AddMethod(Component* component, std::function<void*()> method);
        static void RemoveMethod(Component* component);
        static void Follow(
            GameObject* newFocus, Vec2 cinemachineLength=Vec2(),
            int slicesX=8, int slicesY=8, int deadSlicesX=2, int deadSlicesY=2,
            FocusDirection focusDirectionX=NONE, FocusDirection focusDirectionY=NONE,
            Vec2 focusOffset=Vec2()
        );
        static void Unfollow();
        static void Update(float dt);
        static Vec2 GetPosition();
        static void Reset();
        static void ClearMethods();
};

class Camera::Cinemachine {
    public:
        Vec2 length;
        std::array<int, 2> slices, deadSlices;
        std::array<bool, 4> isAxisEnabled;
        std::array<bool, 4> isAxisResettable;

        void Setup(
            bool enableLeft, bool enableRight, bool enableUp, bool enableDown,
            bool resetLeft, bool resetRight, bool resetUp, bool resetDown,
            int slicesX, int slicesY, int deadSlicesX, int deadSlicesY
        );
        void Setup(
            bool enableLeft, bool enableRight, bool enableUp, bool enableDown,
            bool resetLeft, bool resetRight, bool resetUp, bool resetDown
        );
        void Accelerate(float* velocity, float focusVelocity, float displacement);
        void Decelerate(float* velocity, float focusVelocity, float displacement);
        void Chase(
            float* velocity, float* offset,
            float length, float centerDistance, float undeadZone, float slicedLength,
            float playerVelocity, int axis, float playerDirection
        );
        void StopChasing(
            float* velocity, float* offset, float length, float centerDistance,
            int axis, float playerDirection, float dt
        );
        void Update(float dt);
};

#endif
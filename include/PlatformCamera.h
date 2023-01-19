#ifndef PLATFORM_CAMERA_H
#define PLATFORM_CAMERA_H

#include <array>

#include "Component.h"

class PlatformCamera: public Component {
    private:
        Vec2 targetDisplacement, relativeOffset;
        bool bodied, isMoved;
        
    public:
        enum TriggerAction {MOVE, UNDO};
        std::array<Rect, 2> colliderRects;
        TriggerAction activeRect;
        
        enum Direction {LEFT, RIGHT, UP, DOWN};
        Direction direction;
        float positionLimit;

        PlatformCamera(GameObject& associated, Direction direction, float positionLimit);
        ~PlatformCamera();
        void SetTrigger(Rect rectToMove, Rect rectToUndo);
        void Start();
        void NotifyCollision(GameObject& other);
        void* MoveCamera();
        void* UndoCameraMovement();
        void* LimitCamera();
        bool Is(ComponentType type);
};

#endif
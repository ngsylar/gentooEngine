#ifndef LOOPED_BACKGROUND_H
#define LOOPED_BACKGROUND_H

#include "Component.h"
#include "Sprite.h"

class LoopedBackground: public Component {
    private:
        enum Axis {HORIZONTAL, VERTICAL};
        Sprite* sprite;
        Vec2 cameraInitialPosition, cameraAdjustment;
        int halfSizes[2], renderingCount, middlePositionId;
    
    public:
        Vec2 parallaxFactor;
        
        LoopedBackground(
            GameObject& associated,
            std::string fileName,
            int layerCount=2,
            Vec2 parallaxFactor=Vec2(1.0f, 1.0f)
        );
        ~LoopedBackground();
        void SetLayerCount(int layerCount);
        void SetPosition(float positionX, float positionY);
        void SetPosition(Vec2 position);
        void Start();
        void* LateStart();
        void LateUpdate(float dt);
        void Render();
        bool Is(ComponentType type);
};

#endif
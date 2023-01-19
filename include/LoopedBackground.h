#ifndef LOOPED_BACKGROUND_H
#define LOOPED_BACKGROUND_H

#include "Component.h"
#include "Sprite.h"

class LoopedBackground: public Component {
    private:
        enum Axis {HORIZONTAL, VERTICAL};

        Sprite* sprite;
        int halfSizes[2], renderingCount, middlePositionId;
    
    public:
        float parallaxFactor;
        
        LoopedBackground(
            GameObject& associated,
            std::string fileName,
            int layerCount=2,
            float parallaxFactor=1.0f
        );
        ~LoopedBackground();
        void SetLayerCount(int layerCount);
        void Start();
        void LateUpdate(float dt);
        void Render();
        bool Is(ComponentType type);
};

#endif
#ifndef HUD_H
#define HUD_H

#include "GentooEngine.h"

class Hud : public Component {
    private:
        Sprite* manaBar;
        Sprite* healthBar;
        Sprite* bossFlask;
        Sprite* bossHP;

    public:
        Hud(GameObject& associated);
        ~Hud();
        void Start();
        void Update(float dt);
        // void Render();
        bool Is(std::string type);
        bool Is(ComponentType type);

};


#endif// HUD_H
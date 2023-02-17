#include "Hud.h"
#include "GameData.h"
#include "Kid.h"

#define MANA_BAR "assets/img/hud/espada_106x40_15f.png"
#define HEALTH_BAR "assets/img/hud/hp_106x40_7f.png"

Hud::Hud(GameObject& associated): Component(associated) {

}

Hud::~Hud(){}

void Hud::Start(){
    GameObject* Mana = new GameObject(LayerDistance::_HUD_Far);
    manaBar = new Sprite(*Mana,MANA_BAR,15);
    Mana->box.SetSize(manaBar->GetWidth(), manaBar->GetHeight());
    Mana->AddComponent(manaBar);
    CameraFollower* cf = new CameraFollower(*Mana);
    Mana->AddComponent(cf);
    Game::GetInstance().GetCurrentState().AddObject(Mana);

    GameObject* Health = new GameObject(LayerDistance::_HUDLayer);
    healthBar = new Sprite(*Health, HEALTH_BAR, 7);
    Health->box.SetSize(healthBar->GetWidth(), healthBar->GetHeight());
    Health->AddComponent(healthBar);
    cf = new CameraFollower(*Health);
    Health->AddComponent(cf);
    Game::GetInstance().GetCurrentState().AddObject(Health);

    // GameObject
}  

void Hud::Update(float dt) {
    healthBar->SetFrame(Kid::self->hp);
    manaBar->SetFrame((GameData::canUseMana ? 4 + Kid::self->mp : 0));
}

bool Hud::Is(std::string type) {return (type=="Hud");}
bool Hud::Is(ComponentType type) {return (this->type & type);}
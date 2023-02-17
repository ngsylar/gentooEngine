#include "GentooEngine.h"
#include "ZoneTransition.h"
#include "PlatformCamera.h"
#include "Kid.h"

#define FONT_LED "./assets/font/led_real.ttf"

State::State () {
    debugMode = false;
    awake = false;
    popRequested = false;
    quitRequested = false;
    
    // FPS Counter
    Text * FPSText = new Text(FPSObj, " ", FONT_LED, 24, Text::SHADED, Color("#00FF00").ColorSDL());
    CameraFollower* FPSFollow = new CameraFollower(FPSObj);
    FPSFollow->offset = Vec2(20,20);
    FPSObj.AddComponent(FPSText);
    FPSObj.AddComponent(FPSFollow);
    stateMusic = nullptr;
    fpsLimiter.SetResetTime(0.1);
}

State::~State () {
    renderingArray.clear();
    objectArray.clear();
    Camera::Reset();
    Camera::masterOffset = Vec2();
}

void State::AwakenBase () {
    LoadAssets();
    Awaken();
    
    for (int i=0; i < (int)objectArray.size(); i++)
        objectArray[i]->Awaken();
    awake = true;
}

void State::StartBase () {
    Start();
    
    for (int i=0; i < (int)objectArray.size(); i++) {
        objectArray[i]->Start();
    }
}

void State::UpdateBase (float dt) {
    InputManager& input = InputManager::GetInstance();
    Camera::pos -= Camera::masterOffset;
    stateDt = dt;
    
    if (input.QuitRequested()) {
        quitRequested = true;
        return;
    }

    // DEBUG
    if (input.IsKeyDown(KEY_CTRL_LEFT) and input.IsKeyDown(KEY_SHIFT_LEFT) and input.KeyPress(KEY_P))
        debugMode = not debugMode;

    Update(dt);

    for (int i=0; i < (int)objectArray.size(); i++)
        objectArray[i]->Update(dt);

    DetectCollisions();

    for (int i=(int)objectArray.size()-1; i >= 0; i--)
        if (objectArray[i]->IsDead())
            objectArray.erase(objectArray.begin()+i);

    Camera::Update(dt);
    Camera::pos += Camera::masterOffset;

    for (int i=0; i < (int)objectArray.size(); i++)
        objectArray[i]->LateUpdate(dt);
    
    // FPS Counter
    fpsLimiter.Update(dt);
    if (Game::GetInstance().GetCurrentState().Debugging()) {
        if (fpsLimiter.IsOver()) {
            Text* UpdateFPS =(Text*)FPSObj.GetComponent(ComponentType::_Text);
            UpdateFPS->SetText(std::to_string((int)(1/dt))+" FPS");
            fpsLimiter.Reset();
        } FPSObj.LateUpdate(0);
    }
}

void State::RenderBase () {
    for (int i=(int)renderingArray.size()-1; i >= 0; i--) {
        if (renderingArray[i].expired())
            renderingArray.erase(renderingArray.begin()+i);
    }
    Render();

    if (scheduleSortingLayer) {
        std::sort(renderingArray.begin(), renderingArray.end(), GameObject::CompareLayers);
        scheduleSortingLayer = false;
    } for (int i=0; i < (int)renderingArray.size(); i++) {
        renderingArray[i].lock()->Render();
    }

    // FPS Counter
    if (Game::GetInstance().GetCurrentState().Debugging()) {
        FPSObj.Render();
    }    
}

void State::LoadAssets () {}

void State::Awaken () {}

void State::Start () {}

void State::Update (float dt) {}

void State::Render () {}

void State::Pause () {}

void State::Resume () {}

std::weak_ptr<GameObject> State::AddObject (GameObject* object) {
    std::shared_ptr<GameObject> sptrGo(object);
    std::weak_ptr<GameObject> wptrGo(sptrGo);

    objectArray.push_back(sptrGo);
    if (awake) {
        object->Awaken();
        object->Start();
    }

    // sylar's extra layer rendering
    renderingArray.push_back(wptrGo);
    scheduleSortingLayer = true;
    
    return wptrGo;
}

std::weak_ptr<GameObject> State::GetObjectPtr (GameObject* object) {
    std::weak_ptr<GameObject> wptrGo;

    for (int i=0; i < (int)objectArray.size(); i++) {
        if (objectArray[i].get() == object)
            wptrGo = objectArray[i];
    }
    return wptrGo;
}

std::weak_ptr<GameObject> State::GetObjectPtr (std::string label) {
    std::weak_ptr<GameObject> wptrGo;

    for (int i=0; i < (int)objectArray.size(); i++) {
        if (objectArray[i]->label == label)
            wptrGo = objectArray[i];
    }
    return wptrGo;
}

void State::ScheduleSort() {
    scheduleSortingLayer = true;
}

void State::DetectCollisions () {
    bool thereIsCollision;

    for (int i=0; i < (int)objectArray.size()-1; i++) {
        if(objectArray[i]->enabled and objectArray[i]->Contains(ComponentType::_Collider)
        ) {
            for (int j=i+1; j < (int)objectArray.size(); j++) {
                if (objectArray[j]->enabled
                    and objectArray[j]->Contains(ComponentType::_Collider)
                    and not (objectArray[i]->Contains(ComponentType::_Terrain)
                    and objectArray[j]->Contains(ComponentType::_Terrain))
                ) {
                    Collider* colliderA = (Collider*)objectArray[i]->GetComponent(ComponentType::_Collider);
                    Collider* colliderB = (Collider*)objectArray[j]->GetComponent(ComponentType::_Collider);

                    thereIsCollision = Collision::IsColliding(
                        colliderA->box, colliderB->box,
                        Deg2Rad(objectArray[i]->angleDeg), Deg2Rad(objectArray[j]->angleDeg)
                    );
                    if (thereIsCollision) {
                        objectArray[i]->NotifyCollision(*objectArray[j]);
                        objectArray[j]->NotifyCollision(*objectArray[i]);
                    }
                    else {
                        objectArray[i]->NotifyNoCollision(*objectArray[j]);
                        objectArray[j]->NotifyNoCollision(*objectArray[i]);
                    }
                }
            }
        }
    }
}

void State::FadeIn() {
    //Scene FadeIn
    GameObject* FadeObj = new GameObject(LayerDistance::_FadingLayer);
    ScreenFade* Fade = new ScreenFade(*FadeObj, Color("#000000"),1, 0, STATE_FADE_TIME, true);
    FadeObj->AddComponent(Fade);
    AddObject(FadeObj);
}

void State::AddScenario(std::string file, LayerDistance layer, float parallax, bool align) {
    GameObject* BgObj = new GameObject(layer);
    Sprite* Bg1 = new Sprite(*BgObj,file);
    BgObj->AddComponent(Bg1);
    AddObject(BgObj);
    if (parallax!=0) {
        Bg1->parallaxFactor = Vec2(parallax,1);
    }
    if(align) {
        BgObj->box.SetPurePosition(((Bg1->parallaxFactor.x - 1.0)*Bg1->GetWidth())/4,0);
    } else {
        BgObj->box.SetPurePosition(0,0);
    }
    BgObj->box.SetSize(Bg1->GetWidth(), Bg1->GetHeight());
}

void State::AddAnimated(std::string file, LayerDistance layer, Vec2 position, int frames, float frameTime) {
    GameObject* animatedObj = new GameObject(layer);
    Sprite* animated = new Sprite(*animatedObj, file, frames, frameTime);
    animatedObj->AddComponent(animated);
    animatedObj->box.SetSize(animated->GetWidth(), animated->GetHeight());
    animatedObj->box.SetPurePosition(position.x*TILE_SIZE,position.y*TILE_SIZE);
    AddObject(animatedObj);
}

void State::AddSpikes(float x, float y, float w) {
    GameObject* spikeObj = new GameObject(LayerDistance::_Environment_Close);
    spikeObj->box.SetPurePosition(x*TILE_SIZE, (y+0.5)*TILE_SIZE);
    spikeObj->box.SetSize(w*TILE_SIZE, 0.5*TILE_SIZE);
    Interactor* touchedSpike = new Interactor(*spikeObj);
    touchedSpike->SetResult([](){
        // Vec2 place = ZoneManager::GetSpawnPosition()*TILE_SIZE;
        // Kid::GetInstance()->box.SetPosition(place.x, place.y-KID_HEIGHT);//TODO cause damage, implement transition, fix camera glitch
        // Kid* kid = (Kid*)Kid::GetInstance()->GetComponent(ComponentType::_Kid);
        //Vec2 place = ZoneManager::GetSpawnPosition()*TILE_SIZE;
        //Add fadeout, once everything is black:
        //KidObj->box.SetPosition(place.x, place.y-KID_HEIGHT);
        //Add interactor on the same position to process fadein
        // SDL_Log("Spikes!");
    });
    AttackGeneric* atk = new AttackGeneric(*spikeObj);
    spikeObj->AddComponent(atk);
    spikeObj->AddComponent(touchedSpike);
    AddObject(spikeObj);
}

void State::AddKid() {
    GameObject* KidObj = new GameObject(LayerDistance::_Player);
    AddObject(KidObj);
    KidObj->AddComponent(new Kid(*KidObj));
    Vec2 place = ZoneManager::GetSpawnPosition()*TILE_SIZE;
    KidObj->box.SetPosition(place.x, place.y-KID_HEIGHT);
}

#define MAGIC_BARRIER "assets/img/animated/10_barreira28x84-sheet.png"
void State::AddMagicBarrier(Vec2 position) {
    GameObject* barrierObj = new GameObject(LayerDistance::_Environment_Close);
    Sprite* anim = new Sprite(*barrierObj, MAGIC_BARRIER, 5, 0.1);
    barrierObj->box.SetSize(anim->GetWidth(), anim->GetHeight());
    barrierObj->box.SetPurePosition(position*TILE_SIZE);
    Interactor* barrierLock = new Interactor(*barrierObj, true, false, ComponentType::_Attack);
    barrierLock->SetCondition([](){
        // Kid* kid = (Kid*) Kid::GetInstance()->GetComponent(ComponentType::_Kid);
        return Kid::self->GetCurrentState() == EntityState::CastingSpell_0;//TODO may change to magic attack label
    });
    barrierLock->SetResult([](){SDL_Log("Barrier broken!");});
    barrierObj->AddComponent(anim);
    barrierObj->AddComponent(barrierLock);
    AddObject(barrierObj);
}

void State::LimitMap() {
    GameObject* GO = new GameObject();
    PlatformCamera* plat = new PlatformCamera(*GO, PlatformCamera::Direction::UP,0);
    GO->AddComponent(plat);
    AddObject(GO);

    GO = new GameObject();
    plat = new PlatformCamera(*GO, PlatformCamera::Direction::DOWN, zoneSize.y-Vec2(GAME_RESOLUTION).y);
    GO->AddComponent(plat);
    AddObject(GO);

    GO = new GameObject();
    plat = new PlatformCamera(*GO, PlatformCamera::Direction::LEFT,0);
    GO->AddComponent(plat);
    AddObject(GO);

    GO = new GameObject();
    plat = new PlatformCamera(*GO, PlatformCamera::Direction::RIGHT,zoneSize.x-Vec2(GAME_RESOLUTION).x);
    GO->AddComponent(plat);
    AddObject(GO);
}

Music* State::GetStateMusic () {
    return stateMusic;
}

bool State::Debugging () {
    return debugMode;
}

void State::RequestPop() {
    popRequested = true;
}

bool State::PopRequested () {
    return popRequested;
}

bool State::QuitRequested () {
    return quitRequested;
}
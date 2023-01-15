#include "GentooEngine.h"

#define FONT_LED "./assets/font/led_real.ttf"

State::State () {
    debugMode = false;
    started = false;
    popRequested = false;
    quitRequested = false;
    
    //FPS Counter
    Text * FPSText = new Text(FPSObj, " ", FONT_LED, 24, Text::SHADED, Color("#00FF00").ColorSDL());
    CameraFollower* FPSFollow = new CameraFollower(FPSObj);
    FPSFollow->offset = Vec2(20,20);
    FPSObj.AddComponent(FPSText);
    FPSObj.AddComponent(FPSFollow);
}

State::~State () {
    renderingArray.clear();
    objectArray.clear();
    Camera::ClearMethods();
    Camera::masterOffset = Vec2();
}

void State::StartBase () {
    LoadAssets();
    Start();
    
    for (int i=0; i < (int)objectArray.size(); i++)
        objectArray[i]->Start();
    started = true;
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
    
    //FPS Counter
    if (Game::GetInstance().GetCurrentState().Debugging()) {
        Text* UpdateFPS =(Text*)FPSObj.GetComponent(GameObjID::_Text);
        UpdateFPS->SetText(std::to_string((int)(1/dt))+" FPS");
        FPSObj.LateUpdate(0);
    }
}

void State::RenderBase () {
    // sylar's extra layer rendering
    for (int i=(int)renderingArray.size()-1; i >= 0; i--) {
        if (renderingArray[i].expired())
            renderingArray.erase(renderingArray.begin()+i);
    }

    Render();

    // // idj's original object rendering
    // for (int i=0; i < (int)objectArray.size(); i++) {
    //     objectArray[i]->Render();
    // }

    // sylar's extra layer rendering
    if (scheduleSortingLayer) {
        std::sort(renderingArray.begin(), renderingArray.end(), GameObject::CompareLayers);
        scheduleSortingLayer = false;
    }
    
    // sylar's extra layer rendering
    for (int i=0; i < (int)renderingArray.size(); i++) {
        renderingArray[i].lock()->Render();
    }

    //FPS Counter
    if (Game::GetInstance().GetCurrentState().Debugging()) {
        FPSObj.Render();
    }    
}

void State::LoadAssets () {}

void State::Start () {}

void State::Update (float dt) {}

void State::Render () {}

void State::Pause () {}

void State::Resume () {}

std::weak_ptr<GameObject> State::AddObject (GameObject* object) {
    std::shared_ptr<GameObject> sptrGo(object);
    std::weak_ptr<GameObject> wptrGo(sptrGo);

    objectArray.push_back(sptrGo);
    if (started)
        object->Start();

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

void State::DetectCollisions () {
    bool thereIsCollision;

    for (int i=0; i < (int)objectArray.size()-1; i++) {
        if(objectArray[i]->Contains(GameObjID::_Collider)) {
            for (int j=i+1; j < (int)objectArray.size(); j++) {
                if(objectArray[j]->Contains(GameObjID::_Collider))
                {
                    Collider* colliderA = (Collider*)objectArray[i]->GetComponent(GameObjID::_Collider);
                    Collider* colliderB = (Collider*)objectArray[j]->GetComponent(GameObjID::_Collider);

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

bool State::Debugging () {
    return debugMode;
}

bool State::PopRequested () {
    return popRequested;
}

bool State::QuitRequested () {
    return quitRequested;
}
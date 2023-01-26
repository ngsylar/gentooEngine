#include "GentooEngine.h"
#include "EngineScreen.h"

EngineScreen::EngineScreen () {
    screenTimer.SetResetTime(ENGINE_SCREEN_CONCEAL_TIME);
    currentScreen = 0;
    exchanger = true;
}

void EngineScreen::LoadAssets () {
    std::weak_ptr<GameObject> blankScreen_wptr, engineScreen_wptr;

    GameObject* blankScreen = new GameObject(ENGINE_SCREEN_LAYER, ENGINE_SCREEN_LABEL);
    Sprite* blankScreenSprite = new Sprite(*blankScreen, ENGINE_SCREEN_BLANK_SPRITE);
    blankScreen->AddComponent(blankScreenSprite);
    blankScreen_wptr = AddObject(blankScreen);

    GameObject* engineScreen = new GameObject(ENGINE_SCREEN_LAYER, ENGINE_SCREEN_LABEL);
    Sprite* engineScreenSprite = new Sprite(*engineScreen, ENGINE_SCREEN_ENGINE_SPRITE);
    engineScreen->AddComponent(engineScreenSprite);
    engineScreen_wptr = AddObject(engineScreen);

    renderingArray.insert(
        renderingArray.end(), {blankScreen_wptr, engineScreen_wptr, blankScreen_wptr}
    );

    std::vector<std::string> screens = ENGINE_SCREEN_GAME_SPRITES;
    GameObject* newScreen;
    Sprite* newSprite;

    for (int i=0; i < (int)screens.size(); i++) {
        newScreen = new GameObject(ENGINE_SCREEN_LAYER, ENGINE_SCREEN_LABEL);
        newSprite = new Sprite(*newScreen, screens[i]);
        newScreen->AddComponent(newSprite);
        renderingArray.push_back(AddObject(newScreen));
    }
}

void EngineScreen::Start () {
    Camera::Reset();
}

void EngineScreen::Update (float dt) {
    InputManager& input = InputManager::GetInstance();

    screenTimer.Update(dt);
    if (screenTimer.IsOver() or input.KeyPress(KEY_ESCAPE) or input.KeyPress(KEY_SPACE) or input.MousePress(MOUSE_BUTTON_LEFT)) {
        currentScreen++;
        exchanger = not exchanger;
        screenTimer.SetResetTime(exchanger? ENGINE_SCREEN_CONCEAL_TIME : ENGINE_SCREEN_DISPLAY_TIME);
        screenTimer.Reset();
    }

    if (currentScreen >= (int)renderingArray.size())
        popRequested = true;
}

void EngineScreen::RenderBase () {
    if ((currentScreen < (int)renderingArray.size()) and (not renderingArray[currentScreen].expired()))
        renderingArray[currentScreen].lock()->Render();
}

std::weak_ptr<GameObject> EngineScreen::AddObject (GameObject* object) {
    std::shared_ptr<GameObject> sptrGo(object);
    std::weak_ptr<GameObject> wptrGo(sptrGo);

    objectArray.push_back(sptrGo);
    if (awake) {
        object->Awaken();
        object->Start();
    }

    return wptrGo;
}
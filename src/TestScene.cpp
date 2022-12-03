#include "GentooEngine.h"
#include "TestScene.h"
#include "TestObjects.h"

TestScene::TestScene () {
    GameObject* bg = new GameObject(SCENE_TEST_LAYER, SCENE_TEST_LABEL);
    bg->AddComponent(new Sprite(*bg, SCENE_TEST_BACKGROUND));
    bg->AddComponent(new CameraFollower(*bg));
    AddObject(bg);
}

void TestScene::LoadAssets () {
    GameObject* rawPlat = new GameObject(LAYER_BLACK_SQUARE);
    Sprite* platSpr = new Sprite(*rawPlat, SPRITE_BLACK_SQUARE);
    rawPlat->AddComponent(platSpr);
    AddObject(rawPlat);
    mini_platform = GetObjectPtr(rawPlat);
    platSpr->SetScale(15.0f, 1.0f);
    rawPlat->box.SetPosition(512, 500);
    rawPlat->AddComponent(new Collider(*rawPlat));
    rawPlat->AddComponent(new Ground(*rawPlat));

    rawPlat = new GameObject(LAYER_BLACK_SQUARE);
    platSpr = new Sprite(*rawPlat, SPRITE_BLACK_SQUARE);
    rawPlat->AddComponent(platSpr);
    AddObject(rawPlat);
    mini_platform = GetObjectPtr(rawPlat);
    platSpr->SetScale(15.0f, 1.0f);
    rawPlat->box.SetPosition(512, 100);
    rawPlat->AddComponent(new Collider(*rawPlat));

    rawPlat = new GameObject(LAYER_BLACK_SQUARE);
    platSpr = new Sprite(*rawPlat, SPRITE_BLACK_SQUARE);
    rawPlat->AddComponent(platSpr);
    AddObject(rawPlat);
    mini_platform = GetObjectPtr(rawPlat);
    platSpr->SetScale(1.0f, 7.0f);
    rawPlat->box.SetPosition(100, 300);
    rawPlat->AddComponent(new Collider(*rawPlat));

    rawPlat = new GameObject(LAYER_BLACK_SQUARE);
    platSpr = new Sprite(*rawPlat, SPRITE_BLACK_SQUARE);
    rawPlat->AddComponent(platSpr);
    AddObject(rawPlat);
    mini_platform = GetObjectPtr(rawPlat);
    platSpr->SetScale(1.0f, 7.0f);
    rawPlat->box.SetPosition(900, 300);
    Collider* platcoll = new Collider(*rawPlat);
    rawPlat->AddComponent(platcoll);
    // platcoll->isTrigger = true;

    rawPlat = new GameObject(LAYER_BLACK_SQUARE);
    platSpr = new Sprite(*rawPlat, SPRITE_BLACK_SQUARE);
    rawPlat->AddComponent(platSpr);
    AddObject(rawPlat);
    mini_platform = GetObjectPtr(rawPlat);
    platSpr->SetScale(3.0f, 1.0f);
    rawPlat->box.SetPosition(300, 300);
    rawPlat->AddComponent(new Collider(*rawPlat));

    GameObject* rawSquare = new GameObject(LAYER_RED_SQUARE);
    rawSquare->AddComponent(new Sprite(*rawSquare, SPRITE_RED_SQUARE));
    AddObject(rawSquare);
    square = GetObjectPtr(rawSquare);
    rawSquare->AddComponent(new Ball(*rawSquare));
    rawSquare->box.SetPosition(165,300);

    // rawSquare = new GameObject(4);
    // rawSquare->AddComponent(new LoopedBackground(*rawSquare, SPRITE_RED_SQUARE));
    // rawSquare->box.SetPosition(512,300);
    // AddObject(rawSquare);
}

void TestScene::Start () {
    Camera::Follow(square.lock().get());
}

void TestScene::Update (float dt) {
    if (InputManager::GetInstance().KeyPress(KEY_ESCAPE)) {
        Game::GetInstance().AddState(new TestScene2());
        popRequested = true;
    }
}

TestScene2::TestScene2 () {
    GameObject* bg = new GameObject(SCENE_TEST_LAYER, SCENE_TEST_LABEL);
    LoopedBackground* lbg = new LoopedBackground(*bg, SCENE_TEST_BACKGROUND);
    // lbg->parallaxFactor = 0.25f;
    bg->AddComponent(lbg);
    AddObject(bg);
}

void TestScene2::LoadAssets () {
    GameObject* rawPlat = new GameObject(LAYER_BLACK_SQUARE);
    Sprite* platSpr = new Sprite(*rawPlat, SPRITE_BLACK_SQUARE);
    rawPlat->AddComponent(platSpr);
    AddObject(rawPlat);
    mini_platform = GetObjectPtr(rawPlat);
    platSpr->SetScale(90.0f, 1.0f);
    rawPlat->box.SetPosition(512, 500);
    rawPlat->AddComponent(new Collider(*rawPlat));
    rawPlat->AddComponent(new Ground(*rawPlat));

    // GameObject* rawRun = new GameObject(LAYER_RUN_RIGHT);
    // Sprite* runSpr = new Sprite(*rawRun, SPRITE_RUN_RIGHT, 12, 0.02f);
    // rawRun->AddComponent(runSpr);
    // AddObject(rawRun);
    // rawRun->box.SetPosition(512, 300);
    // for (int i=0; i<30; i++)
    //     runSpr->positionArray.push_back(Vec2(i*142,350));

    // rawRun = new GameObject(LAYER_RUN_LEFT);
    // runSpr = new Sprite(*rawRun, SPRITE_RUN_LEFT, 12, 0.02f);
    // rawRun->AddComponent(runSpr);
    // AddObject(rawRun);
    // rawRun->box.SetPosition(512, 200);
    // runSpr->SetScale(2);
    // runSpr->parallaxFactor = 0.25;
    // for (int i=0; i<30; i++)
    //     runSpr->positionArray.push_back(Vec2(i*142,200));
    
    GameObject* rawSquare = new GameObject(LAYER_RED_SQUARE);
    rawSquare->AddComponent(new Sprite(*rawSquare, SPRITE_RED_SQUARE));
    AddObject(rawSquare);
    square = GetObjectPtr(rawSquare);
    rawSquare->AddComponent(new Ball(*rawSquare));
    rawSquare->box.SetPosition(512,300);
}

void TestScene2::Start () {
    Camera::Follow(square.lock().get());
}

void TestScene2::Update (float dt) {
    if (InputManager::GetInstance().KeyPress(KEY_ESCAPE)) {
        Game::GetInstance().AddState(new TestScene3());
        popRequested = true;
    }
}

TestScene3::TestScene3 () {
    // fazer o bg loop dps
    GameObject* bg = new GameObject(SCENE_TEST_LAYER, SCENE_TEST_LABEL);
    // bg->AddComponent(new Sprite(*bg, SCENE_TEST_BACKGROUND));
    // bg->AddComponent(new CameraFollower(*bg));
    LoopedBackground* lbg = new LoopedBackground(*bg, SCENE_TEST_BACKGROUND);
    lbg->parallaxFactor = 0.25f;
    bg->AddComponent(lbg);
    AddObject(bg);
}

void TestScene3::LoadAssets () {
    for (int i=0; i < 25; i++) {
        GameObject* rawPlat = new GameObject(LAYER_BLACK_SQUARE);
        Sprite* platSpr = new Sprite(*rawPlat, SPRITE_BLACK_SQUARE);
        rawPlat->AddComponent(platSpr);
        AddObject(rawPlat);
        platSpr->SetScale(3.0f, 1.0f);
        rawPlat->box.SetPosition(100, i*400);
        rawPlat->AddComponent(new Collider(*rawPlat));

        rawPlat = new GameObject(LAYER_BLACK_SQUARE);
        platSpr = new Sprite(*rawPlat, SPRITE_BLACK_SQUARE);
        rawPlat->AddComponent(platSpr);
        AddObject(rawPlat);
        platSpr->SetScale(3.0f, 1.0f);
        rawPlat->box.SetPosition(500, i*400);
        rawPlat->AddComponent(new Collider(*rawPlat));
    }
    
    GameObject* rawSquare = new GameObject(LAYER_RED_SQUARE);
    rawSquare->AddComponent(new Sprite(*rawSquare, SPRITE_RED_SQUARE));
    AddObject(rawSquare);
    square = GetObjectPtr(rawSquare);
    rawSquare->AddComponent(new Ball(*rawSquare));
    rawSquare->box.SetPosition(512,300);
}

void TestScene3::Start () {
    Camera::Follow(square.lock().get());
}

void TestScene3::Update (float dt) {
    if (InputManager::GetInstance().KeyPress(KEY_ESCAPE)) {
        popRequested = true;
    }
}
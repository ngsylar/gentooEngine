#include "GentooEngine.h"
#include "TestScene.h"
#include "TestObjects.h"
#include "Kid.h"
#include "PlatformCamera.h"

#define SCENE_TEST_LABEL "TestScene"
#define SCENE_TEST_BACKGROUND "assets/img/test_area.png"
#define SCENE_TEST_LAYER -1

#define SPRITE_RED_SQUARE "assets/img/red_square.png"
#define SPRITE_BLACK_SQUARE "assets/img/black_square.png"
#define SPRITE_RUN_RIGHT "assets/img/corrida1.png"
#define SPRITE_RUN_LEFT "assets/img/corrida2.png"
#define SPRITE_48 "assets/img/48.png"
#define SPRITE_78 "assets/img/prota.png"

#define LABEL_GROUND "Ground"
#define LABEL_PLATFORM "Platform"

#define LAYER_RED_SQUARE 3
#define LAYER_RUN_RIGHT 1
#define LAYER_RUN_LEFT 0
#define LAYER_BLACK_SQUARE 2

TestScene::TestScene () {
    GameObject* bg = new GameObject(SCENE_TEST_LAYER, SCENE_TEST_LABEL);
    Sprite* spirte = new Sprite(*bg, SCENE_TEST_BACKGROUND);
    spirte->SetScale(0.5f);
    bg->AddComponent(spirte);
    bg->AddComponent(new CameraFollower(*bg));
    AddObject(bg);
}

void TestScene::LoadAssets () {
    GameObject* rawPlat = new GameObject(LAYER_BLACK_SQUARE);
    Sprite* platSpr = new Sprite(*rawPlat, SPRITE_BLACK_SQUARE);
    rawPlat->AddComponent(platSpr);
    AddObject(rawPlat);
    mini_platform = GetObjectPtr(rawPlat);
    platSpr->SetScale(7.5f, 0.5f);
    rawPlat->box.SetPosition(256, 250);
    rawPlat->AddComponent(new Collider(*rawPlat));
    rawPlat->AddComponent(new PlatformCamera(*rawPlat, PlatformCamera::DOWN, 0.0f));

    rawPlat = new GameObject(LAYER_BLACK_SQUARE);
    platSpr = new Sprite(*rawPlat, SPRITE_BLACK_SQUARE);
    rawPlat->AddComponent(platSpr);
    AddObject(rawPlat);
    mini_platform = GetObjectPtr(rawPlat);
    platSpr->SetScale(7.5f, 0.5f);
    rawPlat->box.SetPosition(256, 50);
    rawPlat->AddComponent(new Collider(*rawPlat));

    rawPlat = new GameObject(LAYER_BLACK_SQUARE);
    platSpr = new Sprite(*rawPlat, SPRITE_BLACK_SQUARE);
    rawPlat->AddComponent(platSpr);
    AddObject(rawPlat);
    mini_platform = GetObjectPtr(rawPlat);
    platSpr->SetScale(0.5f, 3.5f);
    rawPlat->box.SetPosition(50, 150);
    Collider* platcoll = new Collider(*rawPlat);
    rawPlat->AddComponent(platcoll);

    rawPlat = new GameObject(LAYER_BLACK_SQUARE);
    platSpr = new Sprite(*rawPlat, SPRITE_BLACK_SQUARE);
    rawPlat->AddComponent(platSpr);
    AddObject(rawPlat);
    mini_platform = GetObjectPtr(rawPlat);
    platSpr->SetScale(0.5f, 3.5f);
    rawPlat->box.SetPosition(450, 150);
    platcoll = new Collider(*rawPlat);
    rawPlat->AddComponent(platcoll);
    // platcoll->isTrigger = true;

    rawPlat = new GameObject(LAYER_BLACK_SQUARE);
    platSpr = new Sprite(*rawPlat, SPRITE_BLACK_SQUARE);
    rawPlat->AddComponent(platSpr);
    AddObject(rawPlat);
    mini_platform = GetObjectPtr(rawPlat);
    platSpr->SetScale(1.5f, 0.5f);
    // rawPlat->box.SetPosition(300, 300);
    rawPlat->box.SetPosition(150, 162.5f);
    rawPlat->AddComponent(new Collider(*rawPlat));

    GameObject* rawSquare = new GameObject(LAYER_RED_SQUARE);
    AddObject(rawSquare);
    square = GetObjectPtr(rawSquare);
    rawSquare->AddComponent(new Kid(*rawSquare));
    rawSquare->box.SetPosition(62,100);
    // Sprite* piss = new Sprite(*rawSquare, SPRITE_RED_SQUARE);
    // piss->SetScale(0.5f);
    // rawSquare->AddComponent(piss);
    // AddObject(rawSquare);
    // square = GetObjectPtr(rawSquare);
    // rawSquare->AddComponent(new Ball(*rawSquare));
    // rawSquare->box.SetPosition(85.5f,100);

    // rawSquare = new GameObject(4);
    // rawSquare->AddComponent(new LoopedBackground(*rawSquare, SPRITE_RED_SQUARE));
    // rawSquare->box.SetPosition(512,300);
    // AddObject(rawSquare);
}

void TestScene::Start () {
    // Camera::Follow(square.lock().get());
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
    platSpr->SetScale(45.0f, 0.5f);
    rawPlat->box.SetPosition(256, 250);
    rawPlat->AddComponent(new Collider(*rawPlat));
    rawPlat->AddComponent(new PlatformCamera(*rawPlat, PlatformCamera::DOWN, -5.5f));

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
    Sprite* piss = new Sprite(*rawSquare, SPRITE_RED_SQUARE);
    piss->SetScale(0.5f);
    rawSquare->AddComponent(piss);
    AddObject(rawSquare);
    square = GetObjectPtr(rawSquare);
    rawSquare->AddComponent(new Ball(*rawSquare));
    rawSquare->box.SetPosition(256,150);

    rawSquare = new GameObject(LAYER_RED_SQUARE);
    Sprite* sqspr = new Sprite(*rawSquare, SPRITE_78);
    rawSquare->AddComponent(sqspr);
    AddObject(rawSquare);
    square = GetObjectPtr(rawSquare);
    rawSquare->box.SetPosition(256,150);

    rawSquare = new GameObject(LAYER_RED_SQUARE);
    sqspr = new Sprite(*rawSquare, SPRITE_48);
    rawSquare->AddComponent(sqspr);
    AddObject(rawSquare);
    square = GetObjectPtr(rawSquare);
    rawSquare->box.SetPosition(291,145);
}

void TestScene2::Start () {
    // Camera::Follow(square.lock().get());
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
    Sprite* sqspr = new Sprite(*bg, SCENE_TEST_BACKGROUND);
    sqspr->SetScale(0.5f);
    bg->AddComponent(sqspr);
    bg->AddComponent(new CameraFollower(*bg));
    // LoopedBackground* lbg = new LoopedBackground(*bg, SCENE_TEST_BACKGROUND, 2, 0.25f);
    // bg->AddComponent(lbg);
    AddObject(bg);
}

void TestScene3::LoadAssets () {
    for (int i=0; i < 25; i++) {
        GameObject* rawPlat = new GameObject(LAYER_BLACK_SQUARE);
        Sprite* platSpr = new Sprite(*rawPlat, SPRITE_BLACK_SQUARE);
        rawPlat->AddComponent(platSpr);
        AddObject(rawPlat);
        platSpr->SetScale(1.5f, 0.5f);
        rawPlat->box.SetPosition(50, i*200);
        rawPlat->AddComponent(new Collider(*rawPlat));

        rawPlat = new GameObject(LAYER_BLACK_SQUARE);
        platSpr = new Sprite(*rawPlat, SPRITE_BLACK_SQUARE);
        rawPlat->AddComponent(platSpr);
        AddObject(rawPlat);
        platSpr->SetScale(1.5f, 0.5f);
        rawPlat->box.SetPosition(250, i*200);
        rawPlat->AddComponent(new Collider(*rawPlat));
    }
    
    GameObject* rawSquare = new GameObject(LAYER_RED_SQUARE);
    Sprite* piss = new Sprite(*rawSquare, SPRITE_RED_SQUARE);
    piss->SetScale(0.5f);
    rawSquare->AddComponent(piss);
    AddObject(rawSquare);
    square = GetObjectPtr(rawSquare);
    rawSquare->AddComponent(new Ball(*rawSquare));
    rawSquare->box.SetPosition(256,150);
}

void TestScene3::Start () {
    // Camera::Follow(square.lock().get());
}

void TestScene3::Update (float dt) {
    if (InputManager::GetInstance().KeyPress(KEY_ESCAPE)) {
        Game::GetInstance().AddState(new TestScene4());
        popRequested = true;
    }
}

TestScene4::TestScene4 () {
    // fazer o bg loop dps
    GameObject* bg = new GameObject(SCENE_TEST_LAYER, SCENE_TEST_LABEL);
    Sprite* sqspr = new Sprite(*bg, SCENE_TEST_BACKGROUND);
    sqspr->SetScale(0.5f);
    bg->AddComponent(sqspr);
    bg->AddComponent(new CameraFollower(*bg));
    // LoopedBackground* lbg = new LoopedBackground(*bg, SCENE_TEST_BACKGROUND, 2, 0.25f);
    // bg->AddComponent(lbg);
    AddObject(bg);
}

void TestScene4::LoadAssets () {
    GameObject* rawPlat = new GameObject(LAYER_BLACK_SQUARE, LABEL_PLATFORM);
    Sprite* platSpr = new Sprite(*rawPlat, SPRITE_BLACK_SQUARE);
    rawPlat->AddComponent(platSpr);
    AddObject(rawPlat);
    platSpr->SetScale(1.5f, 0.5f);
    rawPlat->box.SetPosition(50, 162.5f);
    rawPlat->AddComponent(new Collider(*rawPlat));

    rawPlat = new GameObject(LAYER_BLACK_SQUARE, LABEL_GROUND);
    platSpr = new Sprite(*rawPlat, SPRITE_BLACK_SQUARE);
    rawPlat->AddComponent(platSpr);
    AddObject(rawPlat);
    platSpr->SetScale(1.5f, 0.5f);
    rawPlat->box.SetPosition(250, 250);
    rawPlat->AddComponent(new Collider(*rawPlat));
    rawPlat->AddComponent(new PlatformCamera(*rawPlat, PlatformCamera::DOWN, rawPlat->box.GetPosition().y-255.5f));

    for (int i=1; i < 25; i++) {
        GameObject* rawPlat = new GameObject(LAYER_BLACK_SQUARE, LABEL_PLATFORM);
        Sprite* platSpr = new Sprite(*rawPlat, SPRITE_BLACK_SQUARE);
        rawPlat->AddComponent(platSpr);
        AddObject(rawPlat);
        platSpr->SetScale(1.5f, 0.5f);
        rawPlat->box.SetPosition(50, 162.5f-i*175);
        rawPlat->AddComponent(new Collider(*rawPlat));

        rawPlat = new GameObject(LAYER_BLACK_SQUARE, LABEL_PLATFORM);
        platSpr = new Sprite(*rawPlat, SPRITE_BLACK_SQUARE);
        rawPlat->AddComponent(platSpr);
        AddObject(rawPlat);
        platSpr->SetScale(1.5f, 0.5f);
        rawPlat->box.SetPosition(250, 250-i*175);
        rawPlat->AddComponent(new Collider(*rawPlat));
    }
    
    GameObject* rawSquare = new GameObject(LAYER_RED_SQUARE);
    Sprite* piss = new Sprite(*rawSquare, SPRITE_RED_SQUARE);
    piss->SetScale(0.5f);
    rawSquare->AddComponent(piss);
    AddObject(rawSquare);
    rawSquare->AddComponent(new Ball(*rawSquare));
    rawSquare->box.SetPosition(256,150);
}

void TestScene4::Update (float dt) {
    if (InputManager::GetInstance().KeyPress(KEY_ESCAPE)) {
        Game::GetInstance().AddState(new TestScene5());
        popRequested = true;
    }
}

TestScene5::TestScene5 () {
    // fazer o bg loop dps
    GameObject* bg = new GameObject(SCENE_TEST_LAYER, SCENE_TEST_LABEL);
    Sprite* sqspr = new Sprite(*bg, SCENE_TEST_BACKGROUND);
    sqspr->SetScale(0.5f);
    bg->AddComponent(sqspr);
    bg->AddComponent(new CameraFollower(*bg));
    // LoopedBackground* lbg = new LoopedBackground(*bg, SCENE_TEST_BACKGROUND, 2, 0.25f);
    // bg->AddComponent(lbg);
    AddObject(bg);
}

void TestScene5::LoadAssets () {
    GameObject* rawPlat = new GameObject(LAYER_BLACK_SQUARE, LABEL_PLATFORM);
    Sprite* platSpr = new Sprite(*rawPlat, SPRITE_BLACK_SQUARE);
    rawPlat->AddComponent(platSpr);
    AddObject(rawPlat);
    platSpr->SetScale(7.5f, 0.5f);
    rawPlat->box.SetPosition(0, 162.5f);
    rawPlat->AddComponent(new Collider(*rawPlat));

    rawPlat = new GameObject(LAYER_BLACK_SQUARE, LABEL_GROUND);
    platSpr = new Sprite(*rawPlat, SPRITE_BLACK_SQUARE);
    rawPlat->AddComponent(platSpr);
    AddObject(rawPlat);
    platSpr->SetScale(7.5f, 0.5f);
    rawPlat->box.SetPosition(250, 250);
    rawPlat->AddComponent(new Collider(*rawPlat));

    rawPlat = new GameObject(LAYER_BLACK_SQUARE);
    AddObject(rawPlat);
    platSpr->SetScale(7.5f, 0.5f);
    rawPlat->box.SetPosition(0, 112.5f);
    rawPlat->box.w = 30;
    rawPlat->box.h = 30;
    PlatformCamera* realplat = new PlatformCamera(*rawPlat, PlatformCamera::DOWN, 162.5f-255.5f);
    realplat->SetTrigger(Rect(0,127.5f,30,30), Rect(75,127.5f,30,30));
    rawPlat->AddComponent(realplat);
    
    GameObject* rawSquare = new GameObject(LAYER_RED_SQUARE);
    Sprite* piss = new Sprite(*rawSquare, SPRITE_RED_SQUARE);
    piss->SetScale(0.5f);
    rawSquare->AddComponent(piss);
    AddObject(rawSquare);
    rawSquare->AddComponent(new Ball(*rawSquare));
    rawSquare->box.SetPosition(256,150);
}

void TestScene5::Update (float dt) {
    if (InputManager::GetInstance().KeyPress(KEY_ESCAPE)) {
        Game::GetInstance().AddState(new TestScene());
        popRequested = true;
    }
}
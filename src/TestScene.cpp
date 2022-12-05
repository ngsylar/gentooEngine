#include "GentooEngine.h"
#include "TestScene.h"
#include "TestObjects.h"

#define SCENE_TEST_LABEL "TestScene"
#define SCENE_TEST_BACKGROUND "assets/img/test_area.png"
#define SCENE_TEST_LAYER -1

#define SPRITE_RED_SQUARE "assets/img/red_square.png"
#define SPRITE_BLACK_SQUARE "assets/img/black_square.png"
#define SPRITE_RUN_RIGHT "assets/img/corrida1.png"
#define SPRITE_RUN_LEFT "assets/img/corrida2.png"

#define LABEL_GROUND "Ground"
#define LABEL_PLATFORM "Platform"

#define LAYER_RED_SQUARE 3
#define LAYER_RUN_RIGHT 1
#define LAYER_RUN_LEFT 0
#define LAYER_BLACK_SQUARE 2

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
    rawPlat->AddComponent(new Platform(*rawPlat, Platform::DOWN, -11.0f));

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
    // rawPlat->box.SetPosition(300, 300);
    rawPlat->box.SetPosition(300, 325);
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
    platSpr->SetScale(90.0f, 1.0f);
    rawPlat->box.SetPosition(512, 500);
    rawPlat->AddComponent(new Collider(*rawPlat));
    rawPlat->AddComponent(new Platform(*rawPlat, Platform::DOWN, -11.0f));

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
    // bg->AddComponent(new Sprite(*bg, SCENE_TEST_BACKGROUND));
    // bg->AddComponent(new CameraFollower(*bg));
    LoopedBackground* lbg = new LoopedBackground(*bg, SCENE_TEST_BACKGROUND, 2, 0.25f);
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
    bg->AddComponent(new Sprite(*bg, SCENE_TEST_BACKGROUND));
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
    platSpr->SetScale(3.0f, 1.0f);
    rawPlat->box.SetPosition(100, 325);
    rawPlat->AddComponent(new Collider(*rawPlat));
    rawPlat->AddComponent(new Platform(*rawPlat, Platform::DOWN, -11.0f));

    rawPlat = new GameObject(LAYER_BLACK_SQUARE, LABEL_GROUND);
    platSpr = new Sprite(*rawPlat, SPRITE_BLACK_SQUARE);
    rawPlat->AddComponent(platSpr);
    AddObject(rawPlat);
    platSpr->SetScale(3.0f, 1.0f);
    rawPlat->box.SetPosition(500, 500);
    rawPlat->AddComponent(new Collider(*rawPlat));

    for (int i=1; i < 25; i++) {
        GameObject* rawPlat = new GameObject(LAYER_BLACK_SQUARE, LABEL_PLATFORM);
        Sprite* platSpr = new Sprite(*rawPlat, SPRITE_BLACK_SQUARE);
        rawPlat->AddComponent(platSpr);
        AddObject(rawPlat);
        platSpr->SetScale(3.0f, 1.0f);
        rawPlat->box.SetPosition(100, 325-i*350);
        rawPlat->AddComponent(new Collider(*rawPlat));

        rawPlat = new GameObject(LAYER_BLACK_SQUARE, LABEL_PLATFORM);
        platSpr = new Sprite(*rawPlat, SPRITE_BLACK_SQUARE);
        rawPlat->AddComponent(platSpr);
        AddObject(rawPlat);
        platSpr->SetScale(3.0f, 1.0f);
        rawPlat->box.SetPosition(500, 500-i*350);
        rawPlat->AddComponent(new Collider(*rawPlat));
    }
    
    GameObject* rawSquare = new GameObject(LAYER_RED_SQUARE);
    rawSquare->AddComponent(new Sprite(*rawSquare, SPRITE_RED_SQUARE));
    AddObject(rawSquare);
    square = GetObjectPtr(rawSquare);
    rawSquare->AddComponent(new Ball(*rawSquare));
    rawSquare->box.SetPosition(512,300);
}

void TestScene4::Update (float dt) {
    if (InputManager::GetInstance().KeyPress(KEY_ESCAPE)) {
        Game::GetInstance().AddState(new TestScene());
        popRequested = true;
    }
}
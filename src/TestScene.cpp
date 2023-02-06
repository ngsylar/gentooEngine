#include "GentooEngine.h"
#include "TestScene.h"
#include "TestObjects.h"
#include "Kid.h"
#include "EnemyArmadillo.h"
#include "PlatformCamera.h"

#define SCENE_TEST_LABEL "TestScene"
#define SCENE_TEST_BACKGROUND "assets/img/test_area_720p.png"
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

TestScene0::TestScene0 () {
    // // fazer o bg loop dps
    // GameObject* bg = new GameObject(-2, SCENE_TEST_LABEL);
    // Sprite* sqspr = new Sprite(*bg, SCENE_TEST_BACKGROUND);
    // sqspr->SetScale(0.5f);
    // bg->AddComponent(sqspr);
    // bg->AddComponent(new CameraFollower(*bg));
    // // LoopedBackground* lbg = new LoopedBackground(*bg, SCENE_TEST_BACKGROUND, 2, 0.25f);
    // // bg->AddComponent(lbg);
    // AddObject(bg);

    // bg = new GameObject(SCENE_TEST_LAYER, SCENE_TEST_LABEL);
    // lbg = new LoopedBackground(*bg, "assets/img/lbg.png", 2, Vec2(0.25f,0.25f));
    // bg->AddComponent(lbg);
    // AddObject(bg);

    GameObject* bg = new GameObject(SCENE_TEST_LAYER, SCENE_TEST_LABEL);
    lbg = new LoopedBackground(*bg, "assets/img/fundo_teletobis.png", 2, Vec2(0.25f,0.25f));
    bg->AddComponent(lbg);
    AddObject(bg);
}

void TestScene0::LoadAssets () {
    GameObject* rawPlat = new GameObject(LAYER_BLACK_SQUARE, "Ground");
    Sprite* platSpr = new Sprite(*rawPlat, SPRITE_BLACK_SQUARE);
    rawPlat->AddComponent(platSpr);
    AddObject(rawPlat);
    mini_platform = GetObjectPtr(rawPlat);
    // platSpr->SetScale(7.5f, 0.5f);
    platSpr->SetScale(50, 0.5f);
    rawPlat->box.SetPosition(256, 250);
    rawPlat->AddComponent(new Collider(*rawPlat));
    // rawPlat->AddComponent(new PlatformCamera(*rawPlat, PlatformCamera::DOWN, 0.0f));

    rawPlat = new GameObject(LAYER_BLACK_SQUARE);
    platSpr = new Sprite(*rawPlat, SPRITE_BLACK_SQUARE);
    rawPlat->AddComponent(platSpr);
    AddObject(rawPlat);
    mini_platform = GetObjectPtr(rawPlat);
    platSpr->SetScale(7.5f, 0.5f);
    rawPlat->box.SetPosition(256, 50);
    rawPlat->AddComponent(new Collider(*rawPlat));

    // rawPlat = new GameObject(LAYER_BLACK_SQUARE);
    // platSpr = new Sprite(*rawPlat, SPRITE_BLACK_SQUARE);
    // rawPlat->AddComponent(platSpr);
    // AddObject(rawPlat);
    // mini_platform = GetObjectPtr(rawPlat);
    // platSpr->SetScale(0.5f, 3.5f);
    // rawPlat->box.SetPosition(50, 150);
    // Collider* platcoll = new Collider(*rawPlat);
    // rawPlat->AddComponent(platcoll);

    // rawPlat = new GameObject(LAYER_BLACK_SQUARE);
    // platSpr = new Sprite(*rawPlat, SPRITE_BLACK_SQUARE);
    // rawPlat->AddComponent(platSpr);
    // AddObject(rawPlat);
    // mini_platform = GetObjectPtr(rawPlat);
    // platSpr->SetScale(0.5f, 3.5f);
    // rawPlat->box.SetPosition(450, 150);
    // platcoll = new Collider(*rawPlat);
    // rawPlat->AddComponent(platcoll);
    // // platcoll->isTrigger = true;

    rawPlat = new GameObject(LAYER_BLACK_SQUARE);
    platSpr = new Sprite(*rawPlat, SPRITE_BLACK_SQUARE);
    rawPlat->AddComponent(platSpr);
    AddObject(rawPlat);
    mini_platform = GetObjectPtr(rawPlat);
    platSpr->SetScale(1.5f, 0.5f);
    // rawPlat->box.SetPosition(300, 300);
    rawPlat->box.SetPosition(150, 162.5f);
    rawPlat->AddComponent(new Collider(*rawPlat));

    GameObject* rawSquare = new GameObject(4);
    AddObject(rawSquare);
    square = GetObjectPtr(rawSquare);
    rawSquare->AddComponent(new Kid(*rawSquare));
    rawSquare->box.SetPosition(62,100);

    // GameObject* rawSquare = new GameObject(LAYER_RED_SQUARE);
    // Sprite* piss = new Sprite(*rawSquare, SPRITE_RED_SQUARE);
    // piss->SetScale(0.5f);
    // rawSquare->AddComponent(piss);
    // AddObject(rawSquare);
    // square = GetObjectPtr(rawSquare);
    // rawSquare->AddComponent(new Ball(*rawSquare));
    // rawSquare->box.SetPosition(0,0);

    // rawSquare = new GameObject(4);
    // rawSquare->AddComponent(new LoopedBackground(*rawSquare, SPRITE_RED_SQUARE));
    // rawSquare->box.SetPosition(512,300);
    // AddObject(rawSquare);

    // rawPlat = new GameObject(LAYER_BLACK_SQUARE);
    // rawPlat->AddComponent(new CameraBox(*rawPlat, rawSquare, 0, 10));
    // AddObject(rawPlat);

    rawSquare = new GameObject(LAYER_RED_SQUARE);
    AddObject(rawSquare);
    square = GetObjectPtr(rawSquare);
    rawSquare->AddComponent(new EnemyArmadillo(*rawSquare));
    rawSquare->box.SetPosition(262,100);
}

void TestScene0::Start () {
    Camera::Follow(square.lock().get());
    lbg->SetPosition(0,0);
}

void TestScene0::Update (float dt) {
    if (InputManager::GetInstance().KeyPress(KEY_ESCAPE)) {
        Game::GetInstance().AddState(new TestScene1());
        popRequested = true;
    }
}

TestScene1::TestScene1 () {
    // fazer o bg loop dps
    GameObject* bg = new GameObject(-2, SCENE_TEST_LABEL);
    Sprite* sqspr = new Sprite(*bg, SCENE_TEST_BACKGROUND);
    sqspr->SetScale(0.5f);
    bg->AddComponent(sqspr);
    bg->AddComponent(new CameraFollower(*bg));
    // LoopedBackground* lbg = new LoopedBackground(*bg, SCENE_TEST_BACKGROUND, 2, 0.25f);
    // bg->AddComponent(lbg);
    AddObject(bg);

    bg = new GameObject(SCENE_TEST_LAYER, SCENE_TEST_LABEL);
    lbg = new LoopedBackground(*bg, "assets/img/lbg.png", 2, Vec2(0.25f,0.25f));
    bg->AddComponent(lbg);
    AddObject(bg);

    // GameObject* bg = new GameObject(SCENE_TEST_LAYER, SCENE_TEST_LABEL);
    // lbg = new LoopedBackground(*bg, "assets/img/fundo_teletobis.png", 2, Vec2(0.25f,0.25f));
    // bg->AddComponent(lbg);
    // AddObject(bg);
}

void TestScene1::LoadAssets () {
    GameObject* rawPlat = new GameObject(LAYER_BLACK_SQUARE, "Ground");
    Sprite* platSpr = new Sprite(*rawPlat, SPRITE_BLACK_SQUARE);
    rawPlat->AddComponent(platSpr);
    AddObject(rawPlat);
    mini_platform = GetObjectPtr(rawPlat);
    platSpr->SetScale(7.5f, 0.5f);
    // platSpr->SetScale(50, 0.5f);
    rawPlat->box.SetPosition(256, 250);
    rawPlat->AddComponent(new Collider(*rawPlat));
    // rawPlat->AddComponent(new PlatformCamera(*rawPlat, PlatformCamera::DOWN, 0.0f));

    rawPlat = new GameObject(LAYER_BLACK_SQUARE);
    platSpr = new Sprite(*rawPlat, SPRITE_BLACK_SQUARE);
    rawPlat->AddComponent(platSpr);
    AddObject(rawPlat);
    mini_platform = GetObjectPtr(rawPlat);
    platSpr->SetScale(7.5f, 0.5f);
    rawPlat->box.SetPosition(256, 50);
    rawPlat->AddComponent(new Collider(*rawPlat));

    // rawPlat = new GameObject(LAYER_BLACK_SQUARE);
    // platSpr = new Sprite(*rawPlat, SPRITE_BLACK_SQUARE);
    // rawPlat->AddComponent(platSpr);
    // AddObject(rawPlat);
    // mini_platform = GetObjectPtr(rawPlat);
    // platSpr->SetScale(0.5f, 3.5f);
    // rawPlat->box.SetPosition(50, 150);
    // Collider* platcoll = new Collider(*rawPlat);
    // rawPlat->AddComponent(platcoll);

    // rawPlat = new GameObject(LAYER_BLACK_SQUARE);
    // platSpr = new Sprite(*rawPlat, SPRITE_BLACK_SQUARE);
    // rawPlat->AddComponent(platSpr);
    // AddObject(rawPlat);
    // mini_platform = GetObjectPtr(rawPlat);
    // platSpr->SetScale(0.5f, 3.5f);
    // rawPlat->box.SetPosition(450, 150);
    // platcoll = new Collider(*rawPlat);
    // rawPlat->AddComponent(platcoll);
    // // platcoll->isTrigger = true;

    // rawPlat = new GameObject(LAYER_BLACK_SQUARE);
    // platSpr = new Sprite(*rawPlat, SPRITE_BLACK_SQUARE);
    // rawPlat->AddComponent(platSpr);
    // AddObject(rawPlat);
    // mini_platform = GetObjectPtr(rawPlat);
    // platSpr->SetScale(1.5f, 0.5f);
    // // rawPlat->box.SetPosition(300, 300);
    // rawPlat->box.SetPosition(150, 162.5f);
    // rawPlat->AddComponent(new Collider(*rawPlat));

    // GameObject* rawSquare = new GameObject(4);
    // AddObject(rawSquare);
    // square = GetObjectPtr(rawSquare);
    // rawSquare->AddComponent(new Kid(*rawSquare));
    // rawSquare->box.SetPosition(62,100);

    GameObject* rawSquare = new GameObject(LAYER_RED_SQUARE);
    Sprite* piss = new Sprite(*rawSquare, SPRITE_RED_SQUARE);
    piss->SetScale(0.5f);
    rawSquare->AddComponent(piss);
    AddObject(rawSquare);
    square = GetObjectPtr(rawSquare);
    rawSquare->AddComponent(new Ball(*rawSquare));
    rawSquare->box.SetPosition(0,0);

    // rawSquare = new GameObject(4);
    // rawSquare->AddComponent(new LoopedBackground(*rawSquare, SPRITE_RED_SQUARE));
    // rawSquare->box.SetPosition(512,300);
    // AddObject(rawSquare);

    // rawPlat = new GameObject(LAYER_BLACK_SQUARE);
    // rawPlat->AddComponent(new CameraBox(*rawPlat, rawSquare, 0, 10));
    // AddObject(rawPlat);

    // rawSquare = new GameObject(LAYER_RED_SQUARE);
    // AddObject(rawSquare);
    // square = GetObjectPtr(rawSquare);
    // rawSquare->AddComponent(new EnemyArmadillo(*rawSquare));
    // rawSquare->box.SetPosition(262,100);
}

void TestScene1::Start () {
    Camera::Follow(square.lock().get());
    lbg->SetPosition(0,0);
}

void TestScene1::Update (float dt) {
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
        Game::GetInstance().AddState(new TestScene6());
        popRequested = true;
    }
}

#define SPRITE_BG1 "assets/img/bg1.png"
#define SPRITE_BG2 "assets/img/bg2.png"
#define SPRITE_TILE "assets/img/mine.png"
#define SPRITE_FF "assets/img/Firefly.png"
//No .txt extension because TileMap does some tricks with a raw name
#define MAP_BASE "assets/map/Test"

#include "Dummy.h"
#include "DialogueBox.h"

TestScene6::TestScene6 () {
    GameObject* BgObj = new GameObject(SCENE_TEST_LAYER, SCENE_TEST_LABEL);
    Sprite* Bg1 = new Sprite(*BgObj,SPRITE_BG1,1);
    BgObj->AddComponent(Bg1);
    BgObj->AddComponent(new CameraFollower(*BgObj));
    AddObject(BgObj);
    BgObj->box.SetPosition(0,0);
    BgObj->box.SetSize(Bg1->GetWidth(), Bg1->GetHeight());
    
    GameObject* BgObj2 = new GameObject(0, SCENE_TEST_LABEL);
    LoopedBackground* Bg2 = new LoopedBackground(*BgObj2,SPRITE_BG2,3,Vec2(0.4, 0.15));
    BgObj2->AddComponent(Bg2);
    AddObject(BgObj2);
    BgObj2->box.SetPosition(0,0); 
}

// ScreenFade* Silk = nullptr;
#define CUSTOM_BLENDCOOL SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_SRC_ALPHA, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD,\
            SDL_BLENDFACTOR_ONE_MINUS_SRC_COLOR, SDL_BLENDFACTOR_ONE_MINUS_DST_COLOR, SDL_BLENDOPERATION_ADD)

void TestScene6::LoadAssets () {
    //#####################################
    GameObject* TileObj = new GameObject(1);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, 32,32);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_BASE);
    tile->LoadCollision(MAP_BASE);
    TileObj->AddComponent(tile);
    AddObject(TileObj);
    
    // GameObject* FadeObj = new GameObject(10);
    // ScreenFade* Fade = new ScreenFade(*FadeObj, Color("#000000"),0, 0, 1);
    // FadeObj->AddComponent(Fade);
    // AddObject(FadeObj);
    // Silk = Fade;

    GameObject* FireflyObj = new GameObject(11);
    Sprite* Firefly = new Sprite(*FireflyObj, SPRITE_FF, 25, 0.1);
    FireflyObj->box.SetSize(Firefly->GetWidth(), Firefly->GetHeight());
    FireflyObj->box.SetPosition(200,400);
    FireflyObj->AddComponent(Firefly);
    AddObject(FireflyObj);
    Firefly->SetBlendMode(CUSTOM_BLENDCOOL);

    GameObject* DummyObj = new GameObject(3);
    Dummy* Dum = new Dummy(*DummyObj);
    DummyObj->AddComponent(Dum);
    DummyObj->box.SetPosition(120, 70);
    AddObject(DummyObj);
    //#####################################

    GameObject* dialogueObj = new GameObject(LayerDistance::_ForeGround_VeryClose);
    DialogueBox* chat = new DialogueBox(*dialogueObj);
    dialogueObj->AddComponent(chat);
    AddObject(dialogueObj);
}

void TestScene6::Update (float dt) {
    if (InputManager::GetInstance().KeyPress(KEY_ESCAPE)) {
        Game::GetInstance().AddState(new TestScene0());
        popRequested = true;
        // Silk = nullptr;
    }
    // if (InputManager::GetInstance().KeyPress(KEY_MINUS)) {
    //     Silk->RedirectFade(0);
    // }
    // if (InputManager::GetInstance().KeyPress(KEY_EQUAL)) {
    //     Silk->RedirectFade(0.85);
    // }
}
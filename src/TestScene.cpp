#include "GentooEngine.h"
#include "TestScene.h"
#include "TestObjects.h"

TestScene::TestScene () {
    GameObject* bg = new GameObject(SCENE_TEST_LAYER, SCENE_TEST_LABEL);
    Sprite* bgSprite = new Sprite(*bg, SCENE_TEST_BACKGROUND);
    bg->AddComponent(bgSprite);
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
    rawPlat->AddComponent(new Collider(*rawPlat));

    rawPlat = new GameObject(LAYER_BLACK_SQUARE);
    platSpr = new Sprite(*rawPlat, SPRITE_BLACK_SQUARE);
    rawPlat->AddComponent(platSpr);
    AddObject(rawPlat);
    mini_platform = GetObjectPtr(rawPlat);
    platSpr->SetScale(3.0f, 1.0f);
    rawPlat->box.SetPosition(300, 300);
    rawPlat->AddComponent(new Collider(*rawPlat));

    GameObject* rawBall = new GameObject(LAYER_RED_BALL);
    rawBall->AddComponent(new Sprite(*rawBall, SPRITE_RED_BALL));
    AddObject(rawBall);
    ball = GetObjectPtr(rawBall);
    rawBall->AddComponent(new Ball(*rawBall));
    rawBall->AddComponent(new RigidBody(*rawBall));
    rawBall->AddComponent(new Collider(*rawBall));
    rawBall->box.SetPosition(512,0);
    // rawBall->box.SetPosition(512,500);
}

void TestScene::Start () {}

void TestScene::Update (float dt) {}
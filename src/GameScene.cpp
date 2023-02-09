#include "GameScene.h"
//################################################
// Surface zones
//################################################

#define SPRITE_BG1 "assets/img/bg1.png"
#define SPRITE_BG2 "assets/img/bg2.png"
#define SPRITE_TILE "assets/img/tiles.png"
#define SPRITE_FF "assets/img/Firefly.png"
#define MUSIC_DIRT "assets/audio/dirt.ogg"
#define MUSIC_CROSS "assets/audio/cross.ogg"

#include "Dummy.h"
#include "DialogueBox.h"

S1::S1() {
    GameObject* BgObj = new GameObject(LayerDistance::_Background_FarAway);
    Sprite* Bg1 = new Sprite(*BgObj,SPRITE_BG1);
    BgObj->AddComponent(Bg1);
    BgObj->AddComponent(new CameraFollower(*BgObj));
    AddObject(BgObj);
    BgObj->box.SetPosition(0,0);
    BgObj->box.SetSize(Bg1->GetWidth(), Bg1->GetHeight());
    
    GameObject* BgObj2 = new GameObject(LayerDistance::_Background_Close);
    LoopedBackground* Bg2 = new LoopedBackground(*BgObj2,SPRITE_BG2,2,Vec2(0.4, 0.15));
    BgObj2->AddComponent(Bg2);
    AddObject(BgObj2);
    BgObj2->box.SetPosition(0,0); 
}


#define CUSTOM_BLENDCOOL SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_SRC_ALPHA, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD,\
            SDL_BLENDFACTOR_ONE_MINUS_SRC_COLOR, SDL_BLENDFACTOR_ONE_MINUS_DST_COLOR, SDL_BLENDOPERATION_ADD)
#define SPRITE_RED_SQUARE "assets/img/red_square.png"
#include "TestObjects.h"

//No .txt extension because TileMap does some tricks with a raw name
#define MAP_S1 "assets/map/Zone/S1"
void S1::LoadAssets() {
    //#####################################

    stateMusic = ZoneManager::GetCarriedMusic();
    if(stateMusic == nullptr) {
        stateMusic = new Music(MUSIC_DIRT);
        stateMusic->Play(0, STATE_FADE_TIME*1000);
    }

    GameObject* TileObj = new GameObject(LayerDistance::_Environment);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE,TILE_SIZE);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_S1, 1, true);
    tile->LoadCollision(MAP_S1);
    TileObj->AddComponent(tile);
    AddObject(TileObj);

    GameObject* DummyObj = new GameObject(LayerDistance::_Player);
    Dummy* Dum = new Dummy(*DummyObj);
    DummyObj->AddComponent(Dum);
    DummyObj->box.SetBottomPosition(ZoneManager::GetSpawnPosition()*TILE_SIZE);
    AddObject(DummyObj);

    //Path to S2-A
    GameObject *AObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorA = new ZoneTransition(*AObj, Rect(86, 0, 1, 16), 
        std::make_pair(Zone::_S1, ZoneExit::A));
    AObj->AddComponent(doorA);
    AddObject(AObj);

    FadeIn();

    // GameObject* dialogueObj = new GameObject(LayerDistance::_ForeGround_VeryClose);
    // DialogueBox* chat = new DialogueBox(*dialogueObj);
    // dialogueObj->AddComponent(chat);
    // AddObject(dialogueObj);
}

void S1::Update(float dt) {
}

//################################################
#define MAP_S2 "assets/map/Zone/S2"

S2::S2() {
    GameObject* BgObj = new GameObject(LayerDistance::_Background_FarAway);
    Sprite* Bg1 = new Sprite(*BgObj,SPRITE_BG1);
    BgObj->AddComponent(Bg1);
    BgObj->AddComponent(new CameraFollower(*BgObj));
    AddObject(BgObj);
    BgObj->box.SetPosition(0,0);
    BgObj->box.SetSize(Bg1->GetWidth(), Bg1->GetHeight());
    
    GameObject* BgObj2 = new GameObject(LayerDistance::_Background_Close);
    LoopedBackground* Bg2 = new LoopedBackground(*BgObj2,SPRITE_BG2,2,Vec2(0.4, 0.15));
    BgObj2->AddComponent(Bg2);
    AddObject(BgObj2);
    BgObj2->box.SetPosition(0,0); 
}

void S2::LoadAssets() {
    //#####################################
    stateMusic = ZoneManager::GetCarriedMusic();
    if(stateMusic == nullptr) {
        stateMusic = new Music(MUSIC_DIRT);
        stateMusic->Play(0, STATE_FADE_TIME*1000);
    }

    GameObject* TileObj = new GameObject(LayerDistance::_Environment);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE, TILE_SIZE);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_S2,1,true);
    tile->LoadCollision(MAP_S2);
    TileObj->AddComponent(tile);
    AddObject(TileObj);

    GameObject* DummyObj = new GameObject(LayerDistance::_Player);
    Dummy* Dum = new Dummy(*DummyObj);
    DummyObj->AddComponent(Dum);
    DummyObj->box.SetBottomPosition(ZoneManager::GetSpawnPosition()*TILE_SIZE);
    AddObject(DummyObj);

    //Path to S1-A
    GameObject *AObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorA = new ZoneTransition(*AObj, Rect(9, 0, 1, 16), 
        std::make_pair(Zone::_S2, ZoneExit::A));
    AObj->AddComponent(doorA);
    AddObject(AObj);
    
    //Path to U1-A
    GameObject *BObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorB = new ZoneTransition(*BObj, Rect(100, 23, 1, 3), 
        std::make_pair(Zone::_S2, ZoneExit::B), false);
    BObj->AddComponent(doorB);
    AddObject(BObj);

    //Path to S3-A
    GameObject *CObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorC = new ZoneTransition(*CObj, Rect(128, 0, 1, 19), 
        std::make_pair(Zone::_S2, ZoneExit::C));
    CObj->AddComponent(doorC);
    AddObject(CObj);

    FadeIn();
}

void S2::Update(float dt) {

}

//################################################
#define MAP_S3 "assets/map/Zone/S3"
S3::S3() {
    GameObject* BgObj = new GameObject(LayerDistance::_Background_FarAway);
    Sprite* Bg1 = new Sprite(*BgObj,SPRITE_BG1);
    BgObj->AddComponent(Bg1);
    BgObj->AddComponent(new CameraFollower(*BgObj));
    AddObject(BgObj);
    BgObj->box.SetPosition(0,0);
    BgObj->box.SetSize(Bg1->GetWidth(), Bg1->GetHeight());
    
    GameObject* BgObj2 = new GameObject(LayerDistance::_Background_Close);
    LoopedBackground* Bg2 = new LoopedBackground(*BgObj2,SPRITE_BG2,2,Vec2(0.4, 0.15));
    BgObj2->AddComponent(Bg2);
    AddObject(BgObj2);
    BgObj2->box.SetPosition(0,0); 
}

void S3::LoadAssets() {
    //#####################################
    stateMusic = ZoneManager::GetCarriedMusic();
    if(stateMusic == nullptr) {
        stateMusic = new Music(MUSIC_DIRT);
        stateMusic->Play(0, STATE_FADE_TIME*1000);
    }

    GameObject* TileObj = new GameObject(LayerDistance::_Environment);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE,TILE_SIZE);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_S3,1,true);
    tile->LoadCollision(MAP_S3);
    TileObj->AddComponent(tile);
    AddObject(TileObj);

    GameObject* DummyObj = new GameObject(LayerDistance::_Player);
    Dummy* Dum = new Dummy(*DummyObj);
    DummyObj->AddComponent(Dum);
    DummyObj->box.SetBottomPosition(ZoneManager::GetSpawnPosition()*TILE_SIZE);
    AddObject(DummyObj);

    //Path to S2-B
    GameObject *AObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorA = new ZoneTransition(*AObj, Rect(10, 0, 1, 19), 
        std::make_pair(Zone::_S3, ZoneExit::A));
    AObj->AddComponent(doorA);
    AddObject(AObj);

    //Path to S4-A
    GameObject *BObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorB = new ZoneTransition(*BObj, Rect(55, 0, 1, 19), 
        std::make_pair(Zone::_S3, ZoneExit::B));
    BObj->AddComponent(doorB);
    AddObject(BObj);

    FadeIn();
}

void S3::Update(float dt) {

}

//################################################
#define MAP_S4 "assets/map/Zone/S4"
S4::S4() {
    GameObject* BgObj = new GameObject(LayerDistance::_Background_FarAway);
    Sprite* Bg1 = new Sprite(*BgObj,SPRITE_BG1);
    BgObj->AddComponent(Bg1);
    BgObj->AddComponent(new CameraFollower(*BgObj));
    AddObject(BgObj);
    BgObj->box.SetPosition(0,0);
    BgObj->box.SetSize(Bg1->GetWidth(), Bg1->GetHeight());
    
    GameObject* BgObj2 = new GameObject(LayerDistance::_Background_Close);
    LoopedBackground* Bg2 = new LoopedBackground(*BgObj2,SPRITE_BG2,2,Vec2(0.4, 0.15));
    BgObj2->AddComponent(Bg2);
    AddObject(BgObj2);
    BgObj2->box.SetPosition(0,0); 
}

void S4::LoadAssets() {
    stateMusic = ZoneManager::GetCarriedMusic();
    if(stateMusic == nullptr) {
        stateMusic = new Music(MUSIC_DIRT);
        stateMusic->Play(0, STATE_FADE_TIME*1000);
    }
    
    GameObject* TileObj = new GameObject(LayerDistance::_Environment);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE, TILE_SIZE);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_S4,1,true);
    tile->LoadCollision(MAP_S4);
    TileObj->AddComponent(tile);
    AddObject(TileObj);

    GameObject* DummyObj = new GameObject(LayerDistance::_Player);
    Dummy* Dum = new Dummy(*DummyObj);
    DummyObj->AddComponent(Dum);
    DummyObj->box.SetBottomPosition(ZoneManager::GetSpawnPosition()*TILE_SIZE);
    AddObject(DummyObj);

    //Path to S3-B
    GameObject *AObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorA = new ZoneTransition(*AObj, Rect(16, 0, 1, 19), 
        std::make_pair(Zone::_S4, ZoneExit::A));
    AObj->AddComponent(doorA);
    AddObject(AObj);
    
    //Path to U16-A
    GameObject *BObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorB = new ZoneTransition(*BObj, Rect(39, 28, 2, 1), 
        std::make_pair(Zone::_S4, ZoneExit::B), false);
    BObj->AddComponent(doorB);
    AddObject(BObj);


    FadeIn();
}

void S4::Update(float dt) {

}

//################################################
// Underground zones
//################################################

U1::U1() {
    GameObject* BgObj = new GameObject(LayerDistance::_Background_FarAway);
    Sprite* Bg1 = new Sprite(*BgObj,SPRITE_BG1);
    BgObj->AddComponent(Bg1);
    BgObj->AddComponent(new CameraFollower(*BgObj));
    AddObject(BgObj);
    BgObj->box.SetPosition(0,0);
    BgObj->box.SetSize(Bg1->GetWidth(), Bg1->GetHeight());
    
    GameObject* BgObj2 = new GameObject(LayerDistance::_Background_Close);
    LoopedBackground* Bg2 = new LoopedBackground(*BgObj2,SPRITE_BG2,2,Vec2(0.4, 0.15));
    BgObj2->AddComponent(Bg2);
    AddObject(BgObj2);
    BgObj2->box.SetPosition(0,0); 
}
#define MAP_U1 "assets/map/Zone/U1"
void U1::LoadAssets() {
    //#####################################
    stateMusic = ZoneManager::GetCarriedMusic();
    if(stateMusic == nullptr) {
        stateMusic = new Music(MUSIC_CROSS);
        stateMusic->Play(0, STATE_FADE_TIME*1000);
    }

    GameObject* TileObj = new GameObject(LayerDistance::_Environment);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE,TILE_SIZE);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_U1,1,true);
    tile->LoadCollision(MAP_U1);
    TileObj->AddComponent(tile);
    AddObject(TileObj);

    GameObject* DummyObj = new GameObject(LayerDistance::_Player);
    Dummy* Dum = new Dummy(*DummyObj);
    DummyObj->AddComponent(Dum);
    DummyObj->box.SetBottomPosition(ZoneManager::GetSpawnPosition()*TILE_SIZE);
    AddObject(DummyObj);

    //Path to S2-B
    GameObject *AObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorA = new ZoneTransition(*AObj, Rect(59, 6, 1, 3), 
        std::make_pair(Zone::_U1, ZoneExit::A), false);
    AObj->AddComponent(doorA);
    AddObject(AObj);

    //Path to U5-B
    GameObject *BObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorB = new ZoneTransition(*BObj, Rect(16, 35, 1, 4), 
        std::make_pair(Zone::_U1, ZoneExit::B));
    BObj->AddComponent(doorB);
    AddObject(BObj);

    //Path to U2-A
    GameObject *CObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorC = new ZoneTransition(*CObj, Rect(79, 36, 1, 6), 
        std::make_pair(Zone::_U1, ZoneExit::C));
    CObj->AddComponent(doorC);
    AddObject(CObj);

    FadeIn();
}

void U1::Update(float dt) {

}

U2::U2() {
    GameObject* BgObj = new GameObject(LayerDistance::_Background_FarAway);
    Sprite* Bg1 = new Sprite(*BgObj,SPRITE_BG1);
    BgObj->AddComponent(Bg1);
    BgObj->AddComponent(new CameraFollower(*BgObj));
    AddObject(BgObj);
    BgObj->box.SetPosition(0,0);
    BgObj->box.SetSize(Bg1->GetWidth(), Bg1->GetHeight());
    
    GameObject* BgObj2 = new GameObject(LayerDistance::_Background_Close);
    LoopedBackground* Bg2 = new LoopedBackground(*BgObj2,SPRITE_BG2,2,Vec2(0.4, 0.15));
    BgObj2->AddComponent(Bg2);
    AddObject(BgObj2);
    BgObj2->box.SetPosition(0,0); 
}
#define MAP_U2 "assets/map/Zone/U2"
void U2::LoadAssets() {
    //#####################################
    stateMusic = ZoneManager::GetCarriedMusic();
    if(stateMusic == nullptr) {
        stateMusic = new Music(MUSIC_CROSS);
        stateMusic->Play(0, STATE_FADE_TIME*1000);
    }

    GameObject* TileObj = new GameObject(LayerDistance::_Environment);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE,TILE_SIZE);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_U2,1,true);
    tile->LoadCollision(MAP_U2);
    TileObj->AddComponent(tile);
    AddObject(TileObj);

    GameObject* DummyObj = new GameObject(LayerDistance::_Player);
    Dummy* Dum = new Dummy(*DummyObj);
    DummyObj->AddComponent(Dum);
    DummyObj->box.SetBottomPosition(ZoneManager::GetSpawnPosition()*TILE_SIZE);
    AddObject(DummyObj);

    //Path to U1-C
    GameObject *AObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorA = new ZoneTransition(*AObj, Rect(9, 20, 1, 7), 
        std::make_pair(Zone::_U2, ZoneExit::A));
    AObj->AddComponent(doorA);
    AddObject(AObj);

    //Path to U3-B
    GameObject *BObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorB = new ZoneTransition(*BObj, Rect(9, 39, 1, 2), 
        std::make_pair(Zone::_U2, ZoneExit::B));
    BObj->AddComponent(doorB);
    AddObject(BObj);

    //Path to U8-A
    GameObject *CObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorC = new ZoneTransition(*CObj, Rect(22, 36, 1, 2), 
        std::make_pair(Zone::_U2, ZoneExit::C));
    CObj->AddComponent(doorC);
    AddObject(CObj);

    //Path to U6-A
    GameObject *DObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorD = new ZoneTransition(*DObj, Rect(22, 21, 1, 3), 
        std::make_pair(Zone::_U2, ZoneExit::D));
    DObj->AddComponent(doorD);
    AddObject(DObj);

    FadeIn();
}

void U2::Update(float dt) {

}

U3::U3() {
    GameObject* BgObj = new GameObject(LayerDistance::_Background_FarAway);
    Sprite* Bg1 = new Sprite(*BgObj,SPRITE_BG1);
    BgObj->AddComponent(Bg1);
    BgObj->AddComponent(new CameraFollower(*BgObj));
    AddObject(BgObj);
    BgObj->box.SetPosition(0,0);
    BgObj->box.SetSize(Bg1->GetWidth(), Bg1->GetHeight());
    
    GameObject* BgObj2 = new GameObject(LayerDistance::_Background_Close);
    LoopedBackground* Bg2 = new LoopedBackground(*BgObj2,SPRITE_BG2,2,Vec2(0.4, 0.15));
    BgObj2->AddComponent(Bg2);
    AddObject(BgObj2);
    BgObj2->box.SetPosition(0,0); 
}
#define MAP_U3 "assets/map/Zone/U3"
void U3::LoadAssets() {
    //#####################################
    stateMusic = ZoneManager::GetCarriedMusic();
    if(stateMusic == nullptr) {
        stateMusic = new Music(MUSIC_CROSS);
        stateMusic->Play(0, STATE_FADE_TIME*1000);
    }

    GameObject* TileObj = new GameObject(LayerDistance::_Environment);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE,TILE_SIZE);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_U3,1,true);
    tile->LoadCollision(MAP_U3);
    TileObj->AddComponent(tile);
    AddObject(TileObj);

    GameObject* DummyObj = new GameObject(LayerDistance::_Player);
    Dummy* Dum = new Dummy(*DummyObj);
    DummyObj->AddComponent(Dum);
    DummyObj->box.SetBottomPosition(ZoneManager::GetSpawnPosition()*TILE_SIZE);
    AddObject(DummyObj);

    //Path to U4-A
    GameObject *AObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorA = new ZoneTransition(*AObj, Rect(6, 13, 1, 3), 
        std::make_pair(Zone::_U3, ZoneExit::A));
    AObj->AddComponent(doorA);
    AddObject(AObj);

    //Path to U2-B
    GameObject *BObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorB = new ZoneTransition(*BObj, Rect(89, 14, 1, 2), 
        std::make_pair(Zone::_U3, ZoneExit::B));
    BObj->AddComponent(doorB);
    AddObject(BObj);

    FadeIn();
}

void U3::Update(float dt) {

}

#define U4_FUNDO "assets/img/U4/0_fundo.png"
#define U4_2 "assets/img/U4/2.png"
#define U4_5 "assets/img/U4/5.png"
#define U4_5_2 "assets/img/U4/5-2.png"
#define U4_6 "assets/img/U4/6.png"
#define U4_7 "assets/img/U4/7.png"
#define U4_7_BACK "assets/img/U4/7_back.png"
#define U4_8_TREE_BODY "assets/img/U4/8_tree_body.png"
#define U4_9_BASE "assets/img/U4/9_base.png"
#define U4_16 "assets/img/U4/16.png"
#define U4_17 "assets/img/U4/17.png"
#define U4_17_2 "assets/img/U4/17-2.png"

U4::U4() {
    GameObject* BgObj = new GameObject(LayerDistance::_Background_Far);
    Sprite* Bg1 = new Sprite(*BgObj,U4_FUNDO);
    BgObj->AddComponent(Bg1);
    AddObject(BgObj);
    BgObj->box.SetPurePosition(0,0);
    BgObj->box.SetSize(Bg1->GetWidth(), Bg1->GetHeight());
    
    BgObj = new GameObject(LayerDistance::_Background);
    Bg1 = new Sprite(*BgObj,U4_2);
    Bg1->parallaxFactor = Vec2(0.5,1);
    BgObj->AddComponent(Bg1);
    AddObject(BgObj);
    BgObj->box.SetPurePosition(0,0);
    BgObj->box.SetSize(Bg1->GetWidth(), Bg1->GetHeight());
    
    BgObj = new GameObject(LayerDistance::_Scenery_Far);
    Bg1 = new Sprite(*BgObj,U4_5);
    Bg1->parallaxFactor = Vec2(0.7,1);
    BgObj->AddComponent(Bg1);
    AddObject(BgObj);
    BgObj->box.SetPurePosition(0,0);
    BgObj->box.SetSize(Bg1->GetWidth(), Bg1->GetHeight());
    
    BgObj = new GameObject(LayerDistance::_Scenery_Far);
    Bg1 = new Sprite(*BgObj,U4_5_2);
    BgObj->AddComponent(Bg1);
    Bg1->parallaxFactor = Vec2(0.7,1);
    AddObject(BgObj);
    BgObj->box.SetPurePosition(0,0);
    BgObj->box.SetSize(Bg1->GetWidth(), Bg1->GetHeight());
    
    BgObj = new GameObject(LayerDistance::_Scenery);
    Bg1 = new Sprite(*BgObj,U4_6);
    Bg1->parallaxFactor = Vec2(0.8,1);
    BgObj->AddComponent(Bg1);
    AddObject(BgObj);
    BgObj->box.SetPurePosition(0,0);
    BgObj->box.SetSize(Bg1->GetWidth(), Bg1->GetHeight());
    
    BgObj = new GameObject(LayerDistance::_Scenery_Close);
    Bg1 = new Sprite(*BgObj,U4_7);
    Bg1->parallaxFactor = Vec2(0.9,1);
    BgObj->AddComponent(Bg1);
    AddObject(BgObj);
    BgObj->box.SetPurePosition(0,0);
    BgObj->box.SetSize(Bg1->GetWidth(), Bg1->GetHeight());
    
    BgObj = new GameObject(LayerDistance::_Scenery_Close);
    Bg1 = new Sprite(*BgObj,U4_7_BACK);
    BgObj->AddComponent(Bg1);
    Bg1->parallaxFactor = Vec2(0.9,1);
    AddObject(BgObj);
    BgObj->box.SetPurePosition(0,0);
    BgObj->box.SetSize(Bg1->GetWidth(), Bg1->GetHeight());
    
    BgObj = new GameObject(LayerDistance::_Environment_Far);
    Bg1 = new Sprite(*BgObj,U4_8_TREE_BODY);
    BgObj->AddComponent(Bg1);
    AddObject(BgObj);
    BgObj->box.SetPurePosition(0,0);
    BgObj->box.SetSize(Bg1->GetWidth(), Bg1->GetHeight());
    
    BgObj = new GameObject(LayerDistance::_Environment);
    Bg1 = new Sprite(*BgObj,U4_9_BASE);
    BgObj->AddComponent(Bg1);
    AddObject(BgObj);
    BgObj->box.SetPurePosition(0,0);
    BgObj->box.SetSize(Bg1->GetWidth(), Bg1->GetHeight());
    
    BgObj = new GameObject(LayerDistance::_ForeGround);
    Bg1 = new Sprite(*BgObj,U4_16);
    BgObj->AddComponent(Bg1);
    AddObject(BgObj);
    BgObj->box.SetPurePosition(0,0);
    BgObj->box.SetSize(Bg1->GetWidth(), Bg1->GetHeight());
    
    BgObj = new GameObject(LayerDistance::_ForeGround_Close);
    Bg1 = new Sprite(*BgObj,U4_17);
    // Bg1->parallaxFactor = Vec2(1,1);
    BgObj->AddComponent(Bg1);
    AddObject(BgObj);
    BgObj->box.SetPurePosition(0,0);
    BgObj->box.SetSize(Bg1->GetWidth(), Bg1->GetHeight());
    
    BgObj = new GameObject(LayerDistance::_ForeGround_Close);
    Bg1 = new Sprite(*BgObj,U4_17_2);
    // Bg1->parallaxFactor = Vec2(1.05,1);
    BgObj->AddComponent(Bg1);
    AddObject(BgObj);
    BgObj->box.SetPurePosition(0,0);
    BgObj->box.SetSize(Bg1->GetWidth(), Bg1->GetHeight());
    
    // GameObject* BgObj = new GameObject(LayerDistance::);
    // Sprite* Bg1 = new Sprite(*BgObj,);
    // BgObj->AddComponent(Bg1);
    // BgObj->AddComponent(new CameraFollower(*BgObj));
    // AddObject(BgObj);
    // BgObj->box.SetPosition(0,0);
    // BgObj->box.SetSize(Bg1->GetWidth(), Bg1->GetHeight());
    
    // GameObject* BgObj2 = new GameObject(LayerDistance::_Background_Close);
    // LoopedBackground* Bg2 = new LoopedBackground(*BgObj2,SPRITE_BG2,2,Vec2(0.4, 0.15));
    // BgObj2->AddComponent(Bg2);
    // AddObject(BgObj2);
    // BgObj2->box.SetPosition(0,0); 
}
#define MAP_U4 "assets/map/Zone/U4"
void U4::LoadAssets() {
    //#####################################
    stateMusic = ZoneManager::GetCarriedMusic();
    if(stateMusic == nullptr) {
        stateMusic = new Music(MUSIC_CROSS);
        stateMusic->Play(0, STATE_FADE_TIME*1000);
    }

    GameObject* TileObj = new GameObject(LayerDistance::_Background_FarAway);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE,TILE_SIZE);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_U4,1,true);
    tile->LoadCollision(MAP_U4);
    TileObj->AddComponent(tile);
    AddObject(TileObj);

    GameObject* DummyObj = new GameObject(LayerDistance::_Player);
    Dummy* Dum = new Dummy(*DummyObj);
    DummyObj->AddComponent(Dum);
    DummyObj->box.SetBottomPosition(ZoneManager::GetSpawnPosition()*TILE_SIZE);
    AddObject(DummyObj);

    //Path to U3-A
    GameObject *AObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorA = new ZoneTransition(*AObj, Rect(48, 42, 1, 3), 
        std::make_pair(Zone::_U4, ZoneExit::A));
    AObj->AddComponent(doorA);
    AddObject(AObj);

    //Path to U5-A
    GameObject *BObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorB = new ZoneTransition(*BObj, Rect(49, 22, 1, 3), 
        std::make_pair(Zone::_U4, ZoneExit::B));
    BObj->AddComponent(doorB);
    AddObject(BObj);

    FadeIn();
}

void U4::Update(float dt) {

}

U5::U5() {
    GameObject* BgObj = new GameObject(LayerDistance::_Background_FarAway);
    Sprite* Bg1 = new Sprite(*BgObj,SPRITE_BG1);
    BgObj->AddComponent(Bg1);
    BgObj->AddComponent(new CameraFollower(*BgObj));
    AddObject(BgObj);
    BgObj->box.SetPosition(0,0);
    BgObj->box.SetSize(Bg1->GetWidth(), Bg1->GetHeight());
    
    GameObject* BgObj2 = new GameObject(LayerDistance::_Background_Close);
    LoopedBackground* Bg2 = new LoopedBackground(*BgObj2,SPRITE_BG2,2,Vec2(0.4, 0.15));
    BgObj2->AddComponent(Bg2);
    AddObject(BgObj2);
    BgObj2->box.SetPosition(0,0); 
}
#define MAP_U5 "assets/map/Zone/U5"
void U5::LoadAssets() {
    //#####################################
    stateMusic = ZoneManager::GetCarriedMusic();
    if(stateMusic == nullptr) {
        stateMusic = new Music(MUSIC_CROSS);
        stateMusic->Play(0, STATE_FADE_TIME*1000);
    }

    GameObject* TileObj = new GameObject(LayerDistance::_Environment);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE,TILE_SIZE);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_U5,1,true);
    tile->LoadCollision(MAP_U5);
    TileObj->AddComponent(tile);
    AddObject(TileObj);

    GameObject* DummyObj = new GameObject(LayerDistance::_Player);
    Dummy* Dum = new Dummy(*DummyObj);
    DummyObj->AddComponent(Dum);
    DummyObj->box.SetBottomPosition(ZoneManager::GetSpawnPosition()*TILE_SIZE);
    AddObject(DummyObj);

    //Path to U4-B
    GameObject *AObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorA = new ZoneTransition(*AObj, Rect(16, 8, 1, 3), 
        std::make_pair(Zone::_U5, ZoneExit::A));
    AObj->AddComponent(doorA);
    AddObject(AObj);

    //Path to U1-B
    GameObject *BObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorB = new ZoneTransition(*BObj, Rect(48, 10, 1, 4), 
        std::make_pair(Zone::_U5, ZoneExit::B));
    BObj->AddComponent(doorB);
    AddObject(BObj);

    FadeIn();
}

void U5::Update(float dt) {

}

U6::U6() {
    GameObject* BgObj = new GameObject(LayerDistance::_Background_FarAway);
    Sprite* Bg1 = new Sprite(*BgObj,SPRITE_BG1);
    BgObj->AddComponent(Bg1);
    BgObj->AddComponent(new CameraFollower(*BgObj));
    AddObject(BgObj);
    BgObj->box.SetPosition(0,0);
    BgObj->box.SetSize(Bg1->GetWidth(), Bg1->GetHeight());
    
    GameObject* BgObj2 = new GameObject(LayerDistance::_Background_Close);
    LoopedBackground* Bg2 = new LoopedBackground(*BgObj2,SPRITE_BG2,2,Vec2(0.4, 0.15));
    BgObj2->AddComponent(Bg2);
    AddObject(BgObj2);
    BgObj2->box.SetPosition(0,0); 
}
#define MAP_U6 "assets/map/Zone/U6"
void U6::LoadAssets() {
    //#####################################
    stateMusic = ZoneManager::GetCarriedMusic();
    if(stateMusic == nullptr) {
        stateMusic = new Music(MUSIC_CROSS);
        stateMusic->Play(0, STATE_FADE_TIME*1000);
    }

    GameObject* TileObj = new GameObject(LayerDistance::_Environment);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE,TILE_SIZE);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_U6,1,true);
    tile->LoadCollision(MAP_U6);
    TileObj->AddComponent(tile);
    AddObject(TileObj);

    GameObject* DummyObj = new GameObject(LayerDistance::_Player);
    Dummy* Dum = new Dummy(*DummyObj);
    DummyObj->AddComponent(Dum);
    DummyObj->box.SetBottomPosition(ZoneManager::GetSpawnPosition()*TILE_SIZE);
    AddObject(DummyObj);

    //Path to U2-D
    GameObject *AObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorA = new ZoneTransition(*AObj, Rect(15, 18, 1, 3), 
        std::make_pair(Zone::_U6, ZoneExit::A));
    AObj->AddComponent(doorA);
    AddObject(AObj);

    //Path to U7-A
    GameObject *BObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorB = new ZoneTransition(*BObj, Rect(52, 16, 1, 3), 
        std::make_pair(Zone::_U6, ZoneExit::B));
    BObj->AddComponent(doorB);
    AddObject(BObj);
    

    FadeIn();
}

void U6::Update(float dt) {

}

U7::U7() {
    GameObject* BgObj = new GameObject(LayerDistance::_Background_FarAway);
    Sprite* Bg1 = new Sprite(*BgObj,SPRITE_BG1);
    BgObj->AddComponent(Bg1);
    BgObj->AddComponent(new CameraFollower(*BgObj));
    AddObject(BgObj);
    BgObj->box.SetPosition(0,0);
    BgObj->box.SetSize(Bg1->GetWidth(), Bg1->GetHeight());
    
    GameObject* BgObj2 = new GameObject(LayerDistance::_Background_Close);
    LoopedBackground* Bg2 = new LoopedBackground(*BgObj2,SPRITE_BG2,2,Vec2(0.4, 0.15));
    BgObj2->AddComponent(Bg2);
    AddObject(BgObj2);
    BgObj2->box.SetPosition(0,0); 
}
#define MAP_U7 "assets/map/Zone/U7"
void U7::LoadAssets() {
    //#####################################
    stateMusic = ZoneManager::GetCarriedMusic();
    if(stateMusic == nullptr) {
        stateMusic = new Music(MUSIC_CROSS);
        stateMusic->Play(0, STATE_FADE_TIME*1000);
    }

    GameObject* TileObj = new GameObject(LayerDistance::_Environment);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE,TILE_SIZE);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_U7,1,true);
    tile->LoadCollision(MAP_U7);
    TileObj->AddComponent(tile);
    AddObject(TileObj);

    GameObject* DummyObj = new GameObject(LayerDistance::_Player);
    Dummy* Dum = new Dummy(*DummyObj);
    DummyObj->AddComponent(Dum);
    DummyObj->box.SetBottomPosition(ZoneManager::GetSpawnPosition()*TILE_SIZE);
    AddObject(DummyObj);

    //Path to U6-B
    GameObject *AObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorA = new ZoneTransition(*AObj, Rect(11, 15, 1, 3), 
        std::make_pair(Zone::_U7, ZoneExit::A));
    AObj->AddComponent(doorA);
    AddObject(AObj);

    //Path to U16-B
    GameObject *BObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorB = new ZoneTransition(*BObj, Rect(49, 15, 1, 6), 
        std::make_pair(Zone::_U7, ZoneExit::B));
    BObj->AddComponent(doorB);
    AddObject(BObj);

    //Path to U9-D
    GameObject *CObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorC = new ZoneTransition(*CObj, Rect(11, 29, 1, 2), 
        std::make_pair(Zone::_U7, ZoneExit::C));
    CObj->AddComponent(doorC);
    AddObject(CObj);

    FadeIn();
}

void U7::Update(float dt) {

}

U8::U8() {
    GameObject* BgObj = new GameObject(LayerDistance::_Background_FarAway);
    Sprite* Bg1 = new Sprite(*BgObj,SPRITE_BG1);
    BgObj->AddComponent(Bg1);
    BgObj->AddComponent(new CameraFollower(*BgObj));
    AddObject(BgObj);
    BgObj->box.SetPosition(0,0);
    BgObj->box.SetSize(Bg1->GetWidth(), Bg1->GetHeight());
    
    GameObject* BgObj2 = new GameObject(LayerDistance::_Background_Close);
    LoopedBackground* Bg2 = new LoopedBackground(*BgObj2,SPRITE_BG2,2,Vec2(0.4, 0.15));
    BgObj2->AddComponent(Bg2);
    AddObject(BgObj2);
    BgObj2->box.SetPosition(0,0); 
}
#define MAP_U8 "assets/map/Zone/U8"
void U8::LoadAssets() {
    //#####################################
    stateMusic = ZoneManager::GetCarriedMusic();
    if(stateMusic == nullptr) {
        stateMusic = new Music(MUSIC_CROSS);
        stateMusic->Play(0, STATE_FADE_TIME*1000);
    }

    GameObject* TileObj = new GameObject(LayerDistance::_Environment);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE,TILE_SIZE);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_U8,1,true);
    tile->LoadCollision(MAP_U8);
    TileObj->AddComponent(tile);
    AddObject(TileObj);

    GameObject* DummyObj = new GameObject(LayerDistance::_Player);
    Dummy* Dum = new Dummy(*DummyObj);
    DummyObj->AddComponent(Dum);
    DummyObj->box.SetBottomPosition(ZoneManager::GetSpawnPosition()*TILE_SIZE);
    AddObject(DummyObj);

    //Path to U2-C
    GameObject *AObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorA = new ZoneTransition(*AObj, Rect(4, 19, 1, 2), 
        std::make_pair(Zone::_U8, ZoneExit::A));
    AObj->AddComponent(doorA);
    AddObject(AObj);

    //Path to U11-A
    GameObject *BObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorB = new ZoneTransition(*BObj, Rect(21, 24, 1, 1), 
        std::make_pair(Zone::_U8, ZoneExit::B));
    BObj->AddComponent(doorB);
    AddObject(BObj);

    //Path to U9-A
    GameObject *CObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorC = new ZoneTransition(*CObj, Rect(27, 17, 1, 2), 
        std::make_pair(Zone::_U8, ZoneExit::C));
    CObj->AddComponent(doorC);
    AddObject(CObj);

    FadeIn();
}

void U8::Update(float dt) {

}

U9::U9() {
    GameObject* BgObj = new GameObject(LayerDistance::_Background_FarAway);
    Sprite* Bg1 = new Sprite(*BgObj,SPRITE_BG1);
    BgObj->AddComponent(Bg1);
    BgObj->AddComponent(new CameraFollower(*BgObj));
    AddObject(BgObj);
    BgObj->box.SetPosition(0,0);
    BgObj->box.SetSize(Bg1->GetWidth(), Bg1->GetHeight());
    
    GameObject* BgObj2 = new GameObject(LayerDistance::_Background_Close);
    LoopedBackground* Bg2 = new LoopedBackground(*BgObj2,SPRITE_BG2,2,Vec2(0.4, 0.15));
    BgObj2->AddComponent(Bg2);
    AddObject(BgObj2);
    BgObj2->box.SetPosition(0,0); 
}
#define MAP_U9 "assets/map/Zone/U9"
void U9::LoadAssets() {
    //#####################################
    stateMusic = ZoneManager::GetCarriedMusic();
    if(stateMusic == nullptr) {
        stateMusic = new Music(MUSIC_CROSS);
        stateMusic->Play(0, STATE_FADE_TIME*1000);
    }

    GameObject* TileObj = new GameObject(LayerDistance::_Environment);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE,TILE_SIZE);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_U9,1,true);
    tile->LoadCollision(MAP_U9);
    TileObj->AddComponent(tile);
    AddObject(TileObj);

    GameObject* DummyObj = new GameObject(LayerDistance::_Player);
    Dummy* Dum = new Dummy(*DummyObj);
    DummyObj->AddComponent(Dum);
    DummyObj->box.SetBottomPosition(ZoneManager::GetSpawnPosition()*TILE_SIZE);
    AddObject(DummyObj);

    //Path to U8-C
    GameObject *AObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorA = new ZoneTransition(*AObj, Rect(8, 16, 1, 2), 
        std::make_pair(Zone::_U9, ZoneExit::A));
    AObj->AddComponent(doorA);
    AddObject(AObj);

    //Path to U13-A
    GameObject *BObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorB = new ZoneTransition(*BObj, Rect(12, 34, 1, 1), 
        std::make_pair(Zone::_U9, ZoneExit::B));
    BObj->AddComponent(doorB);
    AddObject(BObj);

    //Path to U10-A
    GameObject *CObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorC = new ZoneTransition(*CObj, Rect(24, 30, 1, 2), 
        std::make_pair(Zone::_U9, ZoneExit::C));
    CObj->AddComponent(doorC);
    AddObject(CObj);

    //Path to U7-C
    GameObject *DObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorD = new ZoneTransition(*DObj, Rect(24, 15, 1, 2), 
        std::make_pair(Zone::_U9, ZoneExit::D));
    DObj->AddComponent(doorD);
    AddObject(DObj);

    FadeIn();
}

void U9::Update(float dt) {

}

U10::U10() {
    GameObject* BgObj = new GameObject(LayerDistance::_Background_FarAway);
    Sprite* Bg1 = new Sprite(*BgObj,SPRITE_BG1);
    BgObj->AddComponent(Bg1);
    BgObj->AddComponent(new CameraFollower(*BgObj));
    AddObject(BgObj);
    BgObj->box.SetPosition(0,0);
    BgObj->box.SetSize(Bg1->GetWidth(), Bg1->GetHeight());
    
    GameObject* BgObj2 = new GameObject(LayerDistance::_Background_Close);
    LoopedBackground* Bg2 = new LoopedBackground(*BgObj2,SPRITE_BG2,2,Vec2(0.4, 0.15));
    BgObj2->AddComponent(Bg2);
    AddObject(BgObj2);
    BgObj2->box.SetPosition(0,0); 
}
#define MAP_U10 "assets/map/Zone/U10"
void U10::LoadAssets() {
    //#####################################
    stateMusic = ZoneManager::GetCarriedMusic();
    if(stateMusic == nullptr) {
        stateMusic = new Music(MUSIC_CROSS);
        stateMusic->Play(0, STATE_FADE_TIME*1000);
    }

    GameObject* TileObj = new GameObject(LayerDistance::_Environment);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE,TILE_SIZE);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_U10,1,true);
    tile->LoadCollision(MAP_U10);
    TileObj->AddComponent(tile);
    AddObject(TileObj);

    GameObject* DummyObj = new GameObject(LayerDistance::_Player);
    Dummy* Dum = new Dummy(*DummyObj);
    DummyObj->AddComponent(Dum);
    DummyObj->box.SetBottomPosition(ZoneManager::GetSpawnPosition()*TILE_SIZE);
    AddObject(DummyObj);

    //Path to U9-C
    GameObject *AObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorA = new ZoneTransition(*AObj, Rect(7, 17, 1, 2), 
        std::make_pair(Zone::_U10, ZoneExit::A));
    AObj->AddComponent(doorA);
    AddObject(AObj);

    FadeIn();
}

void U10::Update(float dt) {

}

U11::U11() {
    GameObject* BgObj = new GameObject(LayerDistance::_Background_FarAway);
    Sprite* Bg1 = new Sprite(*BgObj,SPRITE_BG1);
    BgObj->AddComponent(Bg1);
    BgObj->AddComponent(new CameraFollower(*BgObj));
    AddObject(BgObj);
    BgObj->box.SetPosition(0,0);
    BgObj->box.SetSize(Bg1->GetWidth(), Bg1->GetHeight());
    
    GameObject* BgObj2 = new GameObject(LayerDistance::_Background_Close);
    LoopedBackground* Bg2 = new LoopedBackground(*BgObj2,SPRITE_BG2,2,Vec2(0.4, 0.15));
    BgObj2->AddComponent(Bg2);
    AddObject(BgObj2);
    BgObj2->box.SetPosition(0,0); 
}
#define MAP_U11 "assets/map/Zone/U11"
void U11::LoadAssets() {
    //#####################################
    stateMusic = ZoneManager::GetCarriedMusic();
    if(stateMusic == nullptr) {
        stateMusic = new Music(MUSIC_CROSS);
        stateMusic->Play(0, STATE_FADE_TIME*1000);
    }

    GameObject* TileObj = new GameObject(LayerDistance::_Environment);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE,TILE_SIZE);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_U11,1,true);
    tile->LoadCollision(MAP_U11);
    TileObj->AddComponent(tile);
    AddObject(TileObj);

    GameObject* DummyObj = new GameObject(LayerDistance::_Player);
    Dummy* Dum = new Dummy(*DummyObj);
    DummyObj->AddComponent(Dum);
    DummyObj->box.SetBottomPosition(ZoneManager::GetSpawnPosition()*TILE_SIZE);
    AddObject(DummyObj);

    //Path to U8-B
    GameObject *AObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorA = new ZoneTransition(*AObj, Rect(54, 3, 1, 1), 
        std::make_pair(Zone::_U11, ZoneExit::A));
    AObj->AddComponent(doorA);
    AddObject(AObj);

    //Path to U12-A
    GameObject *BObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorB = new ZoneTransition(*BObj, Rect(25, 26, 1, 1), 
        std::make_pair(Zone::_U11, ZoneExit::B));
    BObj->AddComponent(doorB);
    AddObject(BObj);
}

void U11::Update(float dt) {

}

U12::U12() {
    GameObject* BgObj = new GameObject(LayerDistance::_Background_FarAway);
    Sprite* Bg1 = new Sprite(*BgObj,SPRITE_BG1);
    BgObj->AddComponent(Bg1);
    BgObj->AddComponent(new CameraFollower(*BgObj));
    AddObject(BgObj);
    BgObj->box.SetPosition(0,0);
    BgObj->box.SetSize(Bg1->GetWidth(), Bg1->GetHeight());
    
    GameObject* BgObj2 = new GameObject(LayerDistance::_Background_Close);
    LoopedBackground* Bg2 = new LoopedBackground(*BgObj2,SPRITE_BG2,2,Vec2(0.4, 0.15));
    BgObj2->AddComponent(Bg2);
    AddObject(BgObj2);
    BgObj2->box.SetPosition(0,0); 
}
#define MAP_U12 "assets/map/Zone/U12"
void U12::LoadAssets() {
    //#####################################
    stateMusic = ZoneManager::GetCarriedMusic();
    if(stateMusic == nullptr) {
        stateMusic = new Music(MUSIC_CROSS);
        stateMusic->Play(0, STATE_FADE_TIME*1000);
    }

    GameObject* TileObj = new GameObject(LayerDistance::_Environment);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE,TILE_SIZE);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_U12,1,true);
    tile->LoadCollision(MAP_U12);
    TileObj->AddComponent(tile);
    AddObject(TileObj);

    GameObject* DummyObj = new GameObject(LayerDistance::_Player);
    Dummy* Dum = new Dummy(*DummyObj);
    DummyObj->AddComponent(Dum);
    DummyObj->box.SetBottomPosition(ZoneManager::GetSpawnPosition()*TILE_SIZE);
    AddObject(DummyObj);

    //Path to U11-B
    GameObject *AObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorA = new ZoneTransition(*AObj, Rect(19, 11, 1, 1), 
        std::make_pair(Zone::_U12, ZoneExit::A));
    AObj->AddComponent(doorA);
    AddObject(AObj);

    //Path to U15-A
    GameObject *BObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorB = new ZoneTransition(*BObj, Rect(15, 16, 1, 3), 
        std::make_pair(Zone::_U12, ZoneExit::B));
    BObj->AddComponent(doorB);
    AddObject(BObj);

    //Path to U14-A
    GameObject *CObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorC = new ZoneTransition(*CObj, Rect(39, 21, 1, 1), 
        std::make_pair(Zone::_U12, ZoneExit::C));
    CObj->AddComponent(doorC);
    AddObject(CObj);

    //Path to U13-B
    GameObject *DObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorD = new ZoneTransition(*DObj, Rect(48, 13, 1, 4), 
        std::make_pair(Zone::_U12, ZoneExit::D));
    DObj->AddComponent(doorD);
    AddObject(DObj);

    FadeIn();
}

void U12::Update(float dt) {

}

U13::U13() {
    GameObject* BgObj = new GameObject(LayerDistance::_Background_FarAway);
    Sprite* Bg1 = new Sprite(*BgObj,SPRITE_BG1);
    BgObj->AddComponent(Bg1);
    BgObj->AddComponent(new CameraFollower(*BgObj));
    AddObject(BgObj);
    BgObj->box.SetPosition(0,0);
    BgObj->box.SetSize(Bg1->GetWidth(), Bg1->GetHeight());
    
    GameObject* BgObj2 = new GameObject(LayerDistance::_Background_Close);
    LoopedBackground* Bg2 = new LoopedBackground(*BgObj2,SPRITE_BG2,2,Vec2(0.4, 0.15));
    BgObj2->AddComponent(Bg2);
    AddObject(BgObj2);
    BgObj2->box.SetPosition(0,0); 
}
#define MAP_U13 "assets/map/Zone/U13"
void U13::LoadAssets() {
    //#####################################
    stateMusic = ZoneManager::GetCarriedMusic();
    if(stateMusic == nullptr) {
        stateMusic = new Music(MUSIC_CROSS);
        stateMusic->Play(0, STATE_FADE_TIME*1000);
    }

    GameObject* TileObj = new GameObject(LayerDistance::_Environment);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE,TILE_SIZE);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_U13,1,true);
    tile->LoadCollision(MAP_U13);
    TileObj->AddComponent(tile);
    AddObject(TileObj);

    GameObject* DummyObj = new GameObject(LayerDistance::_Player);
    Dummy* Dum = new Dummy(*DummyObj);
    DummyObj->AddComponent(Dum);
    DummyObj->box.SetBottomPosition(ZoneManager::GetSpawnPosition()*TILE_SIZE);
    AddObject(DummyObj);

    //Path to U12-D
    GameObject *BObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorB = new ZoneTransition(*BObj, Rect(7, 14, 1, 4), 
        std::make_pair(Zone::_U13, ZoneExit::B));
    BObj->AddComponent(doorB);
    AddObject(BObj);


    FadeIn();
}

void U13::Update(float dt) {

}

U14::U14() {
    GameObject* BgObj = new GameObject(LayerDistance::_Background_FarAway);
    Sprite* Bg1 = new Sprite(*BgObj,SPRITE_BG1);
    BgObj->AddComponent(Bg1);
    BgObj->AddComponent(new CameraFollower(*BgObj));
    AddObject(BgObj);
    BgObj->box.SetPosition(0,0);
    BgObj->box.SetSize(Bg1->GetWidth(), Bg1->GetHeight());
    
    GameObject* BgObj2 = new GameObject(LayerDistance::_Background_Close);
    LoopedBackground* Bg2 = new LoopedBackground(*BgObj2,SPRITE_BG2,2,Vec2(0.4, 0.15));
    BgObj2->AddComponent(Bg2);
    AddObject(BgObj2);
    BgObj2->box.SetPosition(0,0); 
}
#define MAP_U14 "assets/map/Zone/U14"
void U14::LoadAssets() {
    //#####################################
    stateMusic = ZoneManager::GetCarriedMusic();
    if(stateMusic == nullptr) {
        stateMusic = new Music(MUSIC_CROSS);
        stateMusic->Play(0, STATE_FADE_TIME*1000);
    }

    GameObject* TileObj = new GameObject(LayerDistance::_Environment);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE,TILE_SIZE);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_U14,1,true);
    tile->LoadCollision(MAP_U14);
    TileObj->AddComponent(tile);
    AddObject(TileObj);

    GameObject* DummyObj = new GameObject(LayerDistance::_Player);
    Dummy* Dum = new Dummy(*DummyObj);
    DummyObj->AddComponent(Dum);
    DummyObj->box.SetBottomPosition(ZoneManager::GetSpawnPosition()*TILE_SIZE);
    AddObject(DummyObj);

    //Path to U12-C
    GameObject *AObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorA = new ZoneTransition(*AObj, Rect(16, 7, 1, 1), 
        std::make_pair(Zone::_U14, ZoneExit::A));
    AObj->AddComponent(doorA);
    AddObject(AObj);

    FadeIn();
}

void U14::Update(float dt) {

}

U15::U15() {
    GameObject* BgObj = new GameObject(LayerDistance::_Background_FarAway);
    Sprite* Bg1 = new Sprite(*BgObj,SPRITE_BG1);
    BgObj->AddComponent(Bg1);
    BgObj->AddComponent(new CameraFollower(*BgObj));
    AddObject(BgObj);
    BgObj->box.SetPosition(0,0);
    BgObj->box.SetSize(Bg1->GetWidth(), Bg1->GetHeight());
    
    GameObject* BgObj2 = new GameObject(LayerDistance::_Background_Close);
    LoopedBackground* Bg2 = new LoopedBackground(*BgObj2,SPRITE_BG2,2,Vec2(0.4, 0.15));
    BgObj2->AddComponent(Bg2);
    AddObject(BgObj2);
    BgObj2->box.SetPosition(0,0); 
}
#define MAP_U15 "assets/map/Zone/U15"
void U15::LoadAssets() {
    //#####################################
    stateMusic = ZoneManager::GetCarriedMusic();
    if(stateMusic == nullptr) {
        stateMusic = new Music(MUSIC_CROSS);
        stateMusic->Play(0, STATE_FADE_TIME*1000);
    }

    GameObject* TileObj = new GameObject(LayerDistance::_Environment);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE,TILE_SIZE);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_U15,1,true);
    tile->LoadCollision(MAP_U15);
    TileObj->AddComponent(tile);
    AddObject(TileObj);

    GameObject* DummyObj = new GameObject(LayerDistance::_Player);
    Dummy* Dum = new Dummy(*DummyObj);
    DummyObj->AddComponent(Dum);
    DummyObj->box.SetBottomPosition(ZoneManager::GetSpawnPosition()*TILE_SIZE);
    AddObject(DummyObj);

    //Path to U12-B
    GameObject *AObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorA = new ZoneTransition(*AObj, Rect(98, 21, 1, 3), 
        std::make_pair(Zone::_U15, ZoneExit::A));
    AObj->AddComponent(doorA);
    AddObject(AObj);

    FadeIn();
}

void U15::Update(float dt) {

}

U16::U16() {
    GameObject* BgObj = new GameObject(LayerDistance::_Background_FarAway);
    Sprite* Bg1 = new Sprite(*BgObj,SPRITE_BG1);
    BgObj->AddComponent(Bg1);
    BgObj->AddComponent(new CameraFollower(*BgObj));
    AddObject(BgObj);
    BgObj->box.SetPosition(0,0);
    BgObj->box.SetSize(Bg1->GetWidth(), Bg1->GetHeight());
    
    GameObject* BgObj2 = new GameObject(LayerDistance::_Background_Close);
    LoopedBackground* Bg2 = new LoopedBackground(*BgObj2,SPRITE_BG2,2,Vec2(0.4, 0.15));
    BgObj2->AddComponent(Bg2);
    AddObject(BgObj2);
    BgObj2->box.SetPosition(0,0); 
}
#define MAP_U16 "assets/map/Zone/U16"
void U16::LoadAssets() {
    //#####################################
    stateMusic = ZoneManager::GetCarriedMusic();
    if(stateMusic == nullptr) {
        stateMusic = new Music(MUSIC_CROSS);
        stateMusic->Play(0, STATE_FADE_TIME*1000);
    }

    GameObject* TileObj = new GameObject(LayerDistance::_Environment);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE,TILE_SIZE);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_U16,1,true);
    tile->LoadCollision(MAP_U16);
    TileObj->AddComponent(tile);
    AddObject(TileObj);

    GameObject* DummyObj = new GameObject(LayerDistance::_Player);
    Dummy* Dum = new Dummy(*DummyObj);
    DummyObj->AddComponent(Dum);
    DummyObj->box.SetBottomPosition(ZoneManager::GetSpawnPosition()*TILE_SIZE);
    AddObject(DummyObj);

    //Path to S4-B
    GameObject *AObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorA = new ZoneTransition(*AObj, Rect(11, 2, 2, 1), 
        std::make_pair(Zone::_U16, ZoneExit::A), false);
    AObj->AddComponent(doorA);
    AddObject(AObj);

    //Path to U7-B
    GameObject *BObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorB = new ZoneTransition(*BObj, Rect(4, 25, 1, 6), 
        std::make_pair(Zone::_U16, ZoneExit::B));
    BObj->AddComponent(doorB);
    AddObject(BObj);

    FadeIn();
}

void U16::Update(float dt) {

}

//Example below
// U::U() {
//     GameObject* BgObj = new GameObject(LayerDistance::_Background_FarAway);
//     Sprite* Bg1 = new Sprite(*BgObj,SPRITE_BG1);
//     BgObj->AddComponent(Bg1);
//     BgObj->AddComponent(new CameraFollower(*BgObj));
//     AddObject(BgObj);
//     BgObj->box.SetPosition(0,0);
//     BgObj->box.SetSize(Bg1->GetWidth(), Bg1->GetHeight());
    
//     GameObject* BgObj2 = new GameObject(LayerDistance::_Background_Close);
//     LoopedBackground* Bg2 = new LoopedBackground(*BgObj2,SPRITE_BG2,2,Vec2(0.4, 0.15));
//     BgObj2->AddComponent(Bg2);
//     AddObject(BgObj2);
//     BgObj2->box.SetPosition(0,0); 
// }
// #define MAP_U "assets/map/Zone/U"
// void U::LoadAssets() {
//     //#####################################
//     GameObject* TileObj = new GameObject(LayerDistance::_Environment);
//     TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE,TILE_SIZE);
//     TileMap* tile = new TileMap(*TileObj, Set, MAP_U,1,true);
//     tile->LoadCollision(MAP_U);
//     TileObj->AddComponent(tile);
//     AddObject(TileObj);

//     GameObject* DummyObj = new GameObject(LayerDistance::_Player);
//     Dummy* Dum = new Dummy(*DummyObj);
//     DummyObj->AddComponent(Dum);
//     DummyObj->box.SetBottomPosition(ZoneManager::GetSpawnPosition()*TILE_SIZE);
//     AddObject(DummyObj);

//     //Path to 
//     GameObject *AObj = new GameObject(LayerDistance::_Environment);
//     ZoneTransition* doorA = new ZoneTransition(*AObj, Rect(59, 6, 1, 3), 
//         std::make_pair(Zone::_, ZoneExit::A));
//     AObj->AddComponent(doorA);
//     AddObject(AObj);

//     //Path to 
//     GameObject *BObj = new GameObject(LayerDistance::_Environment);
//     ZoneTransition* doorB = new ZoneTransition(*BObj, Rect(100, 23, 1, 3), 
//         std::make_pair(Zone::_, ZoneExit::B));
//     BObj->AddComponent(doorB);
//     AddObject(BObj);

//     //Path to 
//     GameObject *CObj = new GameObject(LayerDistance::_Environment);
//     ZoneTransition* doorC = new ZoneTransition(*CObj, Rect(128, 0, 1, 19), 
//         std::make_pair(Zone::_, ZoneExit::C));
//     CObj->AddComponent(doorC);
//     AddObject(CObj);

//     //Path to 
//     GameObject *DObj = new GameObject(LayerDistance::_Environment);
//     ZoneTransition* doorD = new ZoneTransition(*DObj, Rect(128, 0, 1, 19), 
//         std::make_pair(Zone::_, ZoneExit::D));
//     DObj->AddComponent(doorD);
//     AddObject(DObj);

//     FadeIn();
// }

// void U::Update(float dt) {

// }

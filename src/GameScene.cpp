#include "GameScene.h"
#include "Kid.h"
#include "GameData.h"
#include "Dummy.h"
#include "Boulder.h"
#include "DialogueBox.h"
#include "EnemyArmadillo.h"
#include "EnemyRunner.h"
#include "Boss.h"

#define OUTER    "assets/img/outer.png"

// #define SPRITE_BG1 "assets/img/bg1.png"
// #define SPRITE_BG2 "assets/img/bg2.png"
#define SPRITE_TILE "assets/img/tiles.png"
// #define SPRITE_FF "assets/img/Firefly.png"
#define MUSIC_DIRT "assets/audio/cen1.ogg"
#define MUSIC_CROSS "assets/audio/caverna.ogg"
#define MUSIC_MENU "assets/audio/menu.ogg"
#define MUSIC_CREDITS "assets/audio/credits.ogg"


#define MENU_TITLE "assets/img/menu/title_screen.png"
#define MENU_CREDITS "assets/img/menu/credits_screen.png"
#define BUTTON_START "assets/img/menu/start.png"
#define BUTTON_CREDITS "assets/img/menu/credits.png"
#define BUTTON_EXIT "assets/img/menu/exit.png"

Menu::Menu() {
    AddScenario(MENU_TITLE, LayerDistance::_Background);
    highlighted = 0;

    startGO = new GameObject(LayerDistance::_Background_Close);
    start = new Sprite(*startGO,BUTTON_START,2);
    startGO->box.SetSize(start->GetWidth(), start->GetHeight());
    startGO->box.SetPosition(114, 174);
    startGO->AddComponent(start);
    AddObject(startGO);
    
    creditsGO = new GameObject(LayerDistance::_Background_Close);
    credits = new Sprite(*creditsGO,BUTTON_CREDITS,2);
    creditsGO->box.SetSize(credits->GetWidth(), credits->GetHeight());
    creditsGO->box.SetPosition(227, 174);
    creditsGO->AddComponent(credits);
    AddObject(creditsGO);

    exitGO = new GameObject(LayerDistance::_Background_Close);
    exit = new Sprite(*exitGO,BUTTON_EXIT,2);
    exitGO->box.SetSize(exit->GetWidth(), exit->GetHeight());
    exitGO->box.SetPosition(341, 174);
    exitGO->AddComponent(exit);
    AddObject(exitGO);

    changeState.SetResetTime(STATE_FADE_TIME);
    changeState.Reset();
    fading = false;
}
void Menu::LoadAssets() {
    FadeIn();
    stateMusic = new Music(MUSIC_MENU);
    stateMusic->Play(-1, STATE_FADE_TIME*1000);
}

void Menu::Update(float dt){
    if(!fading){
        InputManager& input = InputManager::GetInstance();
        int direction = input.KeyPress(KEY_ARROW_RIGHT) - input.KeyPress(KEY_ARROW_LEFT);
        highlighted+=direction;
        (highlighted<0?highlighted = 2:highlighted>2?highlighted=0:highlighted);

        switch(highlighted){
            case 0:
                if(input.KeyPress(SDLK_z)) {
                    start->SetFrame(1);
                    fading = true;
                    GameObject* outObj = new GameObject(LayerDistance::_FadingLayer);
                    ScreenFade* fade = new ScreenFade(*outObj, Color("#000000"), 0, 1, STATE_FADE_TIME);
                    outObj->AddComponent(fade);
                    AddObject(outObj);
                    stateMusic->Stop(STATE_FADE_TIME*1000);

                } else {
                    start->SetFrameTime(0.5);
                    credits->SetFrame(0);
                    credits->SetFrameTime(0);
                    exit->SetFrame(0);
                    exit->SetFrameTime(0);
                }
                break;

            case 1:
                if(input.KeyPress(SDLK_z)) {
                    credits->SetFrame(1);
                    fading = true;
                    GameObject* outObj = new GameObject(LayerDistance::_FadingLayer);
                    ScreenFade* fade = new ScreenFade(*outObj, Color("#000000"), 0, 1, STATE_FADE_TIME);
                    outObj->AddComponent(fade);
                    AddObject(outObj);
                    stateMusic->Stop(STATE_FADE_TIME*1000);
                } else {
                    credits->SetFrameTime(0.5);
                    start->SetFrame(0);
                    start->SetFrameTime(0);
                    exit->SetFrame(0);
                    exit->SetFrameTime(0);
                }
                break;

            case 2:
                if(input.KeyPress(SDLK_z)) {
                    exit->SetFrame(1);
                    RequestPop();
                } else {
                    exit->SetFrameTime(0.5);
                    start->SetFrame(0);
                    start->SetFrameTime(0);
                    credits->SetFrame(0);
                    credits->SetFrameTime(0);
                }
                break;
        }
    }else{
        changeState.Update(dt);
        if(changeState.IsOver()){
            if(highlighted == 0) {
                Game::GetInstance().AddState(new H1);
            } else {
                Game::GetInstance().AddState(new Credits);
            }
            popRequested = true;
        }
    }
}

Credits::Credits() {
    AddScenario(MENU_CREDITS, LayerDistance::_Background);
    changeState.SetResetTime(STATE_FADE_TIME);
    changeState.Reset();
    fading = false;
}

void Credits::LoadAssets() {
    FadeIn();
    stateMusic = new Music(MUSIC_CREDITS);
    stateMusic->Play(-1, STATE_FADE_TIME*1000);
}

void Credits::Update(float dt){
    if(!fading and InputManager::GetInstance().KeyPress(SDLK_z)){
        fading = true;
        GameObject* outObj = new GameObject(LayerDistance::_FadingLayer);
        ScreenFade* fade = new ScreenFade(*outObj, Color("#000000"), 0, 1, STATE_FADE_TIME);
        outObj->AddComponent(fade);
        AddObject(outObj);
        stateMusic->Stop(STATE_FADE_TIME*1000);
    }else if(fading){
        changeState.Update(dt);
        if(changeState.IsOver()){
            Game::GetInstance().AddState(new Menu);
            popRequested = true;
        }
    }
}





//################################################
// Surface zones
//################################################
#define H1_0 "assets/img/H1/Mapa_00_A1_0.png"
#define H1_8 "assets/img/H1/Mapa_00_A1_8.png"
#define H1_9 "assets/img/H1/Mapa_00_A1_9.png"
#define H1_16 "assets/img/H1/Mapa_00_A1_16.png"
#define FIRE "assets/img/H1/fogo-Sheet.png"
#define MOM "assets/img/H1/mae-Sheet.png"


#define CHAT "assets/txt/01_inicio.txt"
#define CHAT2 "assets/txt/02_sair_casa.txt"

H1::H1() {
    AddScenario(H1_0, LayerDistance::_Background_FarAway);
    AddScenario(H1_8, LayerDistance::_Environment_Far);
    AddScenario(H1_9, LayerDistance::_Environment);
    AddScenario(H1_16, LayerDistance::_ForeGround);
}
//No .txt extension because TileMap does some tricks with a raw name
#define MAP_H1 "assets/map/Zone/H1"
void H1::LoadAssets() {
    stateMusic = ZoneManager::GetCarriedMusic();
    if(stateMusic == nullptr) {
        stateMusic = new Music(MUSIC_DIRT);
        stateMusic->Play(-1, STATE_FADE_TIME*1000);
    }

    GameObject* TileObj = new GameObject(LayerDistance::_Background_Far);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE,TILE_SIZE);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_H1, 1, true);
    tile->LoadCollision(MAP_H1);
    zoneSize = Vec2(tile->GetWidth()*TILE_SIZE, tile->GetHeight()*TILE_SIZE);
    TileObj->AddComponent(tile);
    AddObject(TileObj);

    AddKid();

    //Interaction Exit
    
    

    AddAnimated(FIRE, LayerDistance::_Player_Front, Vec2(15, 10), 30);
    AddAnimated(MOM, LayerDistance::_Player_Front, Vec2(12.2, 10.47), 4, 1);


    if(!GameData::firstChat)
    {
        GameObject* dialogueObj = new GameObject(LayerDistance::_ForeGround_VeryClose);
        dialogueObj->box.SetPosition(14*TILE_SIZE, 10*TILE_SIZE);
        dialogueObj->box.SetSize(TILE_SIZE,TILE_SIZE);
        Interactor* chat = new Interactor(*dialogueObj, true);
        chat->SetResult([](){

            GameObject* dialogueObj = new GameObject(LayerDistance::_ForeGround_VeryClose);
            dialogueObj->AddComponent(new DialogueBox(*dialogueObj,CHAT));
            Game::GetInstance().GetCurrentState().AddObject(dialogueObj);      

            GameObject* interact = new GameObject(LayerDistance::_ForeGround_VeryClose);
            interact->box.SetPosition(14*TILE_SIZE, 10*TILE_SIZE);
            interact->box.SetSize(TILE_SIZE,TILE_SIZE);
            Interactor* chat = new Interactor(*interact);

            chat->SetCondition([](){return InputManager::GetInstance().KeyPress(SDLK_z);});
            chat->SetResult([](){
                GameObject* dialogueObj = new GameObject(LayerDistance::_ForeGround_VeryClose);
                dialogueObj->AddComponent(new DialogueBox(*dialogueObj,CHAT2));
                Game::GetInstance().GetCurrentState().AddObject(dialogueObj);
                GameData::firstChat = true;

                GameObject* intObj = new GameObject(LayerDistance::_Environment_Close);
                intObj->box = Rect(0,0,TILE_SIZE,TILE_SIZE);
                intObj->box.SetPurePosition(16*TILE_SIZE,10*TILE_SIZE);
                Interactor* output = new Interactor(*intObj);
                output->SetCondition([](){return InputManager::GetInstance().KeyPress(SDLK_z);});
                output->SetResult([](){
                    GameObject *AObj = new GameObject(LayerDistance::_Environment);
                    ZoneTransition* doorA = new ZoneTransition(*AObj, Rect(16, 10, 1, 1), 
                    std::make_pair(Zone::_H1, ZoneExit::A));
                    AObj->AddComponent(doorA);
                    Game::GetInstance().GetCurrentState().AddObject(AObj);
                });
                intObj->AddComponent(output);       
                Game::GetInstance().GetCurrentState().AddObject(intObj);        
            });
            interact->AddComponent(chat);
            Game::GetInstance().GetCurrentState().AddObject(interact); 
        });
        dialogueObj->AddComponent(chat);
        AddObject(dialogueObj);

    }else{
        GameObject* interact = new GameObject(LayerDistance::_ForeGround_VeryClose);
        interact->box.SetPosition(14*TILE_SIZE, 10*TILE_SIZE);
        interact->box.SetSize(TILE_SIZE,TILE_SIZE);
        Interactor* chat = new Interactor(*interact);
        chat->SetCondition([](){return InputManager::GetInstance().KeyPress(SDLK_z);});
        chat->SetResult([](){
            GameObject* dialogueObj = new GameObject(LayerDistance::_ForeGround_VeryClose);
            dialogueObj->AddComponent(new DialogueBox(*dialogueObj,CHAT2));
            Game::GetInstance().GetCurrentState().AddObject(dialogueObj);
        });
        interact->AddComponent(chat);
        AddObject(interact); 


        GameObject* intObj = new GameObject(LayerDistance::_Environment_Close);
        intObj->box = Rect(0,0,TILE_SIZE,TILE_SIZE);
        intObj->box.SetPurePosition(16*TILE_SIZE,10*TILE_SIZE);
        Interactor* output = new Interactor(*intObj);
        output->SetCondition([](){return InputManager::GetInstance().KeyPress(SDLK_z);});
        output->SetResult([](){
            GameObject *AObj = new GameObject(LayerDistance::_Environment);
            ZoneTransition* doorA = new ZoneTransition(*AObj, Rect(16, 10, 1, 1), 
            std::make_pair(Zone::_H1, ZoneExit::A));
            AObj->AddComponent(doorA);
            Game::GetInstance().GetCurrentState().AddObject(AObj)
        ;});
        intObj->AddComponent(output);
        AddObject(intObj);
    }
    FadeIn();
    // GameObject* dialogueObj = new GameObject(LayerDistance::_ForeGround_VeryClose);
    // DialogueBox* chat = new DialogueBox(*dialogueObj);
    // dialogueObj->AddComponent(chat);
    // AddObject(dialogueObj);
}

void H1::Update(float dt) {
}














//################################################
#define S1_0 "assets/img/S1/Mapa_01_A1_0.png"
#define S1_2 "assets/img/S1/Mapa_01_A1_2.png"
#define S1_3 "assets/img/S1/Mapa_01_A1_3.png"
#define S1_4 "assets/img/S1/Mapa_01_A1_4.png"
#define S1_5 "assets/img/S1/Mapa_01_A1_5.png"
#define S1_6 "assets/img/S1/Mapa_01_A1_6.png"
#define S1_8 "assets/img/S1/Mapa_01_A1_8f.png"
#define S1_PLATAFORMA "assets/img/S1/Mapa_01_A1_plataforma.png"
#define S1_9 "assets/img/S1/Mapa_01_A1_9.png"
#define S1_16 "assets/img/S1/Mapa_01_A1_16.png"
#define S1_17 "assets/img/S1/Mapa_01_A1_17f.png"

#define TREE_0B "assets/img/animated/tree/0b/0b-Sheet.png"
#define TREE_0B_ "assets/img/animated/tree/0b_/0b_-Sheet.png"
#define TREE_0A "assets/img/animated/tree/0a/0a-Sheet.png"
#define TREE_0A_ "assets/img/animated/tree/0a_/0a_-Sheet.png"
#define TREE_1A "assets/img/animated/tree/1a/1a-Sheet.png"
#define TREE_1A_ "assets/img/animated/tree/1a_/1a_-Sheet.png"
#define TREE_1B "assets/img/animated/tree/1b/1b-Sheet.png"
#define TREE_1B_ "assets/img/animated/tree/1b_/1b_-Sheet.png"
#define TREE_1C "assets/img/animated/tree/1c/1c-Sheet.png"
#define TREE_1C_ "assets/img/animated/tree/1c_/1c_-Sheet.png"
#define TREE_1D "assets/img/animated/tree/1d/1d-Sheet.png"
#define TREE_1D_ "assets/img/animated/tree/1d_/1d_-Sheet.png"
#define TREE_1E "assets/img/animated/tree/1e/1e-Sheet.png"
#define TREE_1E_ "assets/img/animated/tree/1e_/1e_-Sheet.png"
#define TREE_2A "assets/img/animated/tree/2a/2a-Sheet.png"
#define TREE_2B "assets/img/animated/tree/2b/2b-Sheet.png"
#define TREE_2C "assets/img/animated/tree/2c/2c-Sheet.png"

#define CHECKPOINT_ANIM "assets/img/animated/10_checkpoint_89x84.png"

S1::S1() {
    AddScenario(S1_0, LayerDistance::_Background_FarAway);
    AddScenario(S1_2, LayerDistance::_Background, 0.4);
    AddScenario(S1_3, LayerDistance::_Background_Close, 0.5);
    AddScenario(S1_4, LayerDistance::_Background_VeryClose, 0.55);
    AddScenario(S1_5, LayerDistance::_Scenery_Far, 0.6);
    AddScenario(S1_6, LayerDistance::_Scenery, 0.7);
    AddScenario(S1_PLATAFORMA, LayerDistance::_Environment_Far);
    AddScenario(S1_8, LayerDistance::_Environment_Far, 0.9);
    AddScenario(S1_9, LayerDistance::_Environment);
    AddScenario(S1_16, LayerDistance::_ForeGround);
    AddScenario(S1_17, LayerDistance::_ForeGround_Close, 1.15);
 
    AddAnimated(TREE_0B, LayerDistance::_Environment_Far,Vec2(2,9), 15);
    AddAnimated(TREE_0A, LayerDistance::_Environment_Far,Vec2(48,8), 15);
    AddAnimated(TREE_0B, LayerDistance::_Environment_Far,Vec2(84,7), 15);

    AddAnimated(TREE_1A, LayerDistance::_Environment_Far,Vec2(14,10), 15);
    AddAnimated(TREE_1B, LayerDistance::_Environment_Far,Vec2(27,10), 15);
    AddAnimated(TREE_2A, LayerDistance::_Environment_Far,Vec2(52,9), 15);
    AddAnimated(TREE_2C, LayerDistance::_Environment_Far,Vec2(59,9), 15);
    AddAnimated(TREE_1E, LayerDistance::_Environment_Far,Vec2(91,7), 15);

    AddAnimated(TREE_1E, LayerDistance::_Environment_Far,Vec2(0,10), 15);
    AddAnimated(TREE_1D, LayerDistance::_Environment_Far,Vec2(23,10), 15);
    AddAnimated(TREE_1C, LayerDistance::_Environment_Far,Vec2(44,8), 15);
    AddAnimated(TREE_1B, LayerDistance::_Environment_Far,Vec2(70,9), 15);
}

//No .txt extension because TileMap does some tricks with a raw name
#define MAP_S1 "assets/map/Zone/S1"



void S1::LoadAssets() {
    //#####################################

    stateMusic = ZoneManager::GetCarriedMusic();
    if(stateMusic == nullptr) {
        stateMusic = new Music(MUSIC_DIRT);
        stateMusic->Play(-1, STATE_FADE_TIME*1000);
    }

    GameObject* TileObj = new GameObject(LayerDistance::_Background_Far);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE,TILE_SIZE);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_S1, 1, true);
    tile->LoadCollision(MAP_S1);
    zoneSize = Vec2(tile->GetWidth()*TILE_SIZE, tile->GetHeight()*TILE_SIZE);
    TileObj->AddComponent(tile);
    AddObject(TileObj);

    AddKid();

    //Path to S2-A
    GameObject *AObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorA = new ZoneTransition(*AObj, Rect(86, 0, 1, 16), 
        std::make_pair(Zone::_S1, ZoneExit::A));
    AObj->AddComponent(doorA);
    AddObject(AObj);

    //Interaction Exit
    GameObject* intObj = new GameObject(LayerDistance::_Environment_Close);
    intObj->box = Rect(0,0,TILE_SIZE,TILE_SIZE);
    intObj->box.SetPurePosition(13*TILE_SIZE,18*TILE_SIZE);
    Interactor* output = new Interactor(*intObj);
    output->SetCondition([](){return InputManager::GetInstance().KeyPress(SDLK_z);});
    output->SetResult([](){
        GameObject *AObj = new GameObject(LayerDistance::_Environment);
        ZoneTransition* doorA = new ZoneTransition(*AObj, Rect(13, 18, 1, 1), 
        std::make_pair(Zone::_S1, ZoneExit::B));
        AObj->AddComponent(doorA);
        Game::GetInstance().GetCurrentState().AddObject(AObj);}
    );
    intObj->AddComponent(output);
    AddObject(intObj);

    //Barrier on left side
    GameObject* limitObj = new GameObject(LayerDistance::_Environment_Close);
    limitObj->AddComponent(new Collider(*limitObj));
    limitObj->box = Rect(7.5*TILE_SIZE,0,TILE_SIZE, TILE_SIZE*19);
    AddObject(limitObj);

    AddSpikes(80,18,3);

    GameObject* checkObj = new GameObject(LayerDistance::_Environment);
    checkObj->box = Rect(0,0,TILE_SIZE,TILE_SIZE);
    checkObj->box.SetPurePosition(13*TILE_SIZE,18*TILE_SIZE);
    Interactor* checkSave = new Interactor(*checkObj);
    checkSave->SetResult([](){
        GameData::checkPoint = Zone::_S1;
        GameData::revivePosition = Vec2(13,18);
    });
    checkObj->AddComponent(checkSave);
    AddObject(checkObj);
    
    LimitMap();
    FadeIn();

    
}

void S1::Update(float dt) {
}














//################################################
#define MAP_S2 "assets/map/Zone/S2"

#define S2_0 "assets/img/S2/Mapa_01_A2_0.png"
#define S2_2 "assets/img/S2/Mapa_01_A2_2.png"
#define S2_3 "assets/img/S2/Mapa_01_A2_3.png"
#define S2_4 "assets/img/S2/Mapa_01_A2_4.png"
#define S2_5 "assets/img/S2/Mapa_01_A2_5.png"
#define S2_6 "assets/img/S2/Mapa_01_A2_6.png"
#define S2_PLATAFORMA "assets/img/S2/Mapa_01_A2_plataformas.png"
#define S2_8 "assets/img/S2/Mapa_01_A2_8.png"
#define S2_9 "assets/img/S2/Mapa_01_A2_9.png"
#define S2_16 "assets/img/S2/Mapa_01_A2_16.png"
#define S2_17 "assets/img/S2/Mapa_01_A2_17f.png"
#define S2_WF "assets/img/S2/Mapa_01_A2_cachoeira_Sheet.png"
#define POTION_ITEM "assets/img/animated/item_0-Sheet.png"
#define GATE_COUNTING "assets/img/animated/gateindicator74x206-5f.png"
#define GATE_OPENING "assets/img/animated/gatefull74x206-26f-sheet.png"
#define FERRO "assets/img/animated/ferro-48x90_10f.png"

S2::S2() {
    AddScenario(S2_0, LayerDistance::_Background_FarAway);
    AddScenario(S2_2, LayerDistance::_Background, 0.45);
    AddScenario(S2_3, LayerDistance::_Background_Close, 0.5);
    AddScenario(S2_4, LayerDistance::_Background_VeryClose, 0.55);
    AddScenario(S2_5, LayerDistance::_Scenery_Far, 0.6);
    AddScenario(S2_6, LayerDistance::_Scenery, 0.7);
    AddScenario(S2_PLATAFORMA, LayerDistance::_Environment_Far);
    AddScenario(S2_8, LayerDistance::_Environment_Far);
    AddAnimated(S2_WF, LayerDistance::_Environment,Vec2(60.7,16.6), 10);
    AddScenario(S2_9, LayerDistance::_Environment);
    AddScenario(S2_16, LayerDistance::_ForeGround);
    AddScenario(S2_17, LayerDistance::_ForeGround_Close, 1.15);

    AddAnimated(TREE_0A, LayerDistance::_Environment_Far,Vec2(-16,7), 15);
    AddAnimated(TREE_0B, LayerDistance::_Environment_Far,Vec2(83,7), 15);
    AddAnimated(TREE_0A, LayerDistance::_Environment_Far,Vec2(125,9), 15);

    AddAnimated(TREE_2C, LayerDistance::_Environment_Far,Vec2(4,7), 15);
    AddAnimated(TREE_1B, LayerDistance::_Environment_Far,Vec2(25,10), 15);
    AddAnimated(TREE_1E, LayerDistance::_Environment_Far,Vec2(68,6), 15);
    AddAnimated(TREE_1A, LayerDistance::_Environment_Far,Vec2(77,6), 15);
    AddAnimated(TREE_2C, LayerDistance::_Environment_Far,Vec2(93,8), 15);
    AddAnimated(TREE_1E, LayerDistance::_Environment_Far,Vec2(102,8), 15);
    AddAnimated(TREE_2B, LayerDistance::_Environment_Far,Vec2(124,10), 15);
    AddAnimated(TREE_2A, LayerDistance::_Environment_Far,Vec2(129,10), 15);
    AddAnimated(TREE_2C, LayerDistance::_Environment_Far,Vec2(138,10), 15);
    
    AddAnimated(TREE_1C, LayerDistance::_Environment_Far,Vec2(19,10), 15);
    AddAnimated(TREE_2B, LayerDistance::_Environment_Far,Vec2(73,6), 15);

    
}

void S2::LoadAssets() {
    //#####################################
    stateMusic = ZoneManager::GetCarriedMusic();
    if(stateMusic == nullptr) {
        stateMusic = new Music(MUSIC_DIRT);
        stateMusic->Play(-1, STATE_FADE_TIME*1000);
    }

    GameObject* TileObj = new GameObject(LayerDistance::_Background_Far);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE, TILE_SIZE);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_S2,1,true);
    tile->LoadCollision(MAP_S2);
    zoneSize = Vec2(tile->GetWidth()*TILE_SIZE, tile->GetHeight()*TILE_SIZE);
    TileObj->AddComponent(tile);
    AddObject(TileObj);

    AddKid();

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
    AddSpikes(14,19,3);
    AddSpikes(44,20,25);


    GameObject* gateObj = new GameObject(LayerDistance::_Environment_Close);
    if(!GameData::openedU2Gate){
        Sprite* gateIndication = new Sprite(*gateObj, FERRO, 10);
        gateIndication->SetFrame(0);
        gateObj->box.SetSize(gateIndication->GetWidth(), gateIndication->GetHeight());
        gateObj->box.SetPurePosition(100*TILE_SIZE, 22.8*TILE_SIZE);
        Interactor* gateOpener = new Interactor(*gateObj, GameData::firstChatAlche, false);
        gateOpener->SetCondition([](){return GameData::firstChatAlche;});
        gateOpener->SetResult([](){
            GameObject* gateObj = new GameObject(LayerDistance::_Environment_Close);
            Sprite* gate = new Sprite(*gateObj, FERRO, 10, 0.1, true);
            gateObj->box.SetSize(gate->GetWidth(), gate->GetHeight());
            gateObj->box.SetPurePosition(100*TILE_SIZE, 22.8*TILE_SIZE);
            Collider* gateLimit = new Collider(*gateObj);
            gateLimit->offset = Vec2(0, -45);
            gateObj->AddComponent(gate);
            gateObj->AddComponent(gateLimit);
            Game::GetInstance().GetCurrentState().AddObject(gateObj);
            GameData::openedU2Gate = true;
        });
        gateObj->AddComponent(gateIndication);
        gateObj->AddComponent(gateOpener);
        
    } else {
        Sprite* gate = new Sprite(*gateObj, FERRO, 10);
        gateObj->box.SetSize(gate->GetWidth(), gate->GetHeight());
        gateObj->box.SetPurePosition(100*TILE_SIZE, 22.8*TILE_SIZE);
        gate->SetFrame(9);
        Collider* gateLimit = new Collider(*gateObj);
        gateLimit->offset = Vec2(0, -45);
        gateObj->AddComponent(gate);
        gateObj->AddComponent(gateLimit);
    }
    AddObject(gateObj);


    GameObject* mouseGO = new GameObject(LayerDistance::_NPC);
    EnemyArmadillo* mouseCP = new EnemyArmadillo(*mouseGO);
    mouseGO->AddComponent(mouseCP);
    mouseGO->box.SetPosition(700,500);
    AddObject(mouseGO);

    mouseGO = new GameObject(LayerDistance::_NPC);
    mouseCP = new EnemyArmadillo(*mouseGO);
    mouseCP->FlipEntity();
    mouseGO->AddComponent(mouseCP);
    mouseGO->box.SetPosition(2870,440);
    AddObject(mouseGO);

    mouseGO = new GameObject(LayerDistance::_NPC);
    mouseCP = new EnemyArmadillo(*mouseGO);
    mouseGO->AddComponent(mouseCP);
    mouseGO->box.SetPosition(2870,440);
    AddObject(mouseGO);


    LimitMap();
    FadeIn();
}

void S2::Update(float dt) {

}














//################################################
#define MAP_S3 "assets/map/Zone/S3"

#define S3A_0 "assets/img/S3_A/Mapa_01_A3a_0.png"
#define S3A_2 "assets/img/S3_A/Mapa_01_A3a_2.png"
#define S3A_3 "assets/img/S3_A/Mapa_01_A3a_3.png"
#define S3A_4 "assets/img/S3_A/Mapa_01_A3a_4.png"
#define S3A_5 "assets/img/S3_A/Mapa_01_A3a_5.png"
#define S3A_6 "assets/img/S3_A/Mapa_01_A3a_6.png"
#define S3A_8 "assets/img/S3_A/Mapa_01_A3a_8.png"
#define S3A_9 "assets/img/S3_A/Mapa_01_A3a_9.png"
#define S3A_16 "assets/img/S3_A/Mapa_01_A3a_16.png"
#define S3A_17 "assets/img/S3_A/Mapa_01_A3a_17.png"

#define S3B_0 "assets/img/S3_B/Mapa_01_A3b_0.png"
#define S3B_2 "assets/img/S3_B/Mapa_01_A3b_2.png"
#define S3B_3 "assets/img/S3_B/Mapa_01_A3b_3.png"
#define S3B_4 "assets/img/S3_B/Mapa_01_A3b_4.png"
#define S3B_5 "assets/img/S3_B/Mapa_01_A3b_5.png"
#define S3B_6 "assets/img/S3_B/Mapa_01_A3b_6.png"
#define S3B_8 "assets/img/S3_B/Mapa_01_A3b_8.png"
#define S3B_9 "assets/img/S3_B/Mapa_01_A3b_9.png"
#define S3B_16 "assets/img/S3_B/Mapa_01_A3b_16.png"
#define S3B_17 "assets/img/S3_B/Mapa_01_A3b_17.png"


S3::S3() {
    if(GameData::changedS3Scenario) {
        AddScenario(S3B_0, LayerDistance::_Background_FarAway);
        AddScenario(S3B_2, LayerDistance::_Background, 0.45);
        AddScenario(S3B_3, LayerDistance::_Background_Close, 0.5);
        AddScenario(S3B_4, LayerDistance::_Background_VeryClose, 0.55);
        AddScenario(S3B_5, LayerDistance::_Scenery_Far, 0.6);
        AddScenario(S3B_6, LayerDistance::_Scenery, 0.7);
        AddScenario(S3B_8, LayerDistance::_Environment_Far);
        AddScenario(S3B_9, LayerDistance::_Environment);
        AddScenario(S3B_16, LayerDistance::_ForeGround);
        AddScenario(S3B_17, LayerDistance::_ForeGround_Close, 1.15);

        AddAnimated(TREE_0A_, LayerDistance::_Environment_Far,Vec2(-12,9), 15);
        AddAnimated(TREE_0B_, LayerDistance::_Environment_Far,Vec2(8,9), 15);
        AddAnimated(TREE_0A_, LayerDistance::_Environment_Far,Vec2(42,9), 15);
        
        AddAnimated(TREE_1C_, LayerDistance::_Environment_Far,Vec2(4,9), 15);
        AddAnimated(TREE_1E_, LayerDistance::_Environment,Vec2(27,10), 15);
        AddAnimated(TREE_1A_, LayerDistance::_Environment,Vec2(47,10), 15);

    } else {
        AddScenario(S3A_0, LayerDistance::_Background_FarAway);
        AddScenario(S3A_2, LayerDistance::_Background, 0.45);
        AddScenario(S3A_3, LayerDistance::_Background_Close, 0.5);
        AddScenario(S3A_4, LayerDistance::_Background_VeryClose, 0.55);
        AddScenario(S3A_5, LayerDistance::_Scenery_Far, 0.6);
        AddScenario(S3A_6, LayerDistance::_Scenery, 0.7);
        AddScenario(S3A_8, LayerDistance::_Environment_Far);
        AddScenario(S3A_9, LayerDistance::_Environment);
        AddScenario(S3A_16, LayerDistance::_ForeGround);
        AddScenario(S3A_17, LayerDistance::_ForeGround_Close, 1.15);

        AddAnimated(TREE_0A, LayerDistance::_Environment_Far,Vec2(-12,9), 15);
        AddAnimated(TREE_0B, LayerDistance::_Environment_Far,Vec2(8,9), 15);
        AddAnimated(TREE_0A, LayerDistance::_Environment_Far,Vec2(42,9), 15);
        
        AddAnimated(TREE_1C, LayerDistance::_Environment_Far,Vec2(4,9), 15);
        AddAnimated(TREE_1E, LayerDistance::_Environment,Vec2(27,10), 15);
        AddAnimated(TREE_1A, LayerDistance::_Environment,Vec2(47,10), 15);
    }
}

#define ALCHE_IDLE "assets/img/alchemist/idle.png"
#define ALCHE_CHAT "assets/img/alchemist/chat.png"

#define CHAT3 "assets/txt/04_alquim.txt"
#define CHAT4 "assets/txt/05_int_alquim.txt"

void S3::LoadAssets() {
    //#####################################
    stateMusic = ZoneManager::GetCarriedMusic();
    if(stateMusic == nullptr) {
        stateMusic = new Music(MUSIC_DIRT);
        stateMusic->Play(-1, STATE_FADE_TIME*1000);
    }

    GameObject* TileObj = new GameObject(LayerDistance::_Background_Far);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE,TILE_SIZE);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_S3,1,true);
    tile->LoadCollision(MAP_S3);
    zoneSize = Vec2(tile->GetWidth()*TILE_SIZE, tile->GetHeight()*TILE_SIZE);
    TileObj->AddComponent(tile);
    AddObject(TileObj);

    AddKid();

    //Path to S2-B
    GameObject *AObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorA = new ZoneTransition(*AObj, Rect(9, 0, 1, 19), 
        std::make_pair(Zone::_S3, ZoneExit::A));
    AObj->AddComponent(doorA);
    AddObject(AObj);

    //Path to S4-A
    GameObject *BObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorB = new ZoneTransition(*BObj, Rect(56, 0, 1, 19), 
        std::make_pair(Zone::_S3, ZoneExit::B));
    BObj->AddComponent(doorB);
    AddObject(BObj);

    if(GameData::changedS3Scenario) { //TODO ENABLE CODE AFTER DEBUGGING MAP
        //Barrier on left side
        GameObject* limitObj = new GameObject(LayerDistance::_Environment_Close);
        limitObj->AddComponent(new Collider(*limitObj));
        limitObj->box = Rect(10*TILE_SIZE,0,TILE_SIZE, TILE_SIZE*20);
        AddObject(limitObj);

        GameObject* bossGO = new GameObject(LayerDistance::_NPC);
        Boss* bossCP = new Boss(*bossGO);
        bossGO->AddComponent(bossCP);
        AddObject(bossGO);

    } else {
        //Barrier on right side
        GameObject* limitObj = new GameObject(LayerDistance::_Environment_Close);
        limitObj->AddComponent(new Collider(*limitObj));
        limitObj->box = Rect(55*TILE_SIZE,0,TILE_SIZE, TILE_SIZE*20);
        AddObject(limitObj);

        //Alchemist interaction
        GameObject *alche =  new GameObject(LayerDistance::_NPC);
        Sprite* idle = new Sprite(*alche, ALCHE_IDLE, 6, 0.1);
        alche->AddComponent(idle);
        alche->box.SetSize(idle->GetWidth(), idle->GetHeight());
        alche->box.SetPurePosition(40*TILE_SIZE, 17*TILE_SIZE);

        Interactor* alcheTalk = new Interactor(*alche);
        alcheTalk->SetCondition([](){return InputManager::GetInstance().KeyPress(SDLK_z);});
        alcheTalk->SetResult([](){
            if(!GameData::firstChatAlche){
                GameObject* dialogueObj = new GameObject(LayerDistance::_ForeGround_VeryClose);
                dialogueObj->AddComponent(new DialogueBox(*dialogueObj,CHAT3));
                Game::GetInstance().GetCurrentState().AddObject(dialogueObj);
                GameData::firstChatAlche = true;
            } else {
                GameObject* dialogueObj = new GameObject(LayerDistance::_ForeGround_VeryClose);
                dialogueObj->AddComponent(new DialogueBox(*dialogueObj,CHAT4));
                Game::GetInstance().GetCurrentState().AddObject(dialogueObj);
            }
        });
        alche->AddComponent(alcheTalk);
        AddObject(alche);
    }

    LimitMap();
    FadeIn();
}

void S3::Update(float dt) {

}














//################################################
#define MAP_S4 "assets/map/Zone/S4"

#define S4_0 "assets/img/S4/Mapa_01_A4_0.png"
#define S4_2 "assets/img/S4/Mapa_01_A4_2.png"
#define S4_3 "assets/img/S4/Mapa_01_A4_3.png"
#define S4_4 "assets/img/S4/Mapa_01_A4_4.png"
#define S4_5 "assets/img/S4/Mapa_01_A4_5.png"
#define S4_6 "assets/img/S4/Mapa_01_A4_6.png"
#define S4_7 "assets/img/S4/Mapa_01_A4_7.png"
#define S4_8 "assets/img/S4/Mapa_01_A4_8.png"
#define S4_9 "assets/img/S4/Mapa_01_A4_9.png"
#define S4_16 "assets/img/S4/Mapa_01_A4_16.png"
#define S4_17 "assets/img/S4/Mapa_01_A4_17.png"

S4::S4() {
    AddScenario(S4_0, LayerDistance::_Background_FarAway);
    AddScenario(S4_2, LayerDistance::_Background, 0.45);
    AddScenario(S4_3, LayerDistance::_Background_Close, 0.5);
    AddScenario(S4_4, LayerDistance::_Background_VeryClose, 0.55);
    AddScenario(S4_5, LayerDistance::_Scenery_Far, 0.6);
    AddScenario(S4_6, LayerDistance::_Scenery, 0.7);
    AddScenario(S4_7, LayerDistance::_Scenery_Close, 0.8);
    AddScenario(S4_8, LayerDistance::_Environment_Far, 0.95);
    AddScenario(S4_9, LayerDistance::_Environment);
    AddScenario(S4_16, LayerDistance::_ForeGround);
    AddScenario(S4_17, LayerDistance::_ForeGround_Close, 1.15);

    AddAnimated(TREE_0B_, LayerDistance::_Environment_Far,Vec2(-2,9), 15);
    AddAnimated(TREE_0A_, LayerDistance::_Environment_Far,Vec2(43,9), 15);

    AddAnimated(TREE_1B_, LayerDistance::_Environment_Far,Vec2(18,10), 15);
    AddAnimated(TREE_1D_, LayerDistance::_Environment_Far,Vec2(40,10), 15);


}

void S4::LoadAssets() {
    stateMusic = ZoneManager::GetCarriedMusic();
    if(stateMusic == nullptr) {
        stateMusic = new Music(MUSIC_DIRT);
        stateMusic->Play(-1, STATE_FADE_TIME*1000);
    }
    
    GameObject* TileObj = new GameObject(LayerDistance::_Background_Far);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE, TILE_SIZE);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_S4,1,true);
    tile->LoadCollision(MAP_S4);
    zoneSize = Vec2(tile->GetWidth()*TILE_SIZE, tile->GetHeight()*TILE_SIZE);
    TileObj->AddComponent(tile);
    AddObject(TileObj);

    AddKid();

    //Path to S3-B
    GameObject *AObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorA = new ZoneTransition(*AObj, Rect(13, 0, 1, 19), 
        std::make_pair(Zone::_S4, ZoneExit::A));
    AObj->AddComponent(doorA);
    AddObject(AObj);
    
    //Path to U16-A
    GameObject *BObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorB = new ZoneTransition(*BObj, Rect(39, 28, 2, 1), 
        std::make_pair(Zone::_S4, ZoneExit::B), false);
    BObj->AddComponent(doorB);
    AddObject(BObj);

    GameObject* checkObj = new GameObject(LayerDistance::_Environment);
    Sprite* checkPoint = new Sprite(*checkObj, CHECKPOINT_ANIM, 6, 0.1);
    checkObj->box.SetPurePosition(33.96*TILE_SIZE, 24.08*TILE_SIZE);
    checkObj->box.SetSize(checkPoint->GetWidth(), checkPoint->GetHeight());
    Interactor* checkSave = new Interactor(*checkObj);
    checkSave->SetResult([](){
        GameData::checkPoint = Zone::_S4;
        GameData::revivePosition = Vec2(33,25);
    });
    checkObj->AddComponent(checkPoint);
    checkObj->AddComponent(checkSave);
    AddObject(checkObj);

    LimitMap();
    FadeIn();
}

void S4::Update(float dt) {
}














//################################################
// Underground zones
//################################################
#define U1_FUNDO "assets/img/U1/0_fundo.png"
#define U1_2 "assets/img/U1/2.png"
#define U1_4 "assets/img/U1/4.png"
#define U1_6 "assets/img/U1/6.png"
#define U1_7 "assets/img/U1/7.png"
#define U1_8 "assets/img/U1/8.png"
#define U1_9_BASE "assets/img/U1/9_base.png"
#define U1_16 "assets/img/U1/16.png"
#define U1_17 "assets/img/U1/17.png"
#define U1_17_2 "assets/img/U1/17-2.png"
    

U1::U1() {
    AddScenario(U1_FUNDO, LayerDistance::_Background_Far);
    AddScenario(U1_2, LayerDistance::_Background);
    AddScenario(U1_4, LayerDistance::_Background_VeryClose);
    AddScenario(U1_6, LayerDistance::_Scenery);
    AddScenario(U1_7, LayerDistance::_Scenery_Close);
    AddScenario(U1_8, LayerDistance::_Environment_Far);
    AddScenario(U1_9_BASE, LayerDistance::_Environment);
    AddScenario(U1_16, LayerDistance::_ForeGround);
    AddScenario(U1_17, LayerDistance::_ForeGround_Close);
    AddScenario(U1_17_2, LayerDistance::_ForeGround_Close);
}
#define MAP_U1 "assets/map/Zone/U1"
void U1::LoadAssets() {
    //#####################################
    stateMusic = ZoneManager::GetCarriedMusic();
    if(stateMusic == nullptr) {
        stateMusic = new Music(MUSIC_CROSS);
        stateMusic->Play(-1, STATE_FADE_TIME*1000);
    }

    GameObject* TileObj = new GameObject(LayerDistance::_Background_Far);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE,TILE_SIZE);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_U1,1,true);
    tile->LoadCollision(MAP_U1);
    zoneSize = Vec2(tile->GetWidth()*TILE_SIZE, tile->GetHeight()*TILE_SIZE);
    TileObj->AddComponent(tile);
    AddObject(TileObj);

    AddKid();

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

    AddSpikes(26,42,5);

    GameObject* checkObj = new GameObject(LayerDistance::_Environment_Far);
    Sprite* checkPoint = new Sprite(*checkObj, CHECKPOINT_ANIM, 6, 0.1);
    checkObj->box.SetPurePosition(52.4*TILE_SIZE, 39.1*TILE_SIZE);
    checkObj->box.SetSize(checkPoint->GetWidth(), checkPoint->GetHeight());
    Interactor* checkSave = new Interactor(*checkObj);
    checkSave->SetResult([](){
        GameData::checkPoint = Zone::_U1;
        GameData::revivePosition = Vec2(50,40);
    });
    checkObj->AddComponent(checkPoint);
    checkObj->AddComponent(checkSave);
    AddObject(checkObj);

    if(!GameData::canUseMana) {
        GameObject* intObj = new GameObject(LayerDistance::_Environment_Close);
        intObj->box = Rect(0,0,TILE_SIZE,TILE_SIZE);
        intObj->box.SetPurePosition(59*TILE_SIZE,41*TILE_SIZE);
        Interactor* output = new Interactor(*intObj, true);
        output->SetCondition([](){return InputManager::GetInstance().KeyPress(SDLK_z);});
        
        output->SetResult([](){
            GameData::canUseMana = true;
        });

        intObj->AddComponent(output);
        intObj->AddComponent(new Sprite(*intObj,FIRE, 30, 0.1));
        AddObject(intObj);
    }

    LimitMap();
    FadeIn();
}

void U1::Update(float dt) {

}














//################################################
#define U2_FUNDO    "assets/img/U2/0_fundo.png"
#define U2_2        "assets/img/U2/2.png"
#define U2_4        "assets/img/U2/4.png"
#define U2_6        "assets/img/U2/6.png"
#define U2_7        "assets/img/U2/7.png"
#define U2_8        "assets/img/U2/8.png"
#define U2_9_BASE   "assets/img/U2/9_base.png"
#define U2_16       "assets/img/U2/16.png"
#define U2_17       "assets/img/U2/17.png"
#define U2_17_2     "assets/img/U2/17-2.png"

U2::U2() {
    AddScenario(U2_FUNDO, LayerDistance::_Background_FarAway);
    AddScenario(U2_2, LayerDistance::_Background);
    AddScenario(U2_4, LayerDistance::_Background_VeryClose);
    AddScenario(U2_6, LayerDistance::_Scenery);
    AddScenario(U2_7, LayerDistance::_Scenery_Close);
    AddScenario(U2_8, LayerDistance::_Environment_Far);
    AddScenario(U2_9_BASE, LayerDistance::_Environment);
    AddScenario(U2_16, LayerDistance::_ForeGround);
    AddScenario(U2_17, LayerDistance::_ForeGround_Close);
    AddScenario(U2_17_2, LayerDistance::_ForeGround_Close);
}
#define MAP_U2 "assets/map/Zone/U2"
void U2::LoadAssets() {
    //#####################################
    stateMusic = ZoneManager::GetCarriedMusic();
    if(stateMusic == nullptr) {
        stateMusic = new Music(MUSIC_CROSS);
        stateMusic->Play(-1, STATE_FADE_TIME*1000);
    }

    GameObject* TileObj = new GameObject(LayerDistance::_Background_Far);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE,TILE_SIZE);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_U2,1,true);
    tile->LoadCollision(MAP_U2);
    zoneSize = Vec2(tile->GetWidth()*TILE_SIZE, tile->GetHeight()*TILE_SIZE);
    TileObj->AddComponent(tile);
    AddObject(TileObj);

    AddKid();

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

    GameObject* mouseGO = new GameObject(LayerDistance::_NPC);
    EnemyArmadillo* mouseCP = new EnemyArmadillo(*mouseGO);
    mouseGO->AddComponent(mouseCP);
    mouseGO->box.SetPosition(396,1090);
    AddObject(mouseGO);

    LimitMap();
    FadeIn();
}

void U2::Update(float dt) {

}














//################################################
#define U3_FUNDO    "assets/img/U3/0_fundo.png"
#define U3_4        "assets/img/U3/4.png"
#define U3_5        "assets/img/U3/5.png"
#define U3_6        "assets/img/U3/6.png"
#define U3_7        "assets/img/U3/7.png"
#define U3_7_1      "assets/img/U3/7-1.png"
#define U3_8        "assets/img/U3/8.png"
#define U3_9_BASE   "assets/img/U3/9_base.png"
#define U3_16       "assets/img/U3/16.png"
#define U3_17       "assets/img/U3/17.png"
#define U3_17_2     "assets/img/U3/17-2.png"
#define U3_17_3     "assets/img/U3/17-3.png"

U3::U3() {
    AddScenario(U3_FUNDO, LayerDistance::_Background_Far);
    AddScenario(U3_4, LayerDistance::_Background_VeryClose, 0.5, true);
    AddScenario(U3_5, LayerDistance::_Scenery_Far, 0.6, true);
    AddScenario(U3_6, LayerDistance::_Scenery, 0.7, true);
    AddScenario(U3_7, LayerDistance::_Scenery_Close);
    AddScenario(U3_7_1, LayerDistance::_Scenery_Close);
    AddScenario(U3_8, LayerDistance::_Environment_Far);
    AddScenario(U3_9_BASE, LayerDistance::_Environment);
    AddScenario(U3_16, LayerDistance::_ForeGround);
    AddScenario(U3_17, LayerDistance::_ForeGround_Close, 1.15, true);
    AddScenario(U3_17_2, LayerDistance::_ForeGround_Close, 1.15);
    AddScenario(U3_17_3, LayerDistance::_ForeGround_Close, 1.15, true);

    // //barriers
    // GameObject* BgObj = new GameObject(LayerDistance::_ForeGround_VeryClose);
    // Sprite* Bg1 = new Sprite(*BgObj,OUTER);
    // BgObj->AddComponent(Bg1);
    // AddObject(BgObj);
    // BgObj->box.SetSize(Bg1->GetWidth(), Bg1->GetHeight());
    // BgObj->box.SetPurePosition(0-Bg1->GetWidth()+12,0);
    
    // BgObj = new GameObject(LayerDistance::_ForeGround_VeryClose);
    // Bg1 = new Sprite(*BgObj,OUTER);
    // BgObj->AddComponent(Bg1);
    // AddObject(BgObj);
    // BgObj->box.SetSize(Bg1->GetWidth(), Bg1->GetHeight());
    // BgObj->box.SetPurePosition(2688,0);

}
#define MAP_U3 "assets/map/Zone/U3"

void U3::LoadAssets() {
    //#####################################
    stateMusic = ZoneManager::GetCarriedMusic();
    if(stateMusic == nullptr) {
        stateMusic = new Music(MUSIC_CROSS);
        stateMusic->Play(-1, STATE_FADE_TIME*1000);
    }

    GameObject* TileObj = new GameObject(LayerDistance::_Background_Far);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE,TILE_SIZE);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_U3,1,true);
    tile->LoadCollision(MAP_U3);
    zoneSize = Vec2(tile->GetWidth()*TILE_SIZE, tile->GetHeight()*TILE_SIZE);
    TileObj->AddComponent(tile);
    AddObject(TileObj);

    AddKid();

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

    AddSpikes(14,22,69);

    // GameObject* infectedGO = new GameObject(LayerDistance::_NPC);
    // EnemyRunner* infectedCP = new EnemyRunner(*infectedGO);
    // infectedGO->AddComponent(infectedCP);
    // infectedGO->box.SetPosition(1622,466);
    // AddObject(infectedGO);

    // infectedGO = new GameObject(LayerDistance::_NPC);
    // infectedCP = new EnemyRunner(*infectedGO);
    // infectedGO->AddComponent(infectedCP);
    // infectedGO->box.SetPosition(1945,592);
    // AddObject(infectedGO);

    // infectedGO = new GameObject(LayerDistance::_NPC);
    // infectedCP = new EnemyRunner(*infectedGO);
    // infectedGO->AddComponent(infectedCP);
    // infectedGO->box.SetPosition(1081,605);
    // AddObject(infectedGO);

    LimitMap();
    FadeIn();
}

void U3::Update(float dt) {

}














//################################################
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
    AddScenario(U4_FUNDO, LayerDistance::_Background_Far);
    AddScenario(U4_2, LayerDistance::_Background, 0.5, true);
    AddScenario(U4_5, LayerDistance::_Scenery_Far, 0.7, true);
    AddScenario(U4_5_2, LayerDistance::_Scenery_Far, 0.7, true);
    AddScenario(U4_6, LayerDistance::_Scenery, 0.8, true);
    AddScenario(U4_7, LayerDistance::_Scenery_Close, 0.9, true);
    AddScenario(U4_7_BACK, LayerDistance::_Scenery_Close, 0.9, true);
    AddScenario(U4_8_TREE_BODY, LayerDistance::_Environment_Far);
    AddScenario(U4_9_BASE, LayerDistance::_Environment);
    AddScenario(U4_16, LayerDistance::_ForeGround);
    AddScenario(U4_17, LayerDistance::_ForeGround_Close, 1.15, true);
    AddScenario(U4_17_2, LayerDistance::_ForeGround_Close, 1.15);
}

#define MAP_U4 "assets/map/Zone/U4"
void U4::LoadAssets() {
    //#####################################
    stateMusic = ZoneManager::GetCarriedMusic();
    if(stateMusic == nullptr) {
        stateMusic = new Music(MUSIC_CROSS);
        stateMusic->Play(-1, STATE_FADE_TIME*1000);
    }

    GameObject* TileObj = new GameObject(LayerDistance::_Background_Far);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE,TILE_SIZE);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_U4,1,true);
    tile->LoadCollision(MAP_U4);
    zoneSize = Vec2(tile->GetWidth()*TILE_SIZE, tile->GetHeight()*TILE_SIZE);
    TileObj->AddComponent(tile);
    AddObject(TileObj);

    AddKid();

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

    //Interaction
    if(!GameData::firstItem) {
        GameObject* intObj = new GameObject(LayerDistance::_Environment_Close);
        intObj->box = Rect(0,0,TILE_SIZE,TILE_SIZE);
        intObj->box.SetPurePosition(27.9*TILE_SIZE,43*TILE_SIZE);
        Interactor* output = new Interactor(*intObj, true);
        output->SetCondition([](){return InputManager::GetInstance().KeyPress(SDLK_z);});
        output->SetResult([](){GameData::firstItem = true;});
        intObj->AddComponent(output);
        intObj->AddComponent(new Sprite(*intObj,POTION_ITEM, 30, 0.1));
        AddObject(intObj);
    }

    if(!GameData::canUseChargedAttack) {
        GameObject* intObj = new GameObject(LayerDistance::_Environment_Close);
        intObj->box = Rect(0,0,TILE_SIZE,TILE_SIZE);
        intObj->box.SetPurePosition(46*TILE_SIZE,29*TILE_SIZE);
        Interactor* output = new Interactor(*intObj, true);
        output->SetCondition([](){return InputManager::GetInstance().KeyPress(SDLK_z);});
        
        output->SetResult([](){
            GameData::canUseChargedAttack = true;
        });

        intObj->AddComponent(output);
        intObj->AddComponent(new Sprite(*intObj,FIRE, 30, 0.1));
        AddObject(intObj);
    }

    GameObject* mouseGO = new GameObject(LayerDistance::_NPC);
    EnemyArmadillo* mouseCP = new EnemyArmadillo(*mouseGO);
    mouseGO->AddComponent(mouseCP);
    mouseGO->box.SetPosition(1195,811);
    AddObject(mouseGO);

    LimitMap();
    FadeIn();
}

void U4::Update(float dt) {}














//################################################
#define U5_0 "assets/img/U5/0_fundo.png"
#define U5_2 "assets/img/U5/2.png"
#define U5_4 "assets/img/U5/4.png"
#define U5_6 "assets/img/U5/6.png"
#define U5_7 "assets/img/U5/7.png"
#define U5_8 "assets/img/U5/8.png"
#define U5_9 "assets/img/U5/9_base.png"
#define U5_16 "assets/img/U5/16.png"
#define U5_17 "assets/img/U5/17.png"
#define U5_17_2 "assets/img/U5/17-2.png"

#define STONE1 "assets/img/U5/10_pedra_M.png"
#define STONE2 "assets/img/U5/10_pedra_M-2.png"

#define MECHA1 "assets/img/animated/mecanismo1_84x84-8f.png"
#define MECHA2 "assets/img/animated/mecanismo2_84x84-8f.png"



U5::U5() {
    AddScenario(U5_0, LayerDistance::_Background_FarAway);
    AddScenario(U5_2, LayerDistance::_Background);
    AddScenario(U5_4, LayerDistance::_Background_VeryClose);
    AddScenario(U5_6, LayerDistance::_Scenery);
    AddScenario(U5_7, LayerDistance::_Scenery_Close);
    AddScenario(U5_8, LayerDistance::_Environment_Far);
    AddScenario(U5_9, LayerDistance::_Environment);
    AddScenario(U5_16, LayerDistance::_ForeGround);
    AddScenario(U5_17, LayerDistance::_ForeGround_Close);
    AddScenario(U5_17_2, LayerDistance::_ForeGround_Close);
}

#define MAP_U5 "assets/map/Zone/U5"
void U5::LoadAssets() {
    //#####################################
    stateMusic = ZoneManager::GetCarriedMusic();
    if(stateMusic == nullptr) {
        stateMusic = new Music(MUSIC_CROSS);
        stateMusic->Play(-1, STATE_FADE_TIME*1000);
    }

    GameObject* TileObj = new GameObject(LayerDistance::_Background_Far);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE,TILE_SIZE);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_U5,1,true);
    tile->LoadCollision(MAP_U5);
    zoneSize = Vec2(tile->GetWidth()*TILE_SIZE, tile->GetHeight()*TILE_SIZE);
    TileObj->AddComponent(tile);
    AddObject(TileObj);

    AddKid();

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

    AddSpikes(24,23,4);
    AddSpikes(32,25,3);

    //Boulders
    if(!GameData::boulder1){
        GameObject* boulderObj = new GameObject(LayerDistance::_Environment);
        boulderObj->AddComponent(new Boulder(*boulderObj, STONE1, Vec2(21,8)));
        AddObject(boulderObj);
        GameObject* intObj = new GameObject(LayerDistance::_Environment_Close);
        intObj->box = Rect(0,0,TILE_SIZE,TILE_SIZE);
        intObj->box.SetPurePosition(23.5*TILE_SIZE,15.8*TILE_SIZE);
        Interactor* output = new Interactor(*intObj, true, true, ComponentType::_Boulder);
        output->SetResult([](){GameData::boulder1 = true;});
        intObj->AddComponent(output);
        AddObject(intObj);

    } else {
        GameObject* boulderObj = new GameObject(LayerDistance::_Environment);
        Sprite* boulderPic = new Sprite(*boulderObj, STONE1);
        boulderObj->AddComponent(boulderPic);
        boulderObj->box.SetSize(boulderPic->GetWidth(), boulderPic->GetHeight());
        boulderObj->box.SetPurePosition(23*TILE_SIZE,13*TILE_SIZE);
        boulderObj->AddComponent(new Collider(*boulderObj));
        AddObject(boulderObj);
    }

    if(!GameData::boulder2){
        GameObject* boulderObj = new GameObject(LayerDistance::_Environment);
        boulderObj->AddComponent(new Boulder(*boulderObj, STONE2, Vec2(38,22)));
        AddObject(boulderObj);
        GameObject* intObj = new GameObject(LayerDistance::_Environment_Close);
        intObj->box = Rect(0,0,TILE_SIZE,TILE_SIZE);
        intObj->box.SetPurePosition(41*TILE_SIZE,24*TILE_SIZE);
        Interactor* output = new Interactor(*intObj, true, true, ComponentType::_Boulder);
        output->SetResult([](){GameData::boulder2 = true;});
        intObj->AddComponent(output);
        AddObject(intObj);
    } else {
        GameObject* boulderObj = new GameObject(LayerDistance::_Environment);
        Sprite* boulderPic = new Sprite(*boulderObj, STONE2);
        boulderObj->AddComponent(boulderPic);
        boulderObj->box.SetSize(boulderPic->GetWidth(), boulderPic->GetHeight());
        boulderObj->box.SetPurePosition(41*TILE_SIZE,23*TILE_SIZE);
        boulderObj->AddComponent(new Collider(*boulderObj));
        AddObject(boulderObj);
    }
    
    //Setup Mechanism
    GameObject* mechaObj = new GameObject(LayerDistance::_Environment_Close);
    if(!GameData::firstMechanism){
        Sprite* mechanism = new Sprite(*mechaObj, MECHA2, 8);
        mechanism->SetFrame(0);
        mechaObj->box.SetSize(mechanism->GetWidth(), mechanism->GetHeight());
        mechaObj->box.SetPurePosition(43*TILE_SIZE, 20*TILE_SIZE);

        Interactor* mechaEnabler = new Interactor(*mechaObj, true);
        mechaEnabler->SetCondition([](){return (InputManager::GetInstance().KeyPress(SDLK_z));});
        mechaEnabler->SetResult([](){
            GameObject* mechaObj = new GameObject(LayerDistance::_Environment_Close);
            Sprite* mechanism = new Sprite(*mechaObj, MECHA2, 8, 0.1, true);
            mechaObj->box.SetSize(mechanism->GetWidth(), mechanism->GetHeight());
            mechaObj->box.SetPurePosition(43*TILE_SIZE, 20*TILE_SIZE);
           
            mechaObj->AddComponent(mechanism);
            Game::GetInstance().GetCurrentState().AddObject(mechaObj);

            GameData::firstMechanism = true;
        });
        mechaObj->AddComponent(mechanism);
        mechaObj->AddComponent(mechaEnabler);
    } else {
        Sprite* mechanism = new Sprite(*mechaObj, MECHA2, 8);
        mechaObj->box.SetSize(mechanism->GetWidth(), mechanism->GetHeight());
        mechaObj->box.SetPurePosition(43*TILE_SIZE, 20*TILE_SIZE);
        mechanism->SetFrame(7);
        mechaObj->AddComponent(mechanism);
    }
    AddObject(mechaObj);

    GameObject* infectedGO = new GameObject(LayerDistance::_NPC);
    EnemyRunner* infectedCP = new EnemyRunner(*infectedGO);
    infectedGO->AddComponent(infectedCP);
    infectedGO->box.SetPosition(1092,388);
    AddObject(infectedGO);

    LimitMap();
    FadeIn();
}

void U5::Update(float dt) {}














//################################################
#define U6_FUNDO "assets/img/U6/0_fundo.png"
#define U6_2 "assets/img/U6/2.png"
#define U6_5 "assets/img/U6/5.png"
#define U6_6 "assets/img/U6/6.png"
#define U6_8 "assets/img/U6/8.png"
#define U6_9_BASE "assets/img/U6/9_base.png"
#define U6_16 "assets/img/U6/16.png"
#define U6_17 "assets/img/U6/17.png"
#define U6_17_2 "assets/img/U6/17-2.png"
    
U6::U6() {
    AddScenario(U6_FUNDO, LayerDistance::_Background_Far);
    AddScenario(U6_2, LayerDistance::_Background);
    AddScenario(U6_5, LayerDistance::_Scenery_Far);
    AddScenario(U6_6, LayerDistance::_Scenery);
    AddScenario(U6_8, LayerDistance::_Environment_Far);
    AddScenario(U6_9_BASE, LayerDistance::_Environment);
    AddScenario(U6_16, LayerDistance::_ForeGround);
    AddScenario(U6_17, LayerDistance::_ForeGround_Close);
    AddScenario(U6_17_2, LayerDistance::_ForeGround_Close);
}
#define MAP_U6 "assets/map/Zone/U6"
void U6::LoadAssets() {
    //#####################################
    stateMusic = ZoneManager::GetCarriedMusic();
    if(stateMusic == nullptr) {
        stateMusic = new Music(MUSIC_CROSS);
        stateMusic->Play(-1, STATE_FADE_TIME*1000);
    }

    GameObject* TileObj = new GameObject(LayerDistance::_Background_Far);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE,TILE_SIZE);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_U6,1,true);
    tile->LoadCollision(MAP_U6);
    zoneSize = Vec2(tile->GetWidth()*TILE_SIZE, tile->GetHeight()*TILE_SIZE);
    TileObj->AddComponent(tile);
    AddObject(TileObj);

    AddKid();

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

    AddSpikes(20,21,7);
    AddSpikes(36,21,13);

    GameObject* mouseGO = new GameObject(LayerDistance::_NPC);
    EnemyArmadillo* mouseCP = new EnemyArmadillo(*mouseGO);
    mouseGO->AddComponent(mouseCP);
    mouseGO->box.SetPosition(680,546);
    AddObject(mouseGO);

    GameObject* infectedGO = new GameObject(LayerDistance::_NPC);
    EnemyRunner* infectedCP = new EnemyRunner(*infectedGO);
    infectedGO->AddComponent(infectedCP);
    infectedGO->box.SetPosition(953,545);
    AddObject(infectedGO);
    
    LimitMap();
    FadeIn();
}

void U6::Update(float dt) {}














//################################################
#define U7_0 "assets/img/U7/Mapa_02_A7_0.png"
#define U7_6 "assets/img/U7/Mapa_02_A7_6.png"
#define U7_8 "assets/img/U7/Mapa_02_A7_8.png"
#define U7_9 "assets/img/U7/Mapa_02_A7_9.png"
#define U7_16 "assets/img/U7/Mapa_02_A7_16.png"
#define U7_17 "assets/img/U7/Mapa_02_A7_17.png"

U7::U7() {
    AddScenario(U7_0, LayerDistance::_Background_FarAway);
    AddScenario(U7_6, LayerDistance::_Scenery);
    AddScenario(U7_8, LayerDistance::_Environment_Far);
    AddScenario(U7_9, LayerDistance::_Environment);
    AddScenario(U7_16, LayerDistance::_ForeGround);
    AddScenario(U7_17, LayerDistance::_ForeGround_Close);
}
#define MAP_U7 "assets/map/Zone/U7"
void U7::LoadAssets() {
    //#####################################
    stateMusic = ZoneManager::GetCarriedMusic();
    if(stateMusic == nullptr) {
        stateMusic = new Music(MUSIC_CROSS);
        stateMusic->Play(-1, STATE_FADE_TIME*1000);
    }

    GameObject* TileObj = new GameObject(LayerDistance::_Background_Far);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE,TILE_SIZE);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_U7,1,true);
    tile->LoadCollision(MAP_U7);
    zoneSize = Vec2(tile->GetWidth()*TILE_SIZE, tile->GetHeight()*TILE_SIZE);
    TileObj->AddComponent(tile);
    AddObject(TileObj);

    AddKid();

    //Path to U6-B
    GameObject *AObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorA = new ZoneTransition(*AObj, Rect(11, 15, 1, 3), 
        std::make_pair(Zone::_U7, ZoneExit::A));
    AObj->AddComponent(doorA);
    AddObject(AObj);

    //Path to U16-B
    GameObject *BObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorB = new ZoneTransition(*BObj, Rect(53, 15, 1, 6), 
        std::make_pair(Zone::_U7, ZoneExit::B));
    BObj->AddComponent(doorB);
    AddObject(BObj);

    //Path to U9-D
    GameObject *CObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorC = new ZoneTransition(*CObj, Rect(11, 29, 1, 2), 
        std::make_pair(Zone::_U7, ZoneExit::C));
    CObj->AddComponent(doorC);
    AddObject(CObj);

    AddSpikes(21,35,6);

    //Setup gate
    GameObject* gateObj = new GameObject(LayerDistance::_Environment_Close);
    if(!GameData::openedU16Gate){
        Sprite* gateIndication = new Sprite(*gateObj, GATE_COUNTING, 5);
        gateIndication->SetFrame(GameData::itemCount);
        gateObj->box.SetSize(gateIndication->GetWidth(), gateIndication->GetHeight());
        gateObj->box.SetPurePosition(50.85*TILE_SIZE, 13.7*TILE_SIZE);
        Interactor* gateOpener = new Interactor(*gateObj,(GameData::itemCount >= 4), false);
        gateOpener->SetCondition([](){return (GameData::itemCount >= 4 and GameData::mechanismCount >= 3);});
        gateOpener->SetResult([](){
            GameObject* gateObj = new GameObject(LayerDistance::_Environment_Close);
            Sprite* gate = new Sprite(*gateObj, GATE_OPENING, 26, 0.1, true);
            gateObj->box.SetSize(gate->GetWidth(), gate->GetHeight());
            gateObj->box.SetPurePosition(50.85*TILE_SIZE, 13.7*TILE_SIZE);
            Collider* gateLimit = new Collider(*gateObj);
            gateLimit->offset = Vec2(0, -84);
            gateObj->AddComponent(gate);
            gateObj->AddComponent(gateLimit);
            Game::GetInstance().GetCurrentState().AddObject(gateObj);
            GameData::openedU16Gate = true;
        });
        gateObj->AddComponent(gateIndication);
        gateObj->AddComponent(gateOpener);
        
    } else {
        Sprite* gate = new Sprite(*gateObj, GATE_OPENING, 26);
        gateObj->box.SetSize(gate->GetWidth(), gate->GetHeight());
        gateObj->box.SetPurePosition(50.85*TILE_SIZE, 13.7*TILE_SIZE);
        gate->SetFrame(25);
        Collider* gateLimit = new Collider(*gateObj);
        gateLimit->offset = Vec2(0, -84);
        gateObj->AddComponent(gate);
        gateObj->AddComponent(gateLimit);
    }
    AddObject(gateObj);

    GameObject* mouseGO = new GameObject(LayerDistance::_NPC);
    EnemyArmadillo* mouseCP = new EnemyArmadillo(*mouseGO);
    mouseGO->AddComponent(mouseCP);
    mouseGO->box.SetPosition(571,644);
    AddObject(mouseGO);

    LimitMap();
    FadeIn();
}

void U7::Update(float dt) {

    // if(InputManager::GetInstance().KeyPress(SDLK_z)) { //for debugging door
    //     if(GameData::firstItem == false)
    //     {
    //         GameData::firstItem = true;
    //     }
    //     else if(GameData::secondItem == false)
    //     {
    //         GameData::secondItem = true;
    //     }
    //     else if(GameData::thirdItem == false)
    //     {
    //         GameData::thirdItem = true;
    //     }
    //     else if(GameData::fourthItem == false)
    //     {
    //         GameData::fourthItem = true;
    //     }
    // }

}














//################################################
#define U8_FUNDO "assets/img/U8/0_fundo.png"
#define U8_2 "assets/img/U8/2.png"
#define U8_4 "assets/img/U8/4.png"
#define U8_6 "assets/img/U8/6.png"
#define U8_8 "assets/img/U8/8.png"
#define U8_9_BASE "assets/img/U8/9_base.png"
#define U8_16 "assets/img/U8/16.png"
#define U8_17 "assets/img/U8/17.png"


U8::U8() {
    AddScenario(U8_FUNDO, LayerDistance::_Background_Far);
    AddScenario(U8_4, LayerDistance::_Background_VeryClose);
    AddScenario(U8_6, LayerDistance::_Scenery);
    AddScenario(U8_8, LayerDistance::_Environment_Far);
    AddScenario(U8_9_BASE, LayerDistance::_Environment);
    AddScenario(U8_16, LayerDistance::_ForeGround);
    AddScenario(U8_17, LayerDistance::_ForeGround_Close);
}
#define MAP_U8 "assets/map/Zone/U8"
void U8::LoadAssets() {
    //#####################################
    stateMusic = ZoneManager::GetCarriedMusic();
    if(stateMusic == nullptr) {
        stateMusic = new Music(MUSIC_CROSS);
        stateMusic->Play(-1, STATE_FADE_TIME*1000);
    }

    GameObject* TileObj = new GameObject(LayerDistance::_Background_Far);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE,TILE_SIZE);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_U8,1,true);
    tile->LoadCollision(MAP_U8);
    zoneSize = Vec2(tile->GetWidth()*TILE_SIZE, tile->GetHeight()*TILE_SIZE);
    TileObj->AddComponent(tile);
    AddObject(TileObj);

    AddKid();

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

    GameObject* checkObj = new GameObject(LayerDistance::_Environment_Far);
    Sprite* checkPoint = new Sprite(*checkObj, CHECKPOINT_ANIM, 6, 0.1);
    checkObj->box.SetPurePosition(12.96*TILE_SIZE, 17.1*TILE_SIZE);
    checkObj->box.SetSize(checkPoint->GetWidth(), checkPoint->GetHeight());
    Interactor* checkSave = new Interactor(*checkObj);
    checkSave->SetResult([](){
        GameData::checkPoint = Zone::_U8;
        GameData::revivePosition = Vec2(12,18);
    });
    checkObj->AddComponent(checkPoint);
    checkObj->AddComponent(checkSave);
    AddObject(checkObj);

    LimitMap();
    FadeIn();
}

void U8::Update(float dt) {

}














//################################################
#define U9_FUNDO    "assets/img/U9/0_fundo.png"
#define U9_2        "assets/img/U9/2.png"
#define U9_4        "assets/img/U9/4.png"
#define U9_7        "assets/img/U9/7.png"
#define U9_8        "assets/img/U9/8.png"
#define U9_9_BASE   "assets/img/U9/9_base.png"
#define U9_16       "assets/img/U9/16.png"
#define U9_17       "assets/img/U9/17.png"

U9::U9() {
    AddScenario(U9_FUNDO, LayerDistance::_Background_FarAway);
    AddScenario(U9_2, LayerDistance::_Background);
    AddScenario(U9_4, LayerDistance::_Background_VeryClose);
    AddScenario(U9_7, LayerDistance::_Scenery_Close);
    AddScenario(U9_8, LayerDistance::_Environment_Far);
    AddScenario(U9_9_BASE, LayerDistance::_Environment);
    AddScenario(U9_16, LayerDistance::_ForeGround);
    AddScenario(U9_17, LayerDistance::_ForeGround_Close);
}
#define MAP_U9 "assets/map/Zone/U9"
void U9::LoadAssets() {
    //#####################################
    stateMusic = ZoneManager::GetCarriedMusic();
    if(stateMusic == nullptr) {
        stateMusic = new Music(MUSIC_CROSS);
        stateMusic->Play(-1, STATE_FADE_TIME*1000);
    }

    GameObject* TileObj = new GameObject(LayerDistance::_Background_Far);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE,TILE_SIZE);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_U9,1,true);
    tile->LoadCollision(MAP_U9);
    zoneSize = Vec2(tile->GetWidth()*TILE_SIZE, tile->GetHeight()*TILE_SIZE);
    TileObj->AddComponent(tile);
    AddObject(TileObj);

    AddKid();

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

    AddSpikes(13,32,5);

    LimitMap();
    FadeIn();
}

void U9::Update(float dt) {

}














//################################################
#define U10_0 "assets/img/U10/Mapa_02_A10_0.png"
#define U10_4 "assets/img/U10/Mapa_02_A10_4.png"
#define U10_5 "assets/img/U10/Mapa_02_A10_5.png"
#define U10_6 "assets/img/U10/Mapa_02_A10_6.png"
#define U10_7 "assets/img/U10/Mapa_02_A10_7.png"
#define U10_8 "assets/img/U10/Mapa_02_A10_8.png"
#define U10_9 "assets/img/U10/Mapa_02_A10_9.png"
#define U10_16 "assets/img/U10/Mapa_02_A10_16.png"
#define U10_17 "assets/img/U10/Mapa_02_A10_17.png"

#define U10_STATUE_ANIM "assets/img/animated/estatua-Sheet.png"
#define U10_CACHOEIRA_ANIM "assets/img/U10/mapa_A10_7_cachoeira-Sheet.png"

U10::U10() {
    AddScenario(U10_0, LayerDistance::_Background_FarAway);
    AddScenario(U10_4, LayerDistance::_Background_VeryClose);
    AddScenario(U10_5, LayerDistance::_Scenery_Far);
    AddScenario(U10_6, LayerDistance::_Scenery);
    AddScenario(U10_7, LayerDistance::_Scenery_Close);
    AddScenario(U10_8, LayerDistance::_Environment_Far);
    AddScenario(U10_9, LayerDistance::_Environment);
    AddScenario(U10_16, LayerDistance::_ForeGround);
    AddScenario(U10_17, LayerDistance::_ForeGround_Close);

    AddAnimated(U10_STATUE_ANIM, LayerDistance::_Environment_Far,Vec2(40.25,17),4);
    AddAnimated(U10_STATUE_ANIM, LayerDistance::_Environment_Far,Vec2(47.25,16.7),4);
    AddAnimated(U10_STATUE_ANIM, LayerDistance::_Environment_Far,Vec2(54.25,17),4);
    AddAnimated(U10_CACHOEIRA_ANIM, LayerDistance::_Scenery_Close,Vec2(70.75,3),10);
}
#define MAP_U10 "assets/map/Zone/U10"
void U10::LoadAssets() {
    //#####################################
    stateMusic = ZoneManager::GetCarriedMusic();
    if(stateMusic == nullptr) {
        stateMusic = new Music(MUSIC_CROSS);
        stateMusic->Play(-1, STATE_FADE_TIME*1000);
    }

    GameObject* TileObj = new GameObject(LayerDistance::_Background_Far);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE,TILE_SIZE);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_U10,1,true);
    tile->LoadCollision(MAP_U10);
    zoneSize = Vec2(tile->GetWidth()*TILE_SIZE, tile->GetHeight()*TILE_SIZE);
    TileObj->AddComponent(tile);
    AddObject(TileObj);

    AddKid();

    //Path to U9-C
    GameObject *AObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorA = new ZoneTransition(*AObj, Rect(7, 17, 1, 2), 
        std::make_pair(Zone::_U10, ZoneExit::A));
    AObj->AddComponent(doorA);
    AddObject(AObj);

    if(!GameData::secondItem) {
        GameObject* intObj = new GameObject(LayerDistance::_Environment_Close);
        intObj->box = Rect(0,0,TILE_SIZE,TILE_SIZE);
        intObj->box.SetPurePosition(73.9*TILE_SIZE,16.5*TILE_SIZE);
        Interactor* output = new Interactor(*intObj, true);
        output->SetCondition([](){return InputManager::GetInstance().KeyPress(SDLK_z);});
        output->SetResult([](){GameData::secondItem = true;});
        intObj->AddComponent(output);
        intObj->AddComponent(new Sprite(*intObj,POTION_ITEM, 30, 0.1));
        AddObject(intObj);
    }

    AddSpikes(18,19,5);
    AddMagicBarrier(Vec2(62,16));

    LimitMap();
    FadeIn();
}

void U10::Update(float dt) {

}














//################################################
#define U11_0 "assets/img/U11/0_fundo.png"
#define U11_2 "assets/img/U11/2.png"
#define U11_4 "assets/img/U11/4.png"
#define U11_6 "assets/img/U11/6.png"
#define U11_7 "assets/img/U11/7.png"
#define U11_8 "assets/img/U11/8.png"
#define U11_9 "assets/img/U11/9_base.png"
#define U11_16 "assets/img/U11/16.png"

#define STONE3 "assets/img/U11/10_pedra_M.png"

U11::U11() {
    AddScenario(U11_0, LayerDistance::_Background_FarAway);
    AddScenario(U11_2, LayerDistance::_Background);
    AddScenario(U11_4, LayerDistance::_Background_VeryClose);
    AddScenario(U11_6, LayerDistance::_Scenery);
    AddScenario(U11_7, LayerDistance::_Scenery_Close);
    AddScenario(U11_8, LayerDistance::_Environment_Far);
    AddScenario(U11_9, LayerDistance::_Environment);
    AddScenario(U11_16, LayerDistance::_ForeGround);
}
#define MAP_U11 "assets/map/Zone/U11"
void U11::LoadAssets() {
    //#####################################
    stateMusic = ZoneManager::GetCarriedMusic();
    if(stateMusic == nullptr) {
        stateMusic = new Music(MUSIC_CROSS);
        stateMusic->Play(-1, STATE_FADE_TIME*1000);
    }

    GameObject* TileObj = new GameObject(LayerDistance::_Background_Far);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE,TILE_SIZE);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_U11,1,true);
    tile->LoadCollision(MAP_U11);
    zoneSize = Vec2(tile->GetWidth()*TILE_SIZE, tile->GetHeight()*TILE_SIZE);
    TileObj->AddComponent(tile);
    AddObject(TileObj);

    AddKid();

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

    //Boulder
    if(!GameData::boulder3){
        GameObject* boulderObj = new GameObject(LayerDistance::_Environment);
        boulderObj->AddComponent(new Boulder(*boulderObj, STONE3, Vec2(13,20)));
        AddObject(boulderObj);

        GameObject* intObj = new GameObject(LayerDistance::_Environment_Close);
        intObj->box = Rect(0,0,TILE_SIZE,TILE_SIZE);
        intObj->box.SetPurePosition(11*TILE_SIZE, 22.5*TILE_SIZE);
        Interactor* output = new Interactor(*intObj, true, true, ComponentType::_Boulder);
        output->SetResult([](){GameData::boulder3 = true;});
        intObj->AddComponent(output);
        AddObject(intObj);

    } else {
        GameObject* boulderObj = new GameObject(LayerDistance::_Environment);
        Sprite* boulderPic = new Sprite(*boulderObj, STONE3);
        boulderObj->AddComponent(boulderPic);
        boulderObj->box.SetSize(boulderPic->GetWidth(), boulderPic->GetHeight());
        boulderObj->box.SetPurePosition(11*TILE_SIZE,21*TILE_SIZE);
        boulderObj->AddComponent(new Collider(*boulderObj));
        AddObject(boulderObj);
    }

    //Setup Mechanism
    GameObject* mechaObj = new GameObject(LayerDistance::_Environment_Close);
    if(!GameData::secondMechanism){
        Sprite* mechanism = new Sprite(*mechaObj, MECHA1, 8);
        mechanism->SetFrame(0);
        mechaObj->box.SetSize(mechanism->GetWidth(), mechanism->GetHeight());
        mechaObj->box.SetPurePosition(5*TILE_SIZE, 18*TILE_SIZE);

        Interactor* mechaEnabler = new Interactor(*mechaObj, true);
        mechaEnabler->SetCondition([](){return (InputManager::GetInstance().KeyPress(SDLK_z));});
        mechaEnabler->SetResult([](){
            GameObject* mechaObj = new GameObject(LayerDistance::_Environment_Close);
            Sprite* mechanism = new Sprite(*mechaObj, MECHA1, 8, 0.1, true);
            mechaObj->box.SetSize(mechanism->GetWidth(), mechanism->GetHeight());
            mechaObj->box.SetPurePosition(5*TILE_SIZE, 18*TILE_SIZE);
           
            mechaObj->AddComponent(mechanism);
            Game::GetInstance().GetCurrentState().AddObject(mechaObj);

            GameData::secondMechanism = true;
        });
        mechaObj->AddComponent(mechanism);
        mechaObj->AddComponent(mechaEnabler);
    } else {
        Sprite* mechanism = new Sprite(*mechaObj, MECHA1, 8);
        mechaObj->box.SetSize(mechanism->GetWidth(), mechanism->GetHeight());
        mechaObj->box.SetPurePosition(5*TILE_SIZE, 18*TILE_SIZE);
        mechanism->SetFrame(7);
        mechaObj->AddComponent(mechanism);
    }
    AddObject(mechaObj);

    AddSpikes(28,23,16);

    GameObject* mouseGO = new GameObject(LayerDistance::_NPC);
    EnemyArmadillo* mouseCP = new EnemyArmadillo(*mouseGO);
    mouseGO->AddComponent(mouseCP);
    mouseGO->box.SetPosition(1444,296);
    AddObject(mouseGO);

    // GameObject* infectedGO = new GameObject(LayerDistance::_NPC);
    // EnemyRunner* infectedCP = new EnemyRunner(*infectedGO);
    // infectedGO->AddComponent(infectedCP);
    // infectedGO->box.SetPosition(1295,452);
    // AddObject(infectedGO);

    LimitMap();
    FadeIn();
}

void U11::Update(float dt) {

}














//################################################
#define U12_0 "assets/img/U12/0_fundo.png"
#define U12_2 "assets/img/U12/2.png"
#define U12_4 "assets/img/U12/4.png"
#define U12_7 "assets/img/U12/7.png"
#define U12_8 "assets/img/U12/8.png"
#define U12_9 "assets/img/U12/9_base.png"
#define U12_16 "assets/img/U12/16.png"
#define U12_17 "assets/img/U12/17.png"
#define U12_17_2 "assets/img/U12/17-2.png"

#define STONE4 "assets/img/U12/10_pedra_M.png"

U12::U12() {
    AddScenario(U12_0, LayerDistance::_Background_FarAway);
    AddScenario(U12_2, LayerDistance::_Background);
    AddScenario(U12_4, LayerDistance::_Background_VeryClose);
    AddScenario(U12_7, LayerDistance::_Scenery_Close);
    AddScenario(U12_8, LayerDistance::_Environment_Far);
    AddScenario(U12_9, LayerDistance::_Environment);
    AddScenario(U12_16, LayerDistance::_ForeGround);
    AddScenario(U12_17, LayerDistance::_ForeGround_Close);
    AddScenario(U12_17_2, LayerDistance::_ForeGround_Close);
}

#define MAP_U12 "assets/map/Zone/U12"
void U12::LoadAssets() {
    //#####################################
    stateMusic = ZoneManager::GetCarriedMusic();
    if(stateMusic == nullptr) {
        stateMusic = new Music(MUSIC_CROSS);
        stateMusic->Play(-1, STATE_FADE_TIME*1000);
    }

    GameObject* TileObj = new GameObject(LayerDistance::_Background_Far);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE,TILE_SIZE);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_U12,1,true);
    tile->LoadCollision(MAP_U12);
    zoneSize = Vec2(tile->GetWidth()*TILE_SIZE, tile->GetHeight()*TILE_SIZE);
    TileObj->AddComponent(tile);
    AddObject(TileObj);

    AddKid();

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

    GameObject* checkObj = new GameObject(LayerDistance::_Environment_Far);
    Sprite* checkPoint = new Sprite(*checkObj, CHECKPOINT_ANIM, 6, 0.1);
    checkObj->box.SetPurePosition(27.96*TILE_SIZE, 15.1*TILE_SIZE);
    checkObj->box.SetSize(checkPoint->GetWidth(), checkPoint->GetHeight());
    Interactor* checkSave = new Interactor(*checkObj);
    checkSave->SetResult([](){
        GameData::checkPoint = Zone::_U12;
        GameData::revivePosition = Vec2(27,16);
    });
    checkObj->AddComponent(checkPoint);
    checkObj->AddComponent(checkSave);
    AddObject(checkObj);

    if(!GameData::boulder4a and !GameData::boulder4b){
        GameObject* boulderObj = new GameObject(LayerDistance::_Environment);
        boulderObj->AddComponent(new Boulder(*boulderObj, STONE4, Vec2(39,18)));
        AddObject(boulderObj);

        GameObject* intObj = new GameObject(LayerDistance::_Environment_Close);
        intObj->box = Rect(0,0,TILE_SIZE,TILE_SIZE);
        intObj->box.SetPurePosition(36.1*TILE_SIZE, 19.5*TILE_SIZE);
        Interactor* output = new Interactor(*intObj, true, true, ComponentType::_Boulder);
        output->SetResult([](){GameData::boulder4a = true;});
        intObj->AddComponent(output);
        AddObject(intObj);

        GameObject* intObj2 = new GameObject(LayerDistance::_Environment_Close);
        intObj2->box = Rect(0,0,TILE_SIZE,TILE_SIZE);
        intObj2->box.SetPurePosition(43.9*TILE_SIZE, 19.5*TILE_SIZE);
        Interactor* output2 = new Interactor(*intObj2, true, true, ComponentType::_Boulder);
        output2->SetResult([](){GameData::boulder4b = true;});
        intObj2->AddComponent(output2);
        AddObject(intObj2);

    } else if(GameData::boulder4a) {
        GameObject* boulderObj = new GameObject(LayerDistance::_Environment);
        Sprite* boulderPic = new Sprite(*boulderObj, STONE4);
        boulderObj->AddComponent(boulderPic);
        boulderObj->box.SetSize(boulderPic->GetWidth(), boulderPic->GetHeight());
        boulderObj->box.SetPurePosition(37*TILE_SIZE,18*TILE_SIZE);
        boulderObj->AddComponent(new Collider(*boulderObj));
        AddObject(boulderObj);
    } else {
        GameObject* boulderObj = new GameObject(LayerDistance::_Environment);
        Sprite* boulderPic = new Sprite(*boulderObj, STONE4);
        boulderObj->AddComponent(boulderPic);
        boulderObj->box.SetSize(boulderPic->GetWidth(), boulderPic->GetHeight());
        boulderObj->box.SetPurePosition(42*TILE_SIZE,18*TILE_SIZE);
        boulderObj->AddComponent(new Collider(*boulderObj));
        AddObject(boulderObj);
    }

    GameObject* mouseGO = new GameObject(LayerDistance::_NPC);
    EnemyArmadillo* mouseCP = new EnemyArmadillo(*mouseGO);
    mouseGO->AddComponent(mouseCP);
    mouseGO->box.SetPosition(1120,466);
    AddObject(mouseGO);

    LimitMap();
    FadeIn();
}

void U12::Update(float dt) {

}














//################################################
#define U13_0 "assets/img/U13/0_base.png"
#define U13_4 "assets/img/U13/4.png"
#define U13_5 "assets/img/U13/5.png"
#define U13_6 "assets/img/U13/6.png"
#define U13_6_2 "assets/img/U13/6-2.png"
#define U13_8 "assets/img/U13/8.png"
#define U13_9 "assets/img/U13/9_base.png"
#define U13_16 "assets/img/U13/16.png"
#define U13_17 "assets/img/U13/17.png"
#define U13_17_2 "assets/img/U13/17-2.png"



U13::U13() {
    AddScenario(U13_0, LayerDistance::_Background_FarAway);
    AddScenario(U13_4, LayerDistance::_Background_VeryClose);
    AddScenario(U13_5, LayerDistance::_Scenery_Far);
    AddScenario(U13_6, LayerDistance::_Scenery);
    AddScenario(U13_6_2, LayerDistance::_Scenery);
    AddScenario(U13_8, LayerDistance::_Environment_Far);
    AddScenario(U13_9, LayerDistance::_Environment);
    AddScenario(U13_16, LayerDistance::_ForeGround);
    AddScenario(U13_17, LayerDistance::_ForeGround_Close);
    AddScenario(U13_17_2, LayerDistance::_ForeGround_Close);
}
#define MAP_U13 "assets/map/Zone/U13"
void U13::LoadAssets() {
    //#####################################
    stateMusic = ZoneManager::GetCarriedMusic();
    if(stateMusic == nullptr) {
        stateMusic = new Music(MUSIC_CROSS);
        stateMusic->Play(-1, STATE_FADE_TIME*1000);
    }

    GameObject* TileObj = new GameObject(LayerDistance::_Background_Far);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE,TILE_SIZE);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_U13,1,true);
    tile->LoadCollision(MAP_U13);
    zoneSize = Vec2(tile->GetWidth()*TILE_SIZE, tile->GetHeight()*TILE_SIZE);
    TileObj->AddComponent(tile);
    AddObject(TileObj);

    AddKid();

    //Path to U12-D
    GameObject *BObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorB = new ZoneTransition(*BObj, Rect(7, 14, 1, 4), 
        std::make_pair(Zone::_U13, ZoneExit::B));
    BObj->AddComponent(doorB);
    AddObject(BObj);

    AddSpikes(10,20,2);
    AddSpikes(14,22,9);
    AddSpikes(31,20,24);
    AddMagicBarrier(Vec2(23,10));

    GameObject* mechaObj = new GameObject(LayerDistance::_Environment_Close);
    if(!GameData::thirdMechanism){
        Sprite* mechanism = new Sprite(*mechaObj, MECHA2, 8);
        mechanism->SetFrame(0);
        mechaObj->box.SetSize(mechanism->GetWidth(), mechanism->GetHeight());
        mechaObj->box.SetPurePosition(52*TILE_SIZE, 16*TILE_SIZE);

        Interactor* mechaEnabler = new Interactor(*mechaObj, true);
        mechaEnabler->SetCondition([](){return (InputManager::GetInstance().KeyPress(SDLK_z));});
        mechaEnabler->SetResult([](){
            GameObject* mechaObj = new GameObject(LayerDistance::_Environment_Close);
            Sprite* mechanism = new Sprite(*mechaObj, MECHA2, 8, 0.1, true);
            mechaObj->box.SetSize(mechanism->GetWidth(), mechanism->GetHeight());
            mechaObj->box.SetPurePosition(52*TILE_SIZE, 16*TILE_SIZE);
           
            mechaObj->AddComponent(mechanism);
            Game::GetInstance().GetCurrentState().AddObject(mechaObj);

            GameData::thirdMechanism = true;
        });
        mechaObj->AddComponent(mechanism);
        mechaObj->AddComponent(mechaEnabler);
    } else {
        Sprite* mechanism = new Sprite(*mechaObj, MECHA2, 8);
        mechaObj->box.SetSize(mechanism->GetWidth(), mechanism->GetHeight());
        mechaObj->box.SetPurePosition(52*TILE_SIZE, 16*TILE_SIZE);
        mechanism->SetFrame(7);
        mechaObj->AddComponent(mechanism);
    }
    AddObject(mechaObj);

    if(!GameData::canUseMagicAttack) {
        GameObject* intObj = new GameObject(LayerDistance::_Environment_Close);
        intObj->box = Rect(0,0,TILE_SIZE,TILE_SIZE);
        intObj->box.SetPurePosition(21*TILE_SIZE,12*TILE_SIZE);
        Interactor* output = new Interactor(*intObj, true);
        output->SetCondition([](){return InputManager::GetInstance().KeyPress(SDLK_z);});
        
        output->SetResult([](){
            GameData::canUseMagicAttack = true;
        });

        intObj->AddComponent(output);
        intObj->AddComponent(new Sprite(*intObj,FIRE, 30, 0.1));
        AddObject(intObj);
    }

    // GameObject* mouseGO = new GameObject(LayerDistance::_NPC);
    // EnemyArmadillo* mouseCP = new EnemyArmadillo(*mouseGO);
    // mouseGO->AddComponent(mouseCP);
    // mouseGO->box.SetPosition(987,485);
    // AddObject(mouseGO);

    // GameObject* infectedGO = new GameObject(LayerDistance::_NPC);
    // EnemyRunner* infectedCP = new EnemyRunner(*infectedGO);
    // infectedGO->AddComponent(infectedCP);
    // infectedGO->box.SetPosition(696,468);
    // AddObject(infectedGO);

    // infectedGO = new GameObject(LayerDistance::_NPC);
    // infectedCP = new EnemyRunner(*infectedGO);
    // infectedGO->AddComponent(infectedCP);
    // infectedGO->box.SetPosition(1260,540);
    // AddObject(infectedGO);

    LimitMap();
    FadeIn();
}

void U13::Update(float dt) {

}














//################################################
#define U14_0 "assets/img/U14/Mapa_02_A14_0.png"
#define U14_4 "assets/img/U14/Mapa_02_A14_4.png"
#define U14_6 "assets/img/U14/Mapa_02_A14_6.png"
#define U14_7 "assets/img/U14/Mapa_02_A14_7.png"
#define U14_8 "assets/img/U14/Mapa_02_A14_8.png"
#define U14_9 "assets/img/U14/Mapa_02_A14_9.png"

U14::U14() {
    AddScenario(U14_0, LayerDistance::_Background_FarAway);
    AddScenario(U14_4, LayerDistance::_Background_VeryClose);
    AddScenario(U14_6, LayerDistance::_Scenery);
    AddScenario(U14_7, LayerDistance::_Scenery_Close);
    AddScenario(U14_8, LayerDistance::_Environment_Far);
    AddScenario(U14_9, LayerDistance::_Environment);
}
#define MAP_U14 "assets/map/Zone/U14"
void U14::LoadAssets() {
    stateMusic = ZoneManager::GetCarriedMusic();
    if(stateMusic == nullptr) {
        stateMusic = new Music(MUSIC_CROSS);
        stateMusic->Play(-1, STATE_FADE_TIME*1000);
    }

    GameObject* TileObj = new GameObject(LayerDistance::_Background_Far);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE,TILE_SIZE);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_U14,1,true);
    tile->LoadCollision(MAP_U14);
    zoneSize = Vec2(tile->GetWidth()*TILE_SIZE, tile->GetHeight()*TILE_SIZE);
    TileObj->AddComponent(tile);
    AddObject(TileObj);

    AddKid();

    //Path to U12-C
    GameObject *AObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorA = new ZoneTransition(*AObj, Rect(16, 7, 1, 1), 
        std::make_pair(Zone::_U14, ZoneExit::A));
    AObj->AddComponent(doorA);
    AddObject(AObj);

    AddSpikes(12,30,10);

    if(!GameData::thirdItem) {
        GameObject* intObj = new GameObject(LayerDistance::_Environment_Close);
        intObj->box = Rect(0,0,TILE_SIZE,TILE_SIZE);
        intObj->box.SetPurePosition(15.5*TILE_SIZE,28.5*TILE_SIZE);
        Interactor* output = new Interactor(*intObj, true);
        output->SetCondition([](){return InputManager::GetInstance().KeyPress(SDLK_z);});
        output->SetResult([](){GameData::thirdItem = true;});
        intObj->AddComponent(output);
        intObj->AddComponent(new Sprite(*intObj,POTION_ITEM, 30, 0.1));
        AddObject(intObj);
    }

    LimitMap();
    FadeIn();
}

void U14::Update(float dt) {

}














//################################################
#define U15_FUNDO    "assets/img/U15/0_fundo.png"
#define U15_4        "assets/img/U15/4t.png"
#define U15_6        "assets/img/U15/6t.png"
#define U15_7        "assets/img/U15/7.png"
#define U15_8        "assets/img/U15/8.png"
#define U15_9_BASE   "assets/img/U15/9_base.png"
#define U15_16       "assets/img/U15/16.png"
#define U15_17       "assets/img/U15/17.png"
#define U15_17_2     "assets/img/U15/17-2.png"

//Animated
#define U15_8_C      "assets/img/U15/8_cachoeira.png"
#define U15_8_CACHOEIRA     "assets/img/U15/8_cachoeira_228x424.png"
#define U15_FONTE    "assets/img/U15/8_fonte_142x162.png"
#define U15_BARREIRA "assets/img/U15/10_barreira14x83.png"
U15::U15() {
    AddScenario(U15_FUNDO, LayerDistance::_Background_Far);
    AddScenario(U15_4, LayerDistance::_Background_VeryClose, 0.55);
    AddScenario(U15_6, LayerDistance::_Scenery, 0,7);
    AddScenario(U15_7, LayerDistance::_Scenery_Close, 0.8, true);
    AddScenario(U15_8, LayerDistance::_Environment_Far);
    AddScenario(U15_9_BASE, LayerDistance::_Environment);
    AddScenario(U15_16, LayerDistance::_ForeGround);
    AddScenario(U15_17, LayerDistance::_ForeGround_Close, 1.15);
    AddScenario(U15_17_2, LayerDistance::_ForeGround_Close, 1.15);
}
#define MAP_U15 "assets/map/Zone/U15"
void U15::LoadAssets() {
    //#####################################
    stateMusic = ZoneManager::GetCarriedMusic();
    if(stateMusic == nullptr) {
        stateMusic = new Music(MUSIC_CROSS);
        stateMusic->Play(-1, STATE_FADE_TIME*1000);
    }

    GameObject* TileObj = new GameObject(LayerDistance::_Background_Far);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE,TILE_SIZE);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_U15,1,true);
    tile->LoadCollision(MAP_U15);
    zoneSize = Vec2(tile->GetWidth()*TILE_SIZE, tile->GetHeight()*TILE_SIZE);
    TileObj->AddComponent(tile);
    AddObject(TileObj);

    AddKid();

    //Path to U12-B
    GameObject *AObj = new GameObject(LayerDistance::_Environment);
    ZoneTransition* doorA = new ZoneTransition(*AObj, Rect(98, 21, 1, 3), 
        std::make_pair(Zone::_U15, ZoneExit::A));
    AObj->AddComponent(doorA);
    AddObject(AObj);

    AddAnimated(U15_FONTE, LayerDistance::_Environment, Vec2(16, 18.2286),4);
    AddAnimated(U15_8_CACHOEIRA, LayerDistance::_Environment_Far, Vec2(34, 15),10);

    //Interaction - 4th item
    if(!GameData::fourthItem) {
        GameObject* intObj = new GameObject(LayerDistance::_Environment_Close);
        intObj->box = Rect(0,0,TILE_SIZE,TILE_SIZE);
        intObj->box.SetPurePosition(18.1*TILE_SIZE,22*TILE_SIZE);
        Interactor* output = new Interactor(*intObj, true);
        output->SetCondition([](){return InputManager::GetInstance().KeyPress(SDLK_z);});
        output->SetResult([](){GameData::fourthItem = true;});
        intObj->AddComponent(output);
        intObj->AddComponent(new Sprite(*intObj,POTION_ITEM, 30, 0.1));
        AddObject(intObj);
    }

    AddSpikes(34,29,9);
    AddSpikes(64,24,26);
    AddSpikes(83,23,4);
    AddMagicBarrier(Vec2(25,21));

    LimitMap();
    FadeIn();
}

void U15::Update(float dt) {

}











//##############################################################################
#define U16_0 "assets/img/U16/Mapa_02_A16_0.png"
#define U16_5 "assets/img/U16/Mapa_02_A16_5.png"
#define U16_7 "assets/img/U16/Mapa_02_A16_7.png"
#define U16_8 "assets/img/U16/Mapa_02_A16_8.png"
#define U16_9 "assets/img/U16/Mapa_02_A16_9.png"
#define U16_17 "assets/img/U16/Mapa_02_A16_17.png"

U16::U16() {
    AddScenario(U16_0, LayerDistance::_Background_FarAway);
    AddScenario(U16_5, LayerDistance::_Scenery_Far);
    AddScenario(U16_7, LayerDistance::_Scenery_Close);
    AddScenario(U16_8, LayerDistance::_Environment_Far);
    AddScenario(U16_9, LayerDistance::_Environment);
    AddScenario(U16_17, LayerDistance::_ForeGround_Close);
}
#define MAP_U16 "assets/map/Zone/U16"
void U16::LoadAssets() {
    //#####################################
    stateMusic = ZoneManager::GetCarriedMusic();
    if(stateMusic == nullptr) {
        stateMusic = new Music(MUSIC_CROSS);
        stateMusic->Play(-1, STATE_FADE_TIME*1000);
    }

    GameObject* TileObj = new GameObject(LayerDistance::_Background_Far);
    TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE,TILE_SIZE);
    TileMap* tile = new TileMap(*TileObj, Set, MAP_U16,1,true);
    tile->LoadCollision(MAP_U16);
    zoneSize = Vec2(tile->GetWidth()*TILE_SIZE, tile->GetHeight()*TILE_SIZE);
    TileObj->AddComponent(tile);
    AddObject(TileObj);

    AddKid();

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

    GameObject* infectedGO = new GameObject(LayerDistance::_NPC);
    EnemyRunner* infectedCP = new EnemyRunner(*infectedGO);
    infectedGO->AddComponent(infectedCP);
    infectedGO->box.SetPosition(1995,624);
    AddObject(infectedGO);

    infectedGO = new GameObject(LayerDistance::_NPC);
    infectedCP = new EnemyRunner(*infectedGO);
    infectedGO->AddComponent(infectedCP);
    infectedGO->box.SetPosition(1617,600);
    AddObject(infectedGO);

    LimitMap();
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
//     GameObject* TileObj = new GameObject(LayerDistance::_Background_Far);
//     TileSet* Set = new TileSet(*TileObj, SPRITE_TILE, TILE_SIZE,TILE_SIZE);
//     TileMap* tile = new TileMap(*TileObj, Set, MAP_U,1,true);
//     tile->LoadCollision(MAP_U);
//     TileObj->AddComponent(tile);
//     AddObject(TileObj);

//     AddKid();

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
        // LimitMap();
//     FadeIn();
// }

// void U::Update(float dt) {

// }

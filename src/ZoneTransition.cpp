#include "ZoneTransition.h"
#include "GameScene.h"

ZoneTransition::ZoneTransition (GameObject& associated, Rect collider, std::pair<Zone, ZoneExit> zoneExit, bool carryMusic)
: Component(associated) {
    transitioning = false;
    this->carryMusic = carryMusic;
    collider = Rect(collider.x*TILE_SIZE, collider.y*TILE_SIZE, 
        collider.w*TILE_SIZE, collider.h*TILE_SIZE);
    associated.box = collider;
    this->zoneExit = zoneExit;
    Collider* door = new Collider(associated);
    door->box = collider;
    associated.AddComponent(door);
}

ZoneTransition::~ZoneTransition () {}
void ZoneTransition::Start () {}
void ZoneTransition::Update (float dt) {}

void ZoneTransition::LateUpdate (float dt) {
    if (transitioning and fade->HasFinished()) {
        ZoneManager::RequestZone(zoneExit,carryMusic);
    }
}

void ZoneTransition::Render() {}

bool ZoneTransition::Is (std::string type) {
    return ("ZoneTransition" == type);
}

bool ZoneTransition::Is (ComponentType type) {
    return (this->type & type);
}

void ZoneTransition::NotifyCollision (GameObject& other){
    if(other.Contains(ComponentType::_Dummy) and !transitioning) {
        GameObject* outObj = new GameObject(LayerDistance::_ForeGround_VeryClose);
        fade = new ScreenFade(*outObj, Color("#000000"), 0, 1, STATE_FADE_TIME);
        outObj->AddComponent(fade);
        Game::GetInstance().GetCurrentState().AddObject(outObj);
        transitioning = true;
        if(!carryMusic) {
            Game::GetInstance().GetCurrentState().GetStateMusic()->Stop(STATE_FADE_TIME*1000);
        }
    }
}

//########################################

// Vec2 ZoneManager::spawnPosition = Vec2(13, 18);//1st map initial position
Vec2 ZoneManager::spawnPosition = Vec2(11, 15);//U3 close to U4
Music* ZoneManager::levelMusic = nullptr;

ZoneManager::ZoneManager() {
    //TODO Set spawn point from load file or to 1st map
}

void ZoneManager::RequestZone(std::pair<Zone, ZoneExit> place, bool carryMusic) {
    switch(place.first) {
        case Zone::_S1:
            if(place.second == ZoneExit::A) {//To S2-A
                spawnPosition = Vec2(11, 15);
                Game::GetInstance().AddState(new S2());
            }
            break;

        case Zone::_S2:
            if(place.second == ZoneExit::A) {//to S1-A
                spawnPosition = Vec2(84, 15);
                Game::GetInstance().AddState(new S1());
            } 
            else if(place.second == ZoneExit::B) {//to U1-A
                spawnPosition = Vec2(57, 8);
                Game::GetInstance().AddState(new U1());
            }
            else if(place.second == ZoneExit::C) {//to S3-A
                spawnPosition = Vec2(12, 18);
                Game::GetInstance().AddState(new S3());
            }
            break;

        case Zone::_S3:
            if(place.second == ZoneExit::A) {//To S2-C
                spawnPosition = Vec2(126, 18);
                Game::GetInstance().AddState(new S2());
            }
            else if(place.second == ZoneExit::B) {//to S4-A
                spawnPosition = Vec2(18, 18);
                Game::GetInstance().AddState(new S4());
            }
            break;
            

        case Zone::_S4:
            if(place.second == ZoneExit::A) {//to S3-B
                spawnPosition = Vec2(53, 18);
                Game::GetInstance().AddState(new S3());
            } 
            else if(place.second == ZoneExit::B) {//to U16-A
                spawnPosition = Vec2(11, 5);
                Game::GetInstance().AddState(new U16());
            }
            break;

        //##################################
        case Zone::_U1:
            if(place.second == ZoneExit::A) {//To S2-B
                spawnPosition = Vec2(103, 25);
                Game::GetInstance().AddState(new S2());
            }
            else if(place.second == ZoneExit::B) {//to U5-B
                spawnPosition = Vec2(46, 13);
                Game::GetInstance().AddState(new U5());
                
            }
            else if(place.second == ZoneExit::C) {//to U2-A
                spawnPosition = Vec2(11, 26);
                Game::GetInstance().AddState(new U2());
            }
            break;

        case Zone::_U2:
            if(place.second == ZoneExit::A) {//to U1-C
                spawnPosition = Vec2(77, 41);
                Game::GetInstance().AddState(new U1());
            } 
            else if(place.second == ZoneExit::B) {//to U3-B
                spawnPosition = Vec2(87, 15);
                Game::GetInstance().AddState(new U3());
            }
            else if(place.second == ZoneExit::C) {//to U8-A
                spawnPosition = Vec2(6, 20);
                Game::GetInstance().AddState(new U8());
            }
            else if(place.second == ZoneExit::D) {//to U6-A
                spawnPosition = Vec2(17, 20);
                Game::GetInstance().AddState(new U6());
            }
            break;

        case Zone::_U3:
            if(place.second == ZoneExit::A) {//to U4-A 
                spawnPosition = Vec2(46, 44);
                Game::GetInstance().AddState(new U4());
            } 
            else if(place.second == ZoneExit::B) {//to U2-B
                spawnPosition = Vec2(11, 40);
                Game::GetInstance().AddState(new U2());
            }
            break;
            
        case Zone::_U4:
            if(place.second == ZoneExit::A) {//to U3-A
                spawnPosition = Vec2(8, 15);
                Game::GetInstance().AddState(new U3());
            } 
            else if(place.second == ZoneExit::B) {//to U5-B
                spawnPosition = Vec2(18, 10);
                Game::GetInstance().AddState(new U5());
            }
            break;
            
        case Zone::_U5:
            if(place.second == ZoneExit::A) {//to U4-B
                spawnPosition = Vec2(47, 24);
                Game::GetInstance().AddState(new U4());
            } 
            else if(place.second == ZoneExit::B) {//to U1-B
                spawnPosition = Vec2(18, 39);
                Game::GetInstance().AddState(new U1());
            }
            break;
            
        case Zone::_U6:
            if(place.second == ZoneExit::A) {//to U2-D
                spawnPosition = Vec2(20, 23);
                Game::GetInstance().AddState(new U2());
            } 
            else if(place.second == ZoneExit::B) {//to U7-A
                spawnPosition = Vec2(13, 17);
                Game::GetInstance().AddState(new U7());
            }
            break;
            
        case Zone::_U7:
            if(place.second == ZoneExit::A) {//to U6-B 
                spawnPosition = Vec2(50, 18);
                Game::GetInstance().AddState(new U6());
            } 
            else if(place.second == ZoneExit::B) {//to U16-B
                spawnPosition = Vec2(6, 30);
                Game::GetInstance().AddState(new U16());
            }
            else if(place.second == ZoneExit::C) {//to U9-D
                spawnPosition = Vec2(22, 16);
                Game::GetInstance().AddState(new U9());
            }
            break;
            
        case Zone::_U8:
            if(place.second == ZoneExit::A) {//to U2-C
                spawnPosition = Vec2(20, 37);
                Game::GetInstance().AddState(new U2());
            } 
            else if(place.second == ZoneExit::B) {//to U11-A
                spawnPosition = Vec2(54, 5);
                Game::GetInstance().AddState(new U11());
            }
            else if(place.second == ZoneExit::C) {//to U9-A
                spawnPosition = Vec2(10, 17);
                Game::GetInstance().AddState(new U9());
            }
            break;

        case Zone::_U9:
            if(place.second == ZoneExit::A) {//to U8-C
                spawnPosition = Vec2(25, 18);
                Game::GetInstance().AddState(new U8());
            } 
            else if(place.second == ZoneExit::B) {//to U13-A
                spawnPosition = Vec2(18, 7);
                Game::GetInstance().AddState(new U13());
            }
            else if(place.second == ZoneExit::C) {//to U10-A
                spawnPosition = Vec2(9, 17);
                Game::GetInstance().AddState(new U10());
            }
            else if(place.second == ZoneExit::D) {//to U7-C
                spawnPosition = Vec2(13, 30);
                Game::GetInstance().AddState(new U7());
            }
            break;
            
        case Zone::_U10:
            if(place.second == ZoneExit::A) {//to U9-C
                spawnPosition = Vec2(22, 31);
                Game::GetInstance().AddState(new U9());
            }
            break;
            
        case Zone::_U11:
            if(place.second == ZoneExit::A) {//to U8-B
                spawnPosition = Vec2(22, 22);
                Game::GetInstance().AddState(new U8());
            } 
            else if(place.second == ZoneExit::B) {//to U12-A
                spawnPosition = Vec2(19, 12);
                Game::GetInstance().AddState(new U12());
            }
            break;
            
        case Zone::_U12:
            if(place.second == ZoneExit::A) {//to U11-B
                spawnPosition = Vec2(26, 24);
                Game::GetInstance().AddState(new U11());
            } 
            else if(place.second == ZoneExit::B) {//to U15-A
                spawnPosition = Vec2(96, 23);
                Game::GetInstance().AddState(new U15());
            }
            else if(place.second == ZoneExit::C) {//to U14-A
                spawnPosition = Vec2(16, 9);
                Game::GetInstance().AddState(new U14());
            }
            else if(place.second == ZoneExit::D) {//to U13-B
                spawnPosition = Vec2(9, 17);
                Game::GetInstance().AddState(new U13());
            }
            break;
            
        case Zone::_U13:
            if(place.second == ZoneExit::B) {//to U12-D
                spawnPosition = Vec2(46, 16);
                Game::GetInstance().AddState(new U12());
            }
            break;
            
        case Zone::_U14:
            if(place.second == ZoneExit::A) {//to U12-C
                spawnPosition = Vec2(38, 19);
                Game::GetInstance().AddState(new U12());
            } 
            break;
            
        case Zone::_U15:
            if(place.second == ZoneExit::A) {//to U12-B
                spawnPosition = Vec2(17, 18);
                Game::GetInstance().AddState(new U12());
            } 
            break;
            
        case Zone::_U16:
            if(place.second == ZoneExit::A) {//to S4-A
                spawnPosition = Vec2(38, 26);
                Game::GetInstance().AddState(new S4());
            } 
            else if(place.second == ZoneExit::B) {//to U7-B
                spawnPosition = Vec2(45, 20);
                Game::GetInstance().AddState(new U7());
            }
            break;
            
        default:
            break;
    }
    if(!carryMusic and levelMusic != nullptr) {
        delete levelMusic;
        levelMusic = nullptr;
    } else if(carryMusic) {
        levelMusic = Game::GetInstance().GetCurrentState().GetStateMusic();
    }

    Game::GetInstance().GetCurrentState().RequestPop();
}

Vec2 ZoneManager::GetSpawnPosition() {
    return spawnPosition;
}

Music* ZoneManager::GetCarriedMusic() {
    return levelMusic;
}
#include "GameData.h"

//Items for the potion
bool GameData::firstItem = false;
bool GameData::secondItem = false;
bool GameData::thirdItem = false;
bool GameData::fourthItem = false;
int GameData::itemCount = 0;

//Extra mechanisms for the door
bool GameData::firstMechanism = false;
bool GameData::secondMechanism = false;
bool GameData::thirdMechanism = false;
int GameData::mechanismCount = 0;

//habilities
bool GameData::canUseChargedAttack = false;
bool GameData::canUseMana = false;
bool GameData::canUseMagicAttack = false;

//Boulders
bool GameData::boulder1 = false;
bool GameData::boulder2 = false;
bool GameData::boulder3 = false;
bool GameData::boulder4a = false;
bool GameData::boulder4b = false;

bool GameData::openedU2Gate = false;
bool GameData::openedU16Gate = false;
bool GameData::changedS3Scenario = false;

//Kiddo
int GameData::kidHp = 0;
int GameData::kidMp = 0;

//Chat controllers
bool GameData::firstChat = false;
bool GameData::firstChatAlche = false;

//Default first spawn point is on area S1, in front of the house
Zone GameData::checkPoint = Zone::_S1;
Vec2 GameData::revivePosition = Vec2(13, 18);


void GameData::MultiConditionalChecks() {
    itemCount = (int)firstItem + (int)secondItem + (int)thirdItem + (int)fourthItem;
    mechanismCount = (int)firstMechanism + (int)secondMechanism + (int)thirdMechanism;
    SDL_Log("ITEMS: %d", itemCount);
    SDL_Log("MECHAS: %d", mechanismCount);
    SDL_Log("USE MANA: %s", (canUseMana?"YES":"NO"));
    SDL_Log("USE MAGIC ATTACK: %s", (canUseMagicAttack?"YES":"NO"));
    SDL_Log("USE CHARGED ATTACK: %s", (canUseChargedAttack?"YES":"NO"));

}

void GameData::ZerarTudo() {
    //Items for the potion
    firstItem = false;
    secondItem = false;
    thirdItem = false;
    fourthItem = false;
    itemCount = 0;

    //Extra mechanisms for the door
    firstMechanism = false;
    secondMechanism = false;
    thirdMechanism = false;
    mechanismCount = 0;

    //habilities
    canUseChargedAttack = false;
    canUseMana = false;
    canUseMagicAttack = false;

    //Boulders
    boulder1 = false;
    boulder2 = false;
    boulder3 = false;
    boulder4a = false;
    boulder4b = false;

    openedU2Gate = false;
    openedU16Gate = false;
    changedS3Scenario = false;

    //Kiddo
    kidHp = 0;
    kidMp = 0;

    //Chat controllers
    firstChat = false;
    firstChatAlche = false;

    //Default first spawn point is on area S1, in front of the house
    checkPoint = Zone::_S1;
    revivePosition = Vec2(13, 18);
}
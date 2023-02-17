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
bool GameData::canPushBoulder = false;
bool GameData::canUseMana = false;
bool GameData::canUseChargedAttack = false;
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
int GameData::kidMana = 0;

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
}
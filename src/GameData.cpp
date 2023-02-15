#include "GameData.h"

//Items for the potion
bool GameData::firstItem = false;
bool GameData::secondItem = false;
bool GameData::thirdItem = false;
bool GameData::fourthItem = false;
int GameData::itemCount = 0;

//For recovery when dying
Vec2 GameData::respawnPos = Vec2(0,0);
bool GameData::canPushBoulder = false;

bool GameData::changedS3Scenario = false;


void GameData::MultiConditionalChecks() {
    itemCount = (int)firstItem + (int)secondItem + (int)thirdItem + (int)fourthItem;

}
#ifndef GAMEDATA_H
#define GAMEDATA_H
#include "GentooEngine.h"
#include "ZoneTransition.h"

class GameData
{
    public:
        static void MultiConditionalChecks();

        //Conditions to unlock
        static bool firstItem;
        static bool secondItem;
        static bool thirdItem;
        static bool fourthItem;
        static int itemCount;

        static Vec2 respawnPos;

        static bool canPushBoulder;

        static bool changedS3Scenario;

        //Info from checkpoints
        static Zone checkPoint;
        static Vec2 revivePosition;

};

#endif//GAMEDATA_H
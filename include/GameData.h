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

        static bool firstMechanism;
        static bool secondMechanism;
        static bool thirdMechanism;
        static int mechanismCount;

        static bool canPushBoulder;
        static bool canUseMana;
        static bool canUseChargedAttack;
        static bool canUseMagicAttack;
        static bool boulder1;
        static bool boulder2;
        static bool boulder3;
        static bool boulder4a;
        static bool boulder4b;

        static bool openedU2Gate;
        static bool openedU16Gate;
        static bool changedS3Scenario;

        //Info from checkpoints
        static Zone checkPoint;
        static Vec2 revivePosition;

        //Kiddo
        static int kidHp;
        static int kidMana;

        //Dialogue dependant
        static bool firstChat;
        static bool firstChatAlche;

};

#endif//GAMEDATA_H
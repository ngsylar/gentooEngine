#ifndef ZONE_TRANSIT_H
#define ZONE_TRANSIT_H

#include "GentooEngine.h"
#include "State.h"

enum Zone {
    //House
    _H1,
    
    //Surface
    _S1,
    _S2,
    _S3,
    _S4,

    //Underground
    _U1,
    _U2,
    _U3,
    _U4,
    _U5,
    _U6,
    _U7,
    _U8,
    _U9,
    _U10,
    _U11,
    _U12,
    _U13,
    _U14,
    _U15,
    _U16,
};

enum ZoneExit {
    A,
    B,
    C,
    D,
};

class ZoneTransition : public Component {
    private:
        bool transitioning;
        bool carryMusic;
        ScreenFade* fade;
        std::pair<Zone, ZoneExit> zoneExit;

    public:
        ZoneTransition(GameObject& associated, Rect collider, std::pair<Zone, ZoneExit>, bool carryMusic=true);
        ~ZoneTransition();
        void Start();
        void Update(float dt);
        void LateUpdate(float dt);
        void Render();
        bool Is(std::string type);
        bool Is(ComponentType type);
        void NotifyCollision(GameObject& other);

};

class ZoneManager {
    private:
        static Vec2 spawnPosition;
        static Music* levelMusic;
    public:
    
        ZoneManager();
        ~ZoneManager();
        static void RequestZone(std::pair<Zone, ZoneExit> place, bool carryMusic);
        static Vec2 GetSpawnPosition();
        static Music* GetCarriedMusic();
};

#endif// ZONE_TRANSIT_H
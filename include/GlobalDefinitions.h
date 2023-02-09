#ifndef GLOBAL_DEFINITIONS
#define GLOBAL_DEFINITIONS

// Window definitions
#define WINDOW_TITLE        "Game Title"
#define WINDOW_SIZE         455, 256
#define GAME_RESOLUTION     455, 256

// State fade definition
#define STATE_FADE_TIME     0.5f

// Tile dimension
#define TILE_SIZE           28

// GameObject Optimization enums for bitwise based operations
enum ComponentType {
    // Engine Components
    _None =                   0,
    _EntityMachine =     1 << 0,
    _RigidBody =         1 << 1,
    _Collider =          1 << 2,
    _Sprite =            1 << 3,
    _TileMap =           1 << 4,
    _Sound =             1 << 5,
    _Text =              1 << 6,
    _ScreenFilter =      1 << 7,
    _ScreenFade =        1 << 8,
    _CameraFollower =    1 << 9,
    _CameraBox =        1 << 10,
    _LoopedBackground = 1 << 11,

    // Game Components
    _PlatformCamera =   1 << 12,
    _Kid =              1 << 13,
    _EnemyArmadillo =   1 << 14,
    _Attack =           1 << 15,
    _DialogueBox =      1 << 16,
    _Terrain =          1 << 17,
    // x = 1 << 18,
    // x = 1 << 19,
    // x = 1 << 20,

    // Testing Environment
    _Ball =             1 << 21,
    _RigidBodyLegacy =  1 << 22,
    _Dummy =            1 << 23,

    //...
};

inline ComponentType operator| (ComponentType a, ComponentType b) {
    return static_cast<ComponentType>(static_cast<int>(a) | static_cast<int>(b));
}

inline ComponentType operator& (ComponentType a, ComponentType b) {
    return static_cast<ComponentType>(static_cast<int>(a) & static_cast<int>(b));
}

inline ComponentType operator^ (ComponentType a, ComponentType b) {
    return static_cast<ComponentType>(static_cast<int>(a) ^ static_cast<int>(b));
}

enum LayerDistance {
    _Background_FarAway,
    _Background_Far,
    _Background,
    _Background_Close,
    _Background_VeryClose,
    _Scenery_Far,
    _Scenery,
    _Scenery_Close,
    _Environment_Far,
    _Environment,
    _Environment_Close,
    _NPC_Far,
    _NPC,
    _NPC_Close,
    _Player_Back,
    _Player,
    _Player_Front,
    _ForeGround,
    _ForeGround_Close,
    _ForeGround_VeryClose,
};

enum EntityState {
    Idle,
    Walking,
    Running,
    Jumping,
    Falling,
    Attacking,
    AttackingSwordOnGround_0,
    AttackingSwordOnGround_1,
    AttackingSwordOnAir,
    CastingSpell,
    Injured,
    Dying,

    // Leave declaration below as the last element,
    // it is the index for how many valid states there are
    _statesCount,

    // Declaration below is for initialization
    None = 0xFF,
};

#endif
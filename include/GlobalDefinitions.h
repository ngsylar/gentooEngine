#ifndef GLOBAL_DEFINITIONS
#define GLOBAL_DEFINITIONS

// Window definitions
#define WINDOW_TITLE        "Game Title"
#define WINDOW_SIZE         455, 256
#define GAME_RESOLUTION     455, 256

//State fade definition
#define STATE_FADE_TIME 0.5f

//Tile dimension
#define TILE_SIZE 28

// GameObject Optimization enums for bitwise based operations
enum ComponentType {
    // Engine Components
    _None =                   0,
    _RigidBody =         1 << 0,
    _Collider =          1 << 1,
    _Sprite =            1 << 2,
    _TileMap =           1 << 3,
    _Sound =             1 << 4,
    _Text =              1 << 5,
    _ScreenFilter =      1 << 6,
    _ScreenFade =        1 << 7,
    _CameraFollower =    1 << 8,
    _CameraBox =         1 << 9,
    _LoopedBackground = 1 << 10,
    _DialogueBox =      1 << 15,

    // Game Components
    _PlatformCamera =   1 << 11,
    _Kid =              1 << 12,
    _Enemy =            1 << 13,
    _EntityMachine =    1 << 14,
    _Attack =           1 << 14,
    
    // x = 1 << 16,
    // x = 1 << 17,
    // x = 1 << 18,
    // x = 1 << 19,

    // Testing Environment
    _Ball =             1 << 20,
    _RBody =            1 << 21,
    _Dummy =            1 << 22,
    _Terrain =          1 << 30,
    
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
    _Player,
    _Player_Item,
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
    HitTaken,

    // Leave declaration below as the last element,
    // it is the index for how many valid states there are
    __TOTAL,

    // Declaration below is for initialization
    None = 0xFF,
};

#endif
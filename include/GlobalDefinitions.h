#ifndef GLOBAL_DEFINITIONS
#define GLOBAL_DEFINITIONS

//Window definitions
#define WINDOW_TITLE        "Game Title"
#define WINDOW_SIZE         1280, 720
#define GAME_RESOLUTION     640, 360

//GameObject Optimization enums for bitwise based operations

enum GameObjID {
    _None =                   0,
    _CameraBox =         1 << 0,
    _CameraFollower =    1 << 1,
    _Collider =          1 << 2,
    _Kid =               1 << 3,
    _LoopedBackground =  1 << 4,
    _PlatformCamera =    1 << 5,
    _RigidBody =         1 << 6,
    _Sound =             1 << 7,
    _Sprite =            1 << 8,
    _Ball =              1 << 9,
    _Text =              1 << 10,
    _TileMap =           1 << 11,
    _ScreenFade =        1 << 12,
    _ScreenFilter =      1 << 13,
    // x = 1 << 14,
    // x = 1 << 15,
    // x = 1 << 16,
    // x = 1 << 17,
    // x = 1 << 18,
    // x = 1 << 19,
    // x = 1 << 20,
    //...
};

inline GameObjID operator|(GameObjID a, GameObjID b) {
    return static_cast<GameObjID>(static_cast<int>(a) | static_cast<int>(b));
}

inline GameObjID operator&(GameObjID a, GameObjID b) {
    return static_cast<GameObjID>(static_cast<int>(a) & static_cast<int>(b));
}

inline GameObjID operator^(GameObjID a, GameObjID b) {
    return static_cast<GameObjID>(static_cast<int>(a) ^ static_cast<int>(b));
}

#endif//GLOBAL_DEFINITIONS
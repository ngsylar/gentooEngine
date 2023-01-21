#ifndef GLOBAL_DEFINITIONS
#define GLOBAL_DEFINITIONS

// Window definitions
#define WINDOW_TITLE        "Game Title"
#define WINDOW_SIZE         455, 256
#define GAME_RESOLUTION     455, 256

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

    // Game Components
    _PlatformCamera =   1 << 11,
    _Kid =              1 << 12,
    _Enemy =            1 << 13,
    // x = 1 << 14,
    // x = 1 << 15,
    // x = 1 << 16,
    // x = 1 << 17,
    // x = 1 << 18,
    // x = 1 << 19,

    // Testing Environment
    _Ball =             1 << 20,
    
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

#endif
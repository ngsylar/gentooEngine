// C++ Library
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <memory>
#include <stack>
#include <vector>
#include <unordered_map>
#include <string>

// SDL2 Library
#define INCLUDE_SDL
#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER
#define INCLUDE_SDL_TTF
#include "SDL_include.h"

// Math Custom Library
#include "Timer.h"
#include "Vec2.h"

// Engine System Components
#include "Rect.h"
#include "GameObject.h"
#include "Camera.h"
#include "InputManager.h"
#include "Collision.h"
#include "Component.h"
#include "Resources.h"

// Engine Object Components
#include "CameraFollower.h"
#include "Collider.h"
#include "Sprite.h"
#include "TileSet.h"
#include "TileMap.h"
#include "Music.h"
#include "Sound.h"
#include "Text.h"

// Engine Control Core
#include "State.h"
#include "Game.h"

// editar: fazer tratamento de erros
// editar: transformar alguns ponteiros em weak_ptr
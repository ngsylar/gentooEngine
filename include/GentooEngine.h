// C++ Library
#include <functional>
#include <memory>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <array>
#include <stack>
#include <queue>
#include <vector>
#include <utility>
#include <unordered_map>
#include <string>
#include <iomanip>

// SDL2 Library
#define INCLUDE_SDL
#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER
#define INCLUDE_SDL_TTF
#include "SDL_include.h"

// Math Custom Library
#include "Timer.h"
#include "Vec2.h"

// Engine System Core
#include "Rect.h"
#include "GameObject.h"
#include "Camera.h"
#include "InputManager.h"
#include "Collision.h"
#include "Component.h"
#include "Resources.h"

// Engine Helpers
#include "TileSet.h"
#include "Color.h"
#include "Music.h"
#include "GlobalDefinitions.h"

// Object Components
#include "RigidBody.h"
#include "Collider.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Sound.h"
#include "Text.h"

// Auxiliary Components
#include "ScreenFilter.h"
#include "ScreenFade.h"
#include "CameraFollower.h"
#include "CameraBox.h"
#include "LoopedBackground.h"

// Game Control Core
#include "State.h"
#include "Game.h"

// editar: fazer tratamento de erros
// editar: transformar alguns ponteiros em weak_ptr
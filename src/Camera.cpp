#include "GentooEngine.h"

GameObject* Camera::focus = nullptr;
Vec2 Camera::pos, Camera::velocity, Camera::offset;
Vec2 Camera::posAdjustment, Camera::screenOffset;
std::array<bool, 2> Camera::isLocked = {true, true};
float Camera::tolerance = 0.2f;

Camera::Cinemachine Camera::cinemachine;
Camera::Player Camera::player;

void Camera::Follow (
    GameObject* newFocus,
    Vec2 cinemachineLength, int slicesX, int slicesY, int deadSlicesX, int deadSlicesY,
    int focusDirectionX, int focusDirectionY
) {
    /*--------------------------------------------------------------------------------------------------*/
    // camera attributes
    /*--------------------------------------------------------------------------------------------------*/

    posAdjustment = (Game::GetInstance().GetWindowSize() * 0.5f);
    velocity = Vec2();
    isLocked = {false, false};
    screenOffset = Vec2(0,25); // editar

    cinemachine.length = Vec2(25,175); // editar
    cinemachine.slices = {8, 32}; // editar
    cinemachine.deadSlices = {2, 28}; // editar
    cinemachine.Setup(true, true, true, false, true, true, false, false); // remover

    /*--------------------------------------------------------------------------------------------------*/
    // player attributes
    /*--------------------------------------------------------------------------------------------------*/

    focus = newFocus;
    player.previousPosition = focus->box.GetPosition();
    player.lastDirection = {LEFT, UP}; // editar
    player.lastVelocity = Vec2();
    player.isStopping = {false, false};

    player.stopwatch[X].SetResetTime(0.08);
    player.stopwatch[Y].SetResetTime(0.08);
    
    float direction[2];
    direction[X] = (player.lastDirection[X] == LEFT) ? -1 : 1;
    direction[Y] = (player.lastDirection[X] == DOWN) ? 1 : -1;

    offset = Vec2(
        direction[X] * cinemachine.length.x,
        direction[Y] * cinemachine.length.y
    );
}

void Camera::Unfollow () {
    focus = nullptr;
}

void Camera::Cinemachine::Setup (
    bool enableLeft, bool enableRight, bool enableUp, bool enableDown,
    bool resetLeft, bool resetRight, bool resetUp, bool resetDown
) {
    isAxisEnabled = {enableLeft, enableRight, enableUp, enableDown};
    isAxisResettable = {resetLeft, resetRight, resetUp, resetDown};
}

void Camera::Cinemachine::Accelerate (float* velocity, float focusVelocity, float displacement) {
    focusVelocity = fabs(focusVelocity);
    displacement = fabs(displacement);

    *velocity += (focusVelocity * focusVelocity) / (2.0f * displacement);
    if (*velocity >= focusVelocity)
        *velocity = 0.0f;
}

void Camera::Cinemachine::Decelerate (float* velocity, float focusVelocity, float displacement) {
    focusVelocity = fabs(focusVelocity);
    displacement = fabs(displacement);

    *velocity += (focusVelocity * focusVelocity) / (2.0f * displacement);
    if (*velocity < focusVelocity)
        *velocity = focusVelocity;
}

void Camera::Cinemachine::Chase (
    float* velocity, float* offset,
    float length, float centerDistance, float undeadZone, float slicedLength,
    float playerVelocity,  int axis, float playerDirection
) {
    player.stopwatch[axis].Reset();
    centerDistance *= playerDirection;
    
    // player is out of cinemachine length
    if (centerDistance < -(fabs(playerVelocity) + tolerance)) {
        isLocked[axis] = false;
        Decelerate(velocity, playerVelocity, undeadZone + slicedLength);
        *offset = playerDirection * (centerDistance + *velocity);
        // SDL_Log("1 %f", centerDistance);
    }
    // player is on center
    else if (centerDistance < tolerance) {
        *velocity = 0.0f;
        *offset = 0.0f;
        // SDL_Log("2 %f %f", centerDistance, tolerance);
    }
    // player is on undeadZone zone
    else if (centerDistance < undeadZone) {
        isLocked[axis] = false;
        Accelerate(velocity, playerVelocity, undeadZone + slicedLength);
        *offset = playerDirection * (centerDistance + *velocity);
        // SDL_Log("3 %f %f", centerDistance, undeadZone);
    }
    // player is on dead zone
    else if (centerDistance > undeadZone) {
        isLocked[axis] = true;
        // SDL_Log("4 %f %f", centerDistance, undeadZone);
    }
}

// editar: incluir desaceleracao de camera
void Camera::Cinemachine::StopChasing (
    float* offset, float length, float centerDistance,
    float playerVelocity, int axis, float playerDirection, float dt
) {
    player.stopwatch[axis].Update(dt);
    bool flags = player.stopwatch[axis].IsOver() and player.isStopping[axis];
    if (not (flags and (fabs(centerDistance) < (length - tolerance))))
        return;

    *offset += playerVelocity;
    if (fabs(*offset) >= (length - tolerance)) {
        *offset = playerDirection * length;
        player.isStopping[axis] = false;
    } else player.isStopping[axis] = true;
}

void Camera::Cinemachine::Update (float dt) {
    if (cinemachine.length == Vec2())
        return;

    Vec2 playerCurrentPosition = focus->box.GetPosition();
    Vec2 playerVelocity = playerCurrentPosition - player.previousPosition;
    player.previousPosition = focus->box.GetPosition();

    std::array<float, 2> slicedLength = {
        cinemachine.length.x / (float)cinemachine.slices[X],
        cinemachine.length.y / (float)cinemachine.slices[Y]
    };
    std::array<float, 2> undeadZoneSlices = {
        (float)cinemachine.slices[X] - cinemachine.deadSlices[X],
        (float)cinemachine.slices[Y] - cinemachine.deadSlices[Y]
    };
    std::array<float, 2> undeadZone = {
        slicedLength[X] * undeadZoneSlices[X], slicedLength[Y] * undeadZoneSlices[Y]
    };
    std::array<float, 2> centerDistance = {
        GetPosition().x - playerCurrentPosition.x,
        GetPosition().y - playerCurrentPosition.y
    };

    // player is going to the right
    if (isAxisEnabled[RIGHT] and (playerVelocity.x > 0.0f)) {
        Chase(&velocity.x, &offset.x, cinemachine.length.x, centerDistance[X],
            undeadZone[X], slicedLength[X], playerVelocity.x, X, 1);
        player.lastDirection[X] = RIGHT;
        player.isStopping[X] = false;
    }
    // player is going to the left
    else if (isAxisEnabled[LEFT] and (playerVelocity.x < 0.0f)) {
        Chase(&velocity.x, &offset.x, cinemachine.length.x, centerDistance[X],
            undeadZone[X], slicedLength[X], playerVelocity.x, X, -1);
        player.lastDirection[X] = LEFT;
        player.isStopping[X] = false;
    }
    // player is facing right
    else if (isAxisResettable[RIGHT] and (player.lastDirection[X] == RIGHT)) {
        StopChasing(&offset.x, cinemachine.length.x, centerDistance[X],
            player.lastVelocity.x, X, 1, dt);
        player.isStopping[X] = true;
    }
    // player is facing left
    else if (isAxisResettable[LEFT]) {
        StopChasing(&offset.x, cinemachine.length.x, centerDistance[X],
            player.lastVelocity.x, X, -1, dt);
        player.isStopping[X] = true;
    }

    // player is going down
    if (isAxisEnabled[DOWN] and (playerVelocity.y > 0.0f)) {
        Chase(&velocity.y, &offset.y, cinemachine.length.y, centerDistance[Y],
            undeadZone[Y], slicedLength[Y], playerVelocity.y, Y, 1);
        player.lastDirection[Y] = DOWN;
        player.isStopping[Y] = false;
    }
    // player is going up
    else if (isAxisEnabled[UP] and (playerVelocity.y < 0.0f)) {
        Chase(&velocity.y, &offset.y, cinemachine.length.y, centerDistance[Y],
            undeadZone[Y], slicedLength[Y], playerVelocity.y, Y, -1);
        player.lastDirection[Y] = UP;
        player.isStopping[Y] = false;
    }
    // player is facing down
    else if (isAxisResettable[DOWN] and (player.lastDirection[Y] == DOWN)) {
        StopChasing(&offset.y, cinemachine.length.y, centerDistance[Y],
            player.lastVelocity.y, Y, 1, dt);
        player.isStopping[Y] = true;
    }
    // player is facing up
    else if (isAxisResettable[UP]) {
        StopChasing(&offset.y, cinemachine.length.y, centerDistance[Y],
            player.lastVelocity.y, Y, -1, dt);
        player.isStopping[Y] = true;
    }

    if (not player.isStopping[X])
        player.lastVelocity.x = playerVelocity.x;
    if (not player.isStopping[Y])
        player.lastVelocity.y = playerVelocity.y;
}

void Camera::Update (float dt) {
    /*--------------------------------------------------------------------------------------------------*/
    // follows a player in focus
    /*--------------------------------------------------------------------------------------------------*/

    if (not focus) return;
    pos -= screenOffset;
    cinemachine.Update(dt);

    if (not isLocked[X])
        player.position.x = focus->box.GetPosition().x;
    if (not isLocked[Y])
        player.position.y = focus->box.GetPosition().y;

    pos = player.position + offset - posAdjustment;
    pos += screenOffset;
}

void Camera::Reset () {
    focus = nullptr;
    pos = Vec2();
    posAdjustment = (Game::GetInstance().GetWindowSize() * 0.5f);
    velocity = Vec2();
    isLocked = {true, true};
    offset = Vec2();
    screenOffset = Vec2();
    cinemachine.length = Vec2();
    cinemachine.slices = {0, 0};
    cinemachine.deadSlices = {0, 0};
}

Vec2 Camera::GetPosition () {
    return (pos + posAdjustment);
}
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
    screenOffset = Vec2();

    cinemachine.length = Vec2(25,175); // editar
    cinemachine.slices = {8, 0}; // editar
    cinemachine.deadSlices = {2, 0}; // editar

    /*--------------------------------------------------------------------------------------------------*/
    // player attributes
    /*--------------------------------------------------------------------------------------------------*/

    focus = newFocus;
    player.previousPosition = focus->box.GetPosition();
    player.lastDirection = {LEFT, UP}; // editar
    player.lastVelocity = Vec2();
    player.isStopping = {false, false};

    player.stopwatch[HORIZONTAL].SetResetTime(0.08);
    player.stopwatch[VERTICAL].SetResetTime(0.08);
    
    float direction[2];
    direction[HORIZONTAL] = (player.lastDirection[HORIZONTAL] == LEFT) ? -1 : 1;
    direction[VERTICAL] = (player.lastDirection[HORIZONTAL] == DOWN) ? 1 : -1;

    offset = Vec2(
        direction[HORIZONTAL] * cinemachine.length.x,
        direction[VERTICAL] * cinemachine.length.y
    );
}

void Camera::Unfollow () {
    focus = nullptr;
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
    float length, float centerDistance, float safeZone, float slicedLength,
    float playerVelocity,  int axis, float playerDirection
) {
    player.stopwatch[axis].Reset();
    centerDistance *= playerDirection;
    
    // player is out of cinemachine length
    if (centerDistance < -(fabs(playerVelocity) + tolerance)) {
        isLocked[axis] = false;
        Decelerate(velocity, playerVelocity, safeZone + slicedLength);
        *offset = playerDirection * (centerDistance + *velocity);
        SDL_Log("1 %f", centerDistance);
    }
    // player is on center
    else if (centerDistance < tolerance) {
        *velocity = 0.0f;
        *offset = 0.0f;
        SDL_Log("2 %f", centerDistance);
    }
    // player is on safe zone
    else if (centerDistance < safeZone) {
        isLocked[axis] = false;
        Accelerate(velocity, playerVelocity, safeZone + slicedLength);
        *offset = playerDirection * (centerDistance + *velocity);
        // SDL_Log("3");
    }
    // player is on dead zone
    else if (centerDistance > safeZone) {
        isLocked[axis] = true;
        // SDL_Log("4");
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

    float slicedLengthX = cinemachine.length.x / (float)cinemachine.slices[HORIZONTAL];
    float safeSlicesX = cinemachine.slices[HORIZONTAL] - cinemachine.deadSlices[HORIZONTAL];
    float safeZoneX = slicedLengthX * safeSlicesX;

    float centerDistance = GetPosition().x - playerCurrentPosition.x;

    // player is going to the right
    if (playerVelocity.x > 0.0f) {
        Chase(&velocity.x, &offset.x, cinemachine.length.x, centerDistance, safeZoneX, slicedLengthX,
            playerVelocity.x, HORIZONTAL, 1);
        player.lastDirection[HORIZONTAL] = RIGHT;
        player.isStopping[HORIZONTAL] = false;
    }
    // player is going to the left
    else if (playerVelocity.x < 0.0f) {
        Chase(&velocity.x, &offset.x, cinemachine.length.x, centerDistance, safeZoneX, slicedLengthX,
            playerVelocity.x, HORIZONTAL, -1);
        player.lastDirection[HORIZONTAL] = LEFT;
        player.isStopping[HORIZONTAL] = false;
    }
    // player is facing right
    else if (player.lastDirection[HORIZONTAL] == RIGHT) {
        StopChasing(&offset.x, cinemachine.length.x, centerDistance,
            player.lastVelocity.x, HORIZONTAL, 1, dt);
        player.isStopping[HORIZONTAL] = true;
    }
    // player is facing left
    else {
        StopChasing(&offset.x, cinemachine.length.x, centerDistance,
            player.lastVelocity.x, HORIZONTAL, -1, dt);
        player.isStopping[HORIZONTAL] = true;
    }

    if (not player.isStopping[HORIZONTAL])
        player.lastVelocity.x = playerVelocity.x;
}

void Camera::Update (float dt) {
    /*--------------------------------------------------------------------------------------------------*/
    // follows a player in focus
    /*--------------------------------------------------------------------------------------------------*/

    if (not focus) return;
    pos -= screenOffset;
    cinemachine.Update(dt);

    if (not isLocked[HORIZONTAL])
        player.position.x = focus->box.GetPosition().x;
    if (not isLocked[VERTICAL])
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
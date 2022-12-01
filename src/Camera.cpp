#include "GentooEngine.h"

GameObject* Camera::focus = nullptr;
Vec2 Camera::pos, Camera::velocity, Camera::offset;
Vec2 Camera::posAdjustment, Camera::screenOffset;
std::array<bool, 2> Camera::isLocked = {true, true};

Camera::Cinemachine Camera::cinemachine;
Camera::Player Camera::player;

void Camera::Follow (GameObject* newFocus) {
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
    player.lastDirection = {RIGHT, UP}; // editar
    
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

void Camera::Cinemachine::Accelerate (float focusVelocity, float displacement) {
    focusVelocity = fabs(focusVelocity);
    displacement = fabs(displacement);

    velocity.x += (focusVelocity * focusVelocity) / (2.0f * displacement);
    if (velocity.x >= focusVelocity)
        velocity.x = 0.0f;
}

void Camera::Cinemachine::Decelerate (float focusVelocity, float displacement) {
    focusVelocity = fabs(focusVelocity);
    displacement = fabs(displacement);

    velocity.x += (focusVelocity * focusVelocity) / (2.0f * displacement);
    if (velocity.x < focusVelocity)
        velocity.x = focusVelocity;
}

void Camera::Cinemachine::Chase (
    float length, float centerDistance, float safeZone, float slicedLength,
    float playerVelocity, float playerDirection
) {
    // player is out of cinemachine length
    if (centerDistance < -(fabs(playerVelocity) + 0.2f)) {
        isLocked[HORIZONTAL] = false;
        Decelerate(playerVelocity, safeZone + slicedLength);
        offset.x = playerDirection * (centerDistance + velocity.x);
    }
    // player is on center
    else if (centerDistance < 0.2f) {
        velocity.x = 0.0f;
        offset.x = 0.0f;
    }
    // player is on safe zone
    else if (centerDistance < safeZone) {
        isLocked[HORIZONTAL] = false;
        Accelerate(playerVelocity, safeZone + slicedLength);
        offset.x = playerDirection * (centerDistance + velocity.x);
    }
    // player is on dead zone
    else if (centerDistance > safeZone) {
        isLocked[HORIZONTAL] = true;
    }
}

void Camera::Cinemachine::Update (float dt) {
    if (cinemachine.length == Vec2())
        return;

    Vec2 playerCurrentPosition = focus->box.GetPosition();
    Vec2 playerVelocity = playerCurrentPosition - player.previousPosition;
    player.previousPosition = focus->box.GetPosition();

    float slicedLengthX = cinemachine.length.x / cinemachine.slices[HORIZONTAL];
    float safeSlicesX = cinemachine.slices[HORIZONTAL] - cinemachine.deadSlices[HORIZONTAL];
    float safeZoneX = slicedLengthX * safeSlicesX;

    // player is going to the right
    if (playerVelocity.x > 0.0f) {
        float centerDistance = GetPosition().x - playerCurrentPosition.x;
        Chase(cinemachine.length.x, centerDistance, safeZoneX, slicedLengthX, playerVelocity.x, 1);
    }
    // player is going to the left
    else if (playerVelocity.x < 0.0f) {
        float centerDistance = playerCurrentPosition.x - GetPosition().x;
        Chase(cinemachine.length.x, centerDistance, safeZoneX, slicedLengthX, playerVelocity.x, -1);
    }
    // player is facing right
}

void Camera::Update (float dt) {
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
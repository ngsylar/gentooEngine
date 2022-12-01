#include "GentooEngine.h"

GameObject* Camera::focus = nullptr;
Vec2 Camera::pos, Camera::posAdjustment, Camera::velocity, Camera::offset;
bool Camera::isLocked[2] = {true, true};

Camera::Cinemachine Camera::cinemachine;
Camera::Player Camera::player;

void Camera::Follow (GameObject* newFocus) {
    posAdjustment = (Game::GetInstance().GetWindowSize() * 0.5f);
    velocity = Vec2();
    isLocked[HORIZONTAL] = false;
    isLocked[VERTICAL] = false;
    screenOffset = Vec2();

    focus = newFocus;
    player.previousPosition = focus->box.GetPosition();

    cinemachine.length = Vec2(25,175); // remover
    cinemachine.slices = {8, 0}; // remover
    cinemachine.deadSlices = {2, 0}; // remover
    offset = Vec2(cinemachine.length.x, -cinemachine.length.y); // remover
}

void Camera::Unfollow () {
    focus = nullptr;
}

void Camera::Cinemachine::Accelerate (float focusVelocity, float displacement) {
    focusVelocity = fabs(focusVelocity);
    displacement = fabs(displacement);
    float acceleration = (focusVelocity * focusVelocity) / (2.0f * displacement);
    velocity.x += acceleration;
    if (velocity.x >= focusVelocity)
        velocity.x = 0.0f;
}

void Camera::Cinemachine::Decelerate (float focusVelocity, float displacement) {
    focusVelocity = fabs(focusVelocity);
    displacement = fabs(displacement);
    float acceleration = (focusVelocity * focusVelocity) / (2.0f * displacement);
    velocity.x += acceleration;
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

    // player is on center
    } else if (centerDistance < 0.2f) {
        velocity.x = 0.0f;
        offset.x = 0.0f;

    // player is on safe zone
    } else if (centerDistance < safeZone) {
        isLocked[HORIZONTAL] = false;
        Accelerate(playerVelocity, safeZone + slicedLength);
        offset.x = playerDirection * (centerDistance + velocity.x);

    // player is on dead zone
    } else if (centerDistance > safeZone) {
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
    offset = Vec2();

    cinemachine.length = Vec2();
    cinemachine.slices = {0, 0};
    cinemachine.deadSlices = {0, 0};
    screenOffset = Vec2();
}

Vec2 Camera::GetPosition () {
    return (pos + posAdjustment);
}
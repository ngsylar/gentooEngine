#include "GentooEngine.h"

CameraFollower::CameraFollower (GameObject& associated): Component(associated) {}

void CameraFollower::Update (float dt) {
    associated.box.x = Camera::pos.x + offset.x;
    associated.box.y = Camera::pos.y + offset.y;
}

void CameraFollower::Render () {}

bool CameraFollower::Is (std::string type) {
    return (type == "CameraFollower");
}
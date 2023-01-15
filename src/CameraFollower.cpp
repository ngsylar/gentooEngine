#include "GentooEngine.h"

CameraFollower::CameraFollower (GameObject& associated): Component(associated) {
    type = GameObjID::_CameraFollower;
}

void CameraFollower::LateUpdate (float dt) {
    associated.box.x = Camera::pos.x + offset.x;
    associated.box.y = Camera::pos.y + offset.y;
}

bool CameraFollower::Is (std::string type) {
    return (type == "CameraFollower");
}

bool CameraFollower::Is (GameObjID type) {
    return (type & this->type);
}
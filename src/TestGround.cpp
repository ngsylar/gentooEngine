#include "GentooEngine.h"
#include "TestObjects.h"

Ground::Ground (GameObject& associated, float relativeOffset): Component(associated) {
    // this->relativeOffset = relativeOffset;
    delay.SetResetTime(1.0f);
}

Ground::~Ground () {
    Camera::RemoveMethod(this);
}

void Ground::Start () {
    Camera::AddMethod(this, std::bind(&HandleCamera, this));
}

void* Ground::HandleCamera () {
    float cameraPosition = Camera::pos.y;
    // SDL_Log("camera: %f", cameraPosition);
    // SDL_Log("offset: %f", Camera::offset.y);

    if (cameraPosition > -11.0f) {
        // relativeOffset -= cameraPosition + 11.0f; // nota: parece que isso tbm eh desnecessario
        Camera::pos.y = -11.0f;
    }
    // nota: nao precisei restaurar Camera::offset, observar se isso causa algum problema futuramente

    return nullptr;
}
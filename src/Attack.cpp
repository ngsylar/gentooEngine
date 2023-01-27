#include "GentooEngine.h"
#include "Attack.h"

Attack::Attack (GameObject& associated): Component(associated) {}

void Attack::Awaken () {}

void Attack::Start () {}

void Attack::Update (float dt) {}

void Attack::Awaken () {}

void Attack::NotifyCollision (GameObject& other) {}
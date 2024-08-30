#include "engine/component/Component.h"

Component::Component(GO* gameobject) {
    this->gameobject = gameobject;
    this->enable = true;
}

Component::~Component() {}

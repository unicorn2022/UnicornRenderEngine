#include "engine/component/Component.h"

Component::Component(GO* gameobject) {
    this->gameobject = gameobject;
}

Component::~Component() {}

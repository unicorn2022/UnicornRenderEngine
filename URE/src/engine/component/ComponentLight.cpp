#include "engine/component/ComponentLight.h"

ComponentLight::ComponentLight(GO* gameobject, Light* light_data) : Component(gameobject) {
    this->type = "component_light";
    this->light_data = light_data;
}

ComponentLight::~ComponentLight() {
    delete light_data;
}
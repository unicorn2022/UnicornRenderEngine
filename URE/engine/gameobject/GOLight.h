#pragma once
#include "engine/gameobject/GO.h"
#include "engine/component/ALL.h"

class GOLight : public GO {
public:
    GOLight(std::string name, Light* light_data) : GO(name) {
        this->light_data = light_data;
        component_light = new ComponentLight(this, light_data);
    }

public:
    ComponentLight* component_light;
    Light* light_data;
};
#pragma once
#include "engine/gameobject/GO.h"

class GOLight : public GO {
public:
    GOLight(std::string name, Light* light_data) : GO(name) {
        this->light_data = light_data;
        AddComponent(new ComponentTransform(this));
        AddComponent(new ComponentLight(this, light_data));
    }
    virtual ~GOLight() {
        delete light_data;
    }

public:
    ComponentLight* component_light;
    Light* light_data;
};
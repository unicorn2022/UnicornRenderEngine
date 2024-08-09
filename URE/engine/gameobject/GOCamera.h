#pragma once
#include "engine/gameobject/GO.h"

class GOCamera : public GO {
public:
    GOCamera(std::string name) : GO(name) {
        AddComponent(new ComponentTransform(this));
        AddComponent(new ComponentCamera(this));
    }
};
#pragma once
#include "engine/gameobject/GO.h"
#include "engine/basic/Light.h"

class GOLight : public GO {
public:
    GOLight(std::string name, Light* light_data);
};
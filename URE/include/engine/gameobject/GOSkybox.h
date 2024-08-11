#pragma once
#include "engine/gameobject/GO.h"
#include "engine/material/Material.h"

class GOSkybox : public GO {
public:
    GOSkybox(std::string name, Material* material);
};
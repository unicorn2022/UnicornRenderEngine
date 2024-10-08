#pragma once
#include "engine/gameobject/GO.h"
#include "engine/material/Material.h"

class GOCube : public GO {
public:
    GOCube(std::string name, Material* material, int num = 1, bool is_debug = false);
};
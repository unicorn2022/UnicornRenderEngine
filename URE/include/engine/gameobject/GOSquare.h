#pragma once
#include "engine/gameobject/GO.h"
#include "engine/material/Material.h"

class GOSquare : public GO {
public:
    GOSquare(std::string name, Material* material, int num = 1, bool is_debug = false, bool is_transport = false);
};
#pragma once
#include "engine/gameobject/GO.h"
#include "engine/material/Material.h"

class GONanosuit : public GO {
public:
    GONanosuit(std::string name, std::string file_name, Material* material);
};
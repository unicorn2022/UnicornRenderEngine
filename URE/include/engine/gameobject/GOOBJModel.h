#pragma once
#include "engine/gameobject/GO.h"
#include "engine/material/Material.h"

class GOOBJModel : public GO {
public:
    GOOBJModel(std::string name, std::string file_name, Material* material);
};
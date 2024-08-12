#pragma once
#include "engine/gameobject/GO.h"
#include "engine/material/MaterialSkybox.h"
#include "engine/basic/TextureCube.h"

class GOSkybox : public GO {
public:
    GOSkybox(std::string name, MaterialSkybox* material);
    TextureCube* GetSkyboxTexture() const;

private:
    MaterialSkybox* material;
};
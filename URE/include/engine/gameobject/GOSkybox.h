#pragma once
#include "engine/gameobject/GO.h"
#include "engine/material/skybox/MaterialSkybox.h"
#include "engine/basic/TextureCube.h"

class GOSkybox : public GO {
public:
    GOSkybox(std::string name, MaterialSkybox* material);
    TextureCube* GetSkyboxTexture() const;
    void SetSkyboxTexture(TextureCube* texture_cube);

private:
    MaterialSkybox* material;
};
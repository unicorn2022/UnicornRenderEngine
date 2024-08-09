#pragma once
#include "engine/gameobject/GO.h"
#include "engine/material/Material.h"

class GOSkybox : public GO {
public:
    GOSkybox(std::string name, Material* material) : GO(name) {
        this->material = material;
        AddComponent(new ComponentTransform(this));
        AddComponent(new ComponentMesh(this, new MeshCube(), material));
    }

    virtual ~GOSkybox(){
        delete material;
    }

public:
    Material* material;
};
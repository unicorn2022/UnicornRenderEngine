#pragma once
#include "engine/gameobject/GO.h"
#include "engine/material/Material.h"

class GOCube : public GO {
public:
    GOCube(std::string name, Material* material) : GO(name) {
        this->material = material;
        AddComponent(new ComponentTransform(this));
        AddComponent(new ComponentMesh(this, new MeshCube(), material));
    }

    virtual ~GOCube() {
        delete material;
    }

public:
    Material* material;
};
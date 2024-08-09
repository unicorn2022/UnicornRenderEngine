#pragma once
#include "engine/gameobject/GO.h"
#include "engine/material/Material.h"

class GOSquare : public GO {
public:
    GOSquare(std::string name, Material* material, bool is_transport = false) : GO(name) {
        this->material = material;
        AddComponent(new ComponentTransform(this));
        AddComponent(new ComponentMesh(this, new MeshSquare(), material, is_transport));
    }

    virtual ~GOSquare() {
        delete material;
    }

public:
    Material* material;
};
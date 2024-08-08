#pragma once
#include "engine/gameobject/GO.h"
#include "engine/component/ALL.h"
#include "engine/material/Material.h"

class GOSquare : public GO {
public:
    GOSquare(std::string name, Material* material, bool is_transport = false) : GO(name) {
        this->material = material;
        component_transform = new ComponentTransform(this);
        component_mesh = new ComponentMesh(this, new MeshSquare(), material, is_transport);
    }

public:
    ComponentTransform* component_transform;
    ComponentMesh* component_mesh;
    Material* material;
};
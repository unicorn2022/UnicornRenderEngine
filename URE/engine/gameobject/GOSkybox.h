#pragma once
#include "engine/gameobject/GO.h"
#include "engine/component/ALL.h"
#include "engine/material/Material.h"

class GOSkybox : public GO {
public:
    GOSkybox(std::string name, Material* material) : GO(name) {
        this->material = material;
        component_transform = new ComponentTransform(this);
        component_mesh = new ComponentMesh(this, new MeshCube(), material);
    }

public:
    ComponentTransform* component_transform;
    ComponentMesh* component_mesh;
    Material* material;
};
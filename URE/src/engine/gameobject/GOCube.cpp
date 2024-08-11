#include "engine/gameobject/GOCube.h"
#include "engine/mesh/MeshCube.h"
#include "engine/component/ALL.h"

GOCube::GOCube(std::string name, Material* material) : GO(name) {
    AddComponent(new ComponentTransform(this));
    AddComponent(new ComponentMesh(this, new MeshCube(), material));
}


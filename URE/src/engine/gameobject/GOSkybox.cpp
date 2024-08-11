#include "engine/gameobject/GOSkybox.h"
#include "engine/mesh/MeshCube.h"
#include "engine/component/ALL.h"

GOSkybox::GOSkybox(std::string name, Material* material) : GO(name) {
    AddComponent(new ComponentTransform(this));
    AddComponent(new ComponentMesh(this, new MeshCube(), material));
}

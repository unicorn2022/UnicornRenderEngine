#include "engine/gameobject/GOSkybox.h"
#include "engine/mesh/MeshCube.h"
#include "engine/component/ALL.h"

GOSkybox::GOSkybox(std::string name, MaterialSkybox* material) : GO(name) {
    this->material = material;
    AddComponent(new ComponentTransform(this));
    AddComponent(new ComponentMesh(this, {new MeshCube()}, {material}));
}

TextureCube* GOSkybox::GetSkyboxTexture() const {
    return material->GetSkyboxTexture();
}

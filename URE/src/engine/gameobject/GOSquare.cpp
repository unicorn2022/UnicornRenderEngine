#include "engine/gameobject/GOSquare.h"
#include "engine/mesh/MeshSquare.h"
#include "engine/component/ALL.h"

GOSquare::GOSquare(std::string name, Material* material, bool is_transport) : GO(name) {
    AddComponent(new ComponentTransform(this));
    AddComponent(new ComponentMesh(this, new MeshSquare(), material, is_transport));
}

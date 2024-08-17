#include "engine/gameobject/GOCube.h"
#include "engine/mesh/MeshCube.h"
#include "engine/component/ALL.h"

GOCube::GOCube(std::string name, Material* material, int num, bool is_debug) : GO(name) {
    for (int i = 0; i < num; i++)
        AddComponent(new ComponentTransform(this));
    AddComponent(new ComponentMesh(this, {new MeshCube()}, {material}, is_debug));
}


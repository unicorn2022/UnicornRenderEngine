#include "engine/gameobject/GOSquare.h"
#include "engine/mesh/MeshSquare.h"
#include "engine/component/ALL.h"

GOSquare::GOSquare(std::string name, Material* material, int num, bool is_transport) : GO(name) {
    for (int i = 0; i < num; i++)
        AddComponent(new ComponentTransform(this));
    AddComponent(new ComponentMesh(this, {new MeshSquare()}, {material}, is_transport));
}

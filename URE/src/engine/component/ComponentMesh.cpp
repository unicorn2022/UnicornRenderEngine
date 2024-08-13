#include "engine/component/ComponentMesh.h"
#include "engine/gameobject/GO.h"
#include "engine/material/ALL.h"

ComponentMesh::ComponentMesh(GO* gameobject, std::vector<Mesh*> meshs, std::vector<Material*> materials, bool is_transport) : Component(gameobject) {
    this->type = "component_mesh";
    for (auto mesh : meshs) this->meshs.push_back(mesh);
    for (auto material : materials) this->materials.push_back(material);
    this->is_transport = is_transport;
}

ComponentMesh::~ComponentMesh() {
    for (auto mesh : meshs) delete mesh;
    Material* old_material = NULL;
    for (auto material : materials) 
        if (material != old_material) {
            delete material;
            old_material = material;
        }
    meshs.clear();
    materials.clear();
}

void ComponentMesh::Draw() {
    if (!enable) return;
    for (int i = 0; i < meshs.size(); i++)
        DrawOneMesh(meshs[i], materials[i]);
}

void ComponentMesh::DrawOneMesh(Mesh* mesh, Material* material) {
    /* 1. 设置变换信息 */
    material->model_transform = gameobject->GetComponent<ComponentTransform>()->GetModelMatrix();
    /* 2. 使用材质 */
    material->Use();
    /* 3. 绘制物体 */
    mesh->Draw();
}

bool ComponentMesh::IsTransport() const { 
    return is_transport; 
}
#include "engine/component/ComponentMesh.h"
#include "engine/gameobject/GO.h"
#include "engine/material/ALL.h"
#include "engine/mesh/MeshCube.h"
#include "engine/mesh/MeshSquare.h"

ComponentMesh::ComponentMesh(GO* gameobject, std::vector<Mesh*> meshs, std::vector<Material*> materials, bool is_transport) : Component(gameobject) {
    this->type = "component_mesh";
    for (auto mesh : meshs) this->meshs.push_back(mesh);
    for (auto material : materials) this->materials.push_back(material);
    this->is_transport = is_transport;

    // 生成实例化VBO
    num = gameobject->GetComponents<ComponentTransform>().size();
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, num * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
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
    glDeleteBuffers(1, &instanceVBO);
}

void ComponentMesh::Draw() {
    if (!enable) return;
    for (int i = 0; i < meshs.size(); i++)
        DrawOneMesh(meshs[i], materials[i]);
}

void ComponentMesh::Draw(Material* material) {
    if (!enable) return;
    for (int i = 0; i < meshs.size(); i++)
        DrawOneMesh(meshs[i], material);
}

void ComponentMesh::DrawOneMesh(Mesh* mesh, Material* material) {
    // std::cout << "DrawOneMesh\n";
    /* 2. 使用材质 */
    material->Use();
    /* 3. 绘制物体 */
    // 3.1 获取 model 矩阵
    auto transforms = gameobject->GetComponents<ComponentTransform>();
    std::vector<glm::mat4> model_transforms;
    for (auto transform : transforms) 
        model_transforms.push_back(transform->GetModelMatrix());
    // 3.2 配置实例化VBO
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, num * sizeof(glm::mat4), &model_transforms[0], GL_STATIC_DRAW);
    // 3.3 绘制物体
    mesh->Draw(num);
}

bool ComponentMesh::IsTransport() const { 
    return is_transport; 
}

bool ComponentMesh::IsBasicShape() const { 
    if (meshs.size() > 1) return false;
    auto mesh = meshs[0];
    if (dynamic_cast<MeshCube*>(mesh) != NULL) return true;
    if (dynamic_cast<MeshSquare*>(mesh) != NULL) return true;
    return false;
}
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

void ComponentMesh::Draw(Camera* camera, std::vector<ComponentLight*> &lights) {
    if (!enable) return;
    for (int i = 0; i < meshs.size(); i++)
        DrawOneMesh(camera, lights, meshs[i], materials[i]);
}

void ComponentMesh::DrawOneMesh(Camera* camera, std::vector<ComponentLight*> &lights, Mesh* mesh, Material* material) {
    /* 使用材质 */
    // 固定颜色
    if (dynamic_cast<MaterialConstantColor*>(material) != NULL) {
        MaterialConstantColor* mat = dynamic_cast<MaterialConstantColor*>(material);
        /* 变换信息 */
        mat->model_transform = gameobject->GetComponent<ComponentTransform>()->GetModelMatrix();
        mat->view_transform = camera->GetViewMatrix();
        mat->projection_transform = camera->GetProjectionMatrix();
        /* 使用材质 */
        mat->Use();
    }
    // 深度
    else if (dynamic_cast<MaterialDepth*>(material) != NULL) {
        MaterialDepth* mat = dynamic_cast<MaterialDepth*>(material);
        /* 变换信息 */
        mat->model_transform = gameobject->GetComponent<ComponentTransform>()->GetModelMatrix();
        mat->view_transform = camera->GetViewMatrix();
        mat->projection_transform = camera->GetProjectionMatrix();
        /* 使用材质 */
        mat->Use();
    }
    // 无光照
    else if (dynamic_cast<MaterialNoLight*>(material) != NULL) {
        MaterialNoLight* mat = dynamic_cast<MaterialNoLight*>(material);
        /* 变换信息 */
        mat->model_transform = gameobject->GetComponent<ComponentTransform>()->GetModelMatrix();
        mat->view_transform = camera->GetViewMatrix();
        mat->projection_transform = camera->GetProjectionMatrix();
        /* 使用材质 */
        mat->Use();
    }
    // Phong光照
    else if (dynamic_cast<MaterialPhongLight*>(material) != NULL) {
        MaterialPhongLight* mat = dynamic_cast<MaterialPhongLight*>(material);
        /* 变换信息 */
        mat->model_transform = gameobject->GetComponent<ComponentTransform>()->GetModelMatrix();
        mat->view_transform = camera->GetViewMatrix();
        mat->projection_transform = camera->GetProjectionMatrix();
        /* 光照信息 */
        int point_light_cnt = 0;
        for (auto light_component : lights) {
            if (dynamic_cast<DirectLight*>(light_component->light_data) != NULL) {
                mat->direct_light = dynamic_cast<DirectLight*>(light_component->light_data);
            } else if (dynamic_cast<PointLight*>(light_component->light_data) != NULL) {
                mat->point_lights[point_light_cnt++] = dynamic_cast<PointLight*>(light_component->light_data);
            }else if (dynamic_cast<SpotLight*>(light_component->light_data) != NULL) {
                mat->spot_light = dynamic_cast<SpotLight*>(light_component->light_data);
            }
        }
        /* 观察位置 */
        mat->view_position = camera->position;
        /* 使用材质 */
        mat->Use();
    }
    // 天空盒
    else if (dynamic_cast<MaterialSkybox*>(material) != NULL) {
        MaterialSkybox* mat = dynamic_cast<MaterialSkybox*>(material);
        /* 变换信息 */
        mat->view_transform = camera->GetViewMatrix();
        mat->projection_transform = camera->GetProjectionMatrix();
        /* 使用材质 */
        mat->Use();
    }
    // 反射天空盒
    else if (dynamic_cast<MaterialSkyboxReflect*>(material) != NULL) {
        MaterialSkyboxReflect* mat = dynamic_cast<MaterialSkyboxReflect*>(material);
        /* 变换信息 */
        mat->model_transform = gameobject->GetComponent<ComponentTransform>()->GetModelMatrix();
        mat->view_transform = camera->GetViewMatrix();
        mat->projection_transform = camera->GetProjectionMatrix();
        /* 观察位置 */
        mat->view_position = camera->position;
        /* 使用材质 */
        mat->Use();
    }
    // 折射天空盒
    else if (dynamic_cast<MaterialSkyboxRefract*>(material) != NULL) {
        MaterialSkyboxRefract* mat = dynamic_cast<MaterialSkyboxRefract*>(material);
        /* 变换信息 */
        mat->model_transform = gameobject->GetComponent<ComponentTransform>()->GetModelMatrix();
        mat->view_transform = camera->GetViewMatrix();
        mat->projection_transform = camera->GetProjectionMatrix();
        /* 观察位置 */
        mat->view_position = camera->position;
        /* 使用材质 */
        mat->Use();
    }

    /* 绘制物体 */
    mesh->Draw();
}

bool ComponentMesh::IsTransport() const { 
    return is_transport; 
}
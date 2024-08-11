#include "engine/component/ComponentMesh.h"
#include "engine/gameobject/GO.h"
#include "engine/material/ALL.h"

ComponentMesh::ComponentMesh(GO* gameobject, Mesh* mesh, Material* material, bool is_transport) : Component(gameobject) {
    this->type = "component_mesh";
    this->mesh = mesh;
    this->material = material;
    this->is_transport = is_transport;
}
ComponentMesh::~ComponentMesh() {
    delete mesh;
    delete material;
}

void ComponentMesh::Draw(Camera* camera, std::vector<ComponentLight*> lights) {
    /* 使用材质 */
    // 固定颜色
    if (dynamic_cast<MaterialConstantColor*>(material) != NULL) {
        MaterialConstantColor* mat = dynamic_cast<MaterialConstantColor*>(material);
        /* 变换信息 */
        mat->model_transform = gameobject->GetComponent<ComponentTransform>()->GetModelMatrix();
        mat->view_transform = camera->GetViewMatrix();
        mat->projection_transform = camera->GetProjectionMatrix();
        mat->Use();
    }
    // 深度
    else if (dynamic_cast<MaterialDepth*>(material) != NULL) {
        MaterialDepth* mat = dynamic_cast<MaterialDepth*>(material);
        /* 变换信息 */
        mat->model_transform = gameobject->GetComponent<ComponentTransform>()->GetModelMatrix();
        mat->view_transform = camera->GetViewMatrix();
        mat->projection_transform = camera->GetProjectionMatrix();
        mat->Use();
    }
    // 无光照
    else if (dynamic_cast<MaterialNoLight*>(material) != NULL) {
        MaterialNoLight* mat = dynamic_cast<MaterialNoLight*>(material);
        /* 变换信息 */
        mat->model_transform = gameobject->GetComponent<ComponentTransform>()->GetModelMatrix();
        mat->view_transform = camera->GetViewMatrix();
        mat->projection_transform = camera->GetProjectionMatrix();
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
        mat->Use();
    }
    // 天空盒
    else if (dynamic_cast<MaterialSkybox*>(material) != NULL) {
        MaterialSkybox* mat = dynamic_cast<MaterialSkybox*>(material);
        /* 变换信息 */
        mat->view_transform = camera->GetViewMatrix();
        mat->projection_transform = camera->GetProjectionMatrix();
        mat->Use();
    }

    /* 绘制物体 */
    mesh->Draw();
}

bool ComponentMesh::IsTransport() const { 
    return is_transport; 
}
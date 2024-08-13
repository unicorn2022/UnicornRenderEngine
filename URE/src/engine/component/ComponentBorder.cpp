#include "engine/component/ComponentBorder.h"
#include "engine/material/MaterialVisualizationBorder.h"
#include "engine/material/MaterialConstantColor.h"
#include "engine/mesh/MeshCube.h"
#include "engine/mesh/MeshSquare.h"
#include "engine/gameobject/GO.h"

ComponentBorder::ComponentBorder(GO* gameobject, ComponentMesh* component_mesh, glm::vec4 border_color) : Component(gameobject) {
    this->component_mesh = component_mesh;
    this->is_basic_shape = component_mesh->IsBasicShape();
    if (is_basic_shape) {
        this->material = new MaterialConstantColor(border_color);
    } else {
        this->material = new MaterialVisualizationBorder(border_color);
    }
}

ComponentBorder::~ComponentBorder() {
    delete material;
}

void ComponentBorder::Draw() {
    if (is_basic_shape) {
        gameobject->GetComponent<ComponentTransform>()->TransformScale(glm::vec3(1.1f));
        component_mesh->Draw(material);
        gameobject->GetComponent<ComponentTransform>()->TransformScale(glm::vec3(1.0f / 1.1f));
    } else {
        component_mesh->Draw(material);
    }
}
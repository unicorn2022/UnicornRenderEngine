#include "engine/gameobject/GOCapture2D.h"
#include "engine/component/ALL.h"
#include "engine/mesh/MeshCube.h"
#include "engine/material/MaterialConstantColor.h"

GOCapture2D::GOCapture2D(std::string name, float fov, float near, float far, int width, int height) : GO(name) {
    AddComponent(new ComponentTransform(this));
    AddComponent(new ComponentCamera(this, fov, near, far, width, height));
    AddComponent(new ComponentMesh(this, {new MeshCube()}, {new MaterialConstantColor(glm::vec4(0.2, 0.2, 0.2, 1.0))}));
    GetComponent<ComponentTransform>()->TransformScale(glm::vec3(0.2, 0.1, 0.1));
}

Texture* GOCapture2D::GetCaptureTexture() const {
    return GetComponent<ComponentCamera>()->frame_buffer->color_texture;
}

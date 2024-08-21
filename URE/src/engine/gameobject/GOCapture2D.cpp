#include "engine/gameobject/GOCapture2D.h"
#include "engine/component/ALL.h"
#include "engine/mesh/MeshCube.h"
#include "engine/material/MaterialConstantColor.h"

GOCapture2D::GOCapture2D(std::string name, float fov, float near, float far, int width, int height, int samples) : GO(name) {
    AddComponent(new ComponentTransform(this));
    AddComponent(new ComponentCamera(this, fov, near, far, width, height, samples));
    AddComponent(new ComponentMesh(this, {new MeshCube()}, {new MaterialConstantColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f))}, true));
    GetComponents<ComponentTransform>()[0]->TransformScale(glm::vec3(0.5, 0.2, 0.2));
}

Texture* GOCapture2D::GetCaptureTexture() const {
    return GetComponents<ComponentCamera>()[0]->frame_buffer->color_texture;
}

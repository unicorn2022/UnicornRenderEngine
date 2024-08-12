#include "engine/gameobject/GOCamera.h"
#include "engine/component/ALL.h"

GOCamera::GOCamera(std::string name, float fov, float near, float far, int width, int height) : GO(name) {
    AddComponent(new ComponentTransform(this));
    AddComponent(new ComponentCamera(this, fov, near, far, width, height));
}
#include "engine/gameobject/GOCamera.h"
#include "engine/component/ALL.h"

GOCamera::GOCamera(std::string name, float aspect, float fov, float near, float far) : GO(name) {
    AddComponent(new ComponentTransform(this));
    AddComponent(new ComponentCamera(this, aspect, fov, near, far));
}
#include "engine/gameobject/GOShadow.h"
#include "engine/component/ALL.h"

GOShadowDirectLight::GOShadowDirectLight(std::string name, DirectLight* direct_light, glm::mat4* light_matrix, int* shadow_map_index, int width, int height, int samples, float near, float far, float left, float right, float bottom, float top) : GO(name) {
    AddComponent(new ComponentTransform(this));
    AddComponent(new ComponentShadowDirectLight(this, direct_light, light_matrix, shadow_map_index, width, height, samples, near, far, left, right, bottom, top));
}
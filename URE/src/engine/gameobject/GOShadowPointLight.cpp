#include "engine/gameobject/GOShadowPointLight.h"
#include "engine/component/ALL.h"

GOShadowPointLight::GOShadowPointLight(std::string name, PointLight* point_light, int* shadow_map_index, int width, int height, float near, float far) : GO(name) {
    AddComponent(new ComponentTransform(this));
    AddComponent(new ComponentShadowPointLight(this, point_light, shadow_map_index, width, height, near, far));
}
#include "engine/gameobject/GOLight.h"
#include "engine/component/ALL.h"

GOLight::GOLight(std::string name, Light* light_data) : GO(name) {
    AddComponent(new ComponentTransform(this));
    AddComponent(new ComponentLight(this, light_data));
}
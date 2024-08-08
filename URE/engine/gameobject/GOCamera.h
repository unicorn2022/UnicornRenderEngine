#pragma once
#include "engine/gameobject/GO.h"
#include "engine/component/ALL.h"

class GOCamera : public GO {
public:
    GOCamera(std::string name) : GO(name) {
        component_transform = new ComponentTransform(this);
        component_transform->TransformTranslate(main_camera_position);
        component_camera = new ComponentCamera(this);
    }

public:
    ComponentTransform* component_transform;
    ComponentCamera* component_camera;
};
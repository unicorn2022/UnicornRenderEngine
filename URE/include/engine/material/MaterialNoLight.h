#pragma once
#include "engine/material/Material.h"
#include "engine/basic/Texture.h"

class MaterialNoLight : public Material {
public:
    /** 材质: 无光照
     * \param diffuse 漫反射贴图
     */
    MaterialNoLight(Texture* diffuse);

    ~MaterialNoLight();
public:
    virtual void Use();

public:
    /* 变换信息 */
    glm::mat4 model_transform;
    glm::mat4 view_transform;
    glm::mat4 projection_transform;

private:
    /* 材质信息 */
    Texture* diffuse;
};
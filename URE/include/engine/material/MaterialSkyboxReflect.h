#pragma once
#include "engine/material/Material.h"
#include "engine/basic/TextureCube.h"

class MaterialSkyboxReflect : public Material {
public:
    /** 材质: 无光照
     * \param skybox 天空盒贴图
     */
    MaterialSkyboxReflect(TextureCube* skybox);

    ~MaterialSkyboxReflect();
public:
    virtual void Use();

public:
    /* 变换信息 */
    glm::mat4 model_transform;
    glm::mat4 view_transform;
    glm::mat4 projection_transform;
    /* 观察位置 */
    glm::vec3 view_position;

private:
    /* 材质信息 */
    TextureCube* skybox;
};
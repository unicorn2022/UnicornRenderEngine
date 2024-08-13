#pragma once
#include "engine/material/Material.h"
#include "engine/basic/TextureCube.h"

class MaterialSkyboxRefract : public Material {
public:
    /** 材质: 无光照
     * \param skybox 天空盒贴图
     */
    MaterialSkyboxRefract(TextureCube* skybox);

    ~MaterialSkyboxRefract();
public:
    virtual void Use();

public:
    /* 变换信息 */
    glm::mat4 model_transform;

private:
    /* 材质信息 */
    TextureCube* skybox;
};
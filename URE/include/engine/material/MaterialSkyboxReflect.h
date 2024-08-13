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

private:
    /* 材质信息 */
    TextureCube* skybox;
};
#pragma once
#include "engine/material/Material.h"
#include "engine/basic/TextureCube.h"

class MaterialSkybox : public Material {
public:
    /** 材质: 天空盒
     * \param skybox_texture 天空盒纹理
     */
    MaterialSkybox(TextureCube* skybox_texture);

    ~MaterialSkybox();

public:
    virtual void Use();
    TextureCube* GetSkyboxTexture() const;

private:
    /* 材质信息 */
    TextureCube* skybox_texture;
};
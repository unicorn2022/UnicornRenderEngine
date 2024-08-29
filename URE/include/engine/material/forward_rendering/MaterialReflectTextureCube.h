#pragma once
#include "engine/material/Material.h"
#include "engine/basic/TextureCube.h"

class MaterialReflectTextureCube : public Material {
public:
    /** 材质: 反射立方纹理
     * \param texture_cube 立方纹理
     */
    MaterialReflectTextureCube(TextureCube* texture_cube);

    ~MaterialReflectTextureCube();
public:
    virtual void Use();

private:
    /* 材质信息 */
    TextureCube* texture_cube;
};
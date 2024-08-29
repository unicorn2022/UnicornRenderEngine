#pragma once
#include "engine/material/Material.h"
#include "engine/basic/TextureCube.h"

class MaterialRefractTextureCube : public Material {
public:
    /** 材质: 折射立方纹理
     * \param texture_cube 立方纹理
     */
    MaterialRefractTextureCube(TextureCube* texture_cube);

    ~MaterialRefractTextureCube();
public:
    virtual void Use();

private:
    /* 材质信息 */
    TextureCube* texture_cube;
};
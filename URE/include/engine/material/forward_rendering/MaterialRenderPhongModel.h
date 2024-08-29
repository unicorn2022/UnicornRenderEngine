#pragma once
#include "engine/material/Material.h"
#include "engine/basic/Texture.h"
#include "engine/basic/Light.h"

class MaterialRenderPhongModel : public Material {
public:
    /** 材质: Phong光照模型
     * \param diffuse 漫反射贴图
     * \param specular 高光度贴图
     * \param shininess 高光项
     */
    MaterialRenderPhongModel(Texture* diffuse, Texture* specular, float shininess = 32.0f);

    ~MaterialRenderPhongModel();

public:
    virtual void Use();

private:
    /* 材质信息 */
    Texture* diffuse;
    Texture* specular;
    float shininess;

    /* 光照模型信息 */
    int use_blinn_phong;
    int show_render_shadow;
};
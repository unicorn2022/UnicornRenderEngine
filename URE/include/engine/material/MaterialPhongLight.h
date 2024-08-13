#pragma once
#include "engine/material/Material.h"
#include "engine/basic/Texture.h"
#include "engine/basic/Light.h"

class MaterialPhongLight : public Material {
public:
    /** 材质: Phong光照
     * \param diffuse 漫反射贴图
     * \param specular 高光度贴图
     * \param shininess 高光项
     */
    MaterialPhongLight(Texture* diffuse, Texture* specular, float shininess = 32.0f);

    ~MaterialPhongLight();

public:
    virtual void Use();

private:
    /* 材质信息 */
    Texture* diffuse;
    Texture* specular;
    float shininess;
};
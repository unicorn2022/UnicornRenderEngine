#pragma once
#include "engine/material/Material.h"
#include "engine/basic/Texture.h"
#include "engine/basic/Light.h"

static const int MAX_POINT_LIGHT_COUNT = 4;

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
    
public:
    /* 变换信息 */
    glm::mat4 model_transform;
    /* 光照信息 */
    DirectLight* direct_light;
    PointLight* point_lights[MAX_POINT_LIGHT_COUNT];
    SpotLight* spot_light;

private:
    /* 材质信息 */
    Texture* diffuse;
    Texture* specular;
    float shininess;
};
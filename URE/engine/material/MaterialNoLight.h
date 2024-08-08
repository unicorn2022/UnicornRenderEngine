#pragma once
#include "engine/material/Material.h"
#include "engine/basic/Texture.h"

class MaterialNoLight : public Material {
public:
    /** 材质: 无光照
     * \param diffuse 漫反射贴图
     * \param specular 高光度贴图
     * \param shininess 高光项
     */
    MaterialNoLight(Texture* diffuse, Texture* specular, float shininess = 32.0f) {
        this->shader = new Shader("mesh_no_light");
        this->diffuse = diffuse;
        this->specular = specular;
        this->shininess = shininess;
    }

    ~MaterialNoLight() {
        delete diffuse;
        delete specular;
    }

public:
    virtual void Use() {
        shader->Use();
        shader->SetUniform("model_transform", model_transform);
        shader->SetUniform("view_transform", view_transform);
        shader->SetUniform("projection_transform", projection_transform);

        if (diffuse != NULL) {
            diffuse->Use(0);
            shader->SetUniform("material.diffuse", 0);
        }
        if (specular != NULL) {
            specular->Use(1);
            shader->SetUniform("material.specular", 1);
        }
        shader->SetUniform("material.shininess", shininess);
    }

public:
    /* 变换信息 */
    glm::mat4 model_transform;
    glm::mat4 view_transform;
    glm::mat4 projection_transform;

private:
    /* 材质信息 */
    Texture* diffuse;
    Texture* specular;
    float shininess;
};
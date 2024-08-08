#pragma once
#include "engine/material/Material.h"
#include "engine/basic/TextureCube.h"

class MaterialSkybox : public Material {
public:
    /** 材质: 天空盒
     * \param skybox_texture 天空盒纹理
     */
    MaterialSkybox(TextureCube* skybox_texture) {
        this->shader = new Shader("skybox");
        this->skybox_texture = skybox_texture;
    }

    ~MaterialSkybox() {
        delete skybox_texture;
    }

public:
    virtual void Use() {
        shader->Use();
        shader->SetUniform("view_transform", view_transform);
        shader->SetUniform("projection_transform", projection_transform);
        if (skybox_texture != NULL) {
            skybox_texture->Use(0);
            shader->SetUniform("skybox_texture", 0);
        }
    }

public:
    /* 变换信息 */
    glm::mat4 view_transform;
    glm::mat4 projection_transform;
private:
    /* 材质信息 */
    TextureCube* skybox_texture;
};
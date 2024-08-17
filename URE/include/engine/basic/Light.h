#pragma once
#include "Utils.h"

class Light {
public:
    /** 光源
     * \param ambient 环境光颜色
     * \param diffuse 漫反射光颜色
     * \param specular 高光颜色
     */
    Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    virtual ~Light();

public:
    // 光源颜色
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};


class DirectLight : public Light {
public:
    /** 方向光源
     * \param direction 光源方向
     * \param ambient 环境光颜色
     * \param diffuse 漫反射光颜色
     * \param specular 高光颜色
     */
    DirectLight(glm::vec3 direction = glm::vec3(1), glm::vec3 ambient = glm::vec3(0), glm::vec3 diffuse = glm::vec3(0), glm::vec3 specular = glm::vec3(0));
    virtual ~DirectLight();

public:
    // 光源属性
    glm::vec3 direction;
};


class PointLight : public Light {
public:
    /** 点光源
     * \param position 光源位置
     * \param ambient 环境光颜色
     * \param diffuse 漫反射光颜色
     * \param specular 高光颜色
     */
    PointLight(glm::vec3 position = glm::vec3(0), glm::vec3 ambient = glm::vec3(0), glm::vec3 diffuse = glm::vec3(0), glm::vec3 specular = glm::vec3(0));
    virtual ~PointLight();

public:
    // 光源属性
    glm::vec3 position;
    // 衰减
    float constant;
    float linear;
    float quadratic;
};

class SpotLight : public Light {
public:
    /** 聚光源
     * \param position 光源位置
     * \param direction 光源方向
     * \param ambient 环境光颜色
     * \param diffuse 漫反射光颜色
     * \param specular 高光颜色
     * \param inner_cut_off 内切角(角度)
     * \param outer_cut_off 外切角(角度)
     */
    SpotLight(glm::vec3 position = glm::vec3(0), glm::vec3 direction = glm::vec3(1), glm::vec3 ambient = glm::vec3(0), glm::vec3 diffuse = glm::vec3(0), glm::vec3 specular = glm::vec3(0), float inner_cut_off = 12.5f, float outer_cut_off = 15.0f);
    virtual ~SpotLight();

public:
    // 光源属性
    glm::vec3 position;
    glm::vec3 direction;
    float inner_cut_off; // 内切角(角度)
    float outer_cut_off; // 外切角(角度)   
    // 衰减
    float constant;
    float linear;
    float quadratic;
};
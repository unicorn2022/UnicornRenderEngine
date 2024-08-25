#pragma once
#include "Utils.h"
#include "engine/basic/Light.h"

static const int MAX_DIRECT_LIGHT_COUNT = 2;
static const int MAX_POINT_LIGHT_COUNT = 4;
static const int MAX_SPOT_LIGHT_COUNT = 2;
static const int N = 4;
static const float POINT_LIGHT_SHADOW_ZFAR = 100.0f;

class UniformBuffer {
public:
    /** Uniform缓冲对象, 用于向所有shader传输公用uniform变量
     * \param binding 绑定点
     * \param size uniform缓冲块大小
     */
    UniformBuffer(int binding, int size);
    ~UniformBuffer();

public:
    /* 更新 uniform 数据 */
    virtual void UpdateUniformData() = 0;

protected:
    /* uniform 缓冲块 ID */
    unsigned int ID;
    /* uniform 缓冲块大小 */
    unsigned int size;

public:
    int binding; // 绑定点

private:
    void CreateUniformBuffer();
};

class UniformBufferCamera : public UniformBuffer {
public:
    /* Camera对应的帧缓冲对象 */
    static UniformBufferCamera& GetInstance() {
        static UniformBufferCamera instance;
        return instance;
    }

    /* 更新 uniform 数据 */
    virtual void UpdateUniformData(); 

private:
    UniformBufferCamera() : UniformBuffer(0, 2 * sizeof(glm::mat4) + sizeof(glm::vec3)) {}

public:
    glm::mat4 view_transform;
    glm::mat4 projection_transform;
    glm::vec3 view_position;
};

class UniformBufferLight : public UniformBuffer {
public:
    /* Light 对应的帧缓冲对象 */
    static UniformBufferLight& GetInstance() {
        static UniformBufferLight instance;
        return instance;
    }

    /* 更新 uniform 数据 */
    virtual void UpdateUniformData(); 

private:
    UniformBufferLight() : UniformBuffer(1, (16 * MAX_DIRECT_LIGHT_COUNT + 20 * MAX_POINT_LIGHT_COUNT + 24 * MAX_SPOT_LIGHT_COUNT + 3) * N) {}

public:
    // 1. 光源数据
    DirectLight direct_light[MAX_DIRECT_LIGHT_COUNT];
    PointLight point_lights[MAX_POINT_LIGHT_COUNT];
    SpotLight spot_light[MAX_SPOT_LIGHT_COUNT];
    // 2. 使用的光源个数
    int use_direct_light_num;   // 使用的方向光源个数
    int use_point_light_num;    // 使用的点光源个数, !!!至少为1个!!!
    int use_spot_light_num;     // 使用的聚光源个数
};

class UniformBufferShadow : public UniformBuffer {
public:
    /* Shadow 对应的帧缓冲对象 */
    static UniformBufferShadow& GetInstance() {
        static UniformBufferShadow instance;
        return instance;
    }

    /* 更新 uniform 数据 */
    virtual void UpdateUniformData(); 

private:
    UniformBufferShadow() : UniformBuffer(2, sizeof(direct_light_matrix) + N) {}

public:
    glm::mat4 direct_light_matrix[MAX_DIRECT_LIGHT_COUNT];

private:
    int use_direct_light_num;
    
public:
    // 阴影贴图索引
    int direct_light_shadow_map_index[MAX_DIRECT_LIGHT_COUNT];
    int point_light_shadow_map_index[MAX_POINT_LIGHT_COUNT];
};
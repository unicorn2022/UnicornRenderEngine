#pragma once
#include "Utils.h"
#include "engine/basic/Light.h"

static const int MAX_POINT_LIGHT_COUNT = 4;

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
    UniformBufferLight() : UniformBuffer(1, (16 + 20 * MAX_POINT_LIGHT_COUNT + 24 + 1) * 4) {}

public:
    DirectLight direct_light;
    PointLight point_lights[MAX_POINT_LIGHT_COUNT];
    SpotLight spot_light;
    int use_point_light_num;
};
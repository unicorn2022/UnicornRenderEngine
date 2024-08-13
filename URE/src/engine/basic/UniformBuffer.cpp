#include "engine/basic/UniformBuffer.h"

static const int N = 4;

UniformBuffer::UniformBuffer(int binding, int size) {
    this->binding = binding;
    this->size = size;
    CreateUniformBuffer();
}

UniformBuffer::~UniformBuffer(){
    glDeleteBuffers(1, &ID);
}

void UniformBuffer::CreateUniformBuffer() {
    // 1. 生成缓冲对象
    glGenBuffers(1, &ID);
    glBindBuffer(GL_UNIFORM_BUFFER, ID);
    glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    // 2. 绑定到绑定点上
    glBindBufferRange(GL_UNIFORM_BUFFER, binding, ID, 0, size);
}

void UniformBufferCamera::UpdateUniformData() {
    glBindBuffer(GL_UNIFORM_BUFFER, ID);
    unsigned int offset = 0;
    // 1. view_transform
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(view_transform), glm::value_ptr(view_transform));
    offset += sizeof(view_transform);
    // 2. projection_transform
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(projection_transform), glm::value_ptr(projection_transform));
    offset += sizeof(projection_transform);
    // 3. view_position
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(view_position), glm::value_ptr(view_position));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBufferLight::UpdateUniformData() {
    glBindBuffer(GL_UNIFORM_BUFFER, ID);
    unsigned int offset = 0;
    /* 1. direct_light */
    {
        // 1.1 ambient
        glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(direct_light.ambient), glm::value_ptr(direct_light.ambient));
        offset += 4 * N;
        // 1.2 diffuse
        glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(direct_light.diffuse), glm::value_ptr(direct_light.diffuse));
        offset += 4 * N;
        // 1.3 specular
        glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(direct_light.specular), glm::value_ptr(direct_light.specular));
        offset += 4 * N;
        // 1.4 direction
        glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(direct_light.direction), glm::value_ptr(direct_light.direction));
        offset += 4 * N;
    }

    /* 2. point_lights */
    for (int i = 0; i < MAX_POINT_LIGHT_COUNT; i++) {
        // 2.1 ambient
        glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(point_lights[i].ambient), glm::value_ptr(point_lights[i].ambient));
        offset += 4 * N;
        // 2.2 diffuse
        glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(point_lights[i].diffuse), glm::value_ptr(point_lights[i].diffuse));
        offset += 4 * N;
        // 2.3 specular
        glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(point_lights[i].specular), glm::value_ptr(point_lights[i].specular));
        offset += 4 * N;
        // 2.4 position
        glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(point_lights[i].position), glm::value_ptr(point_lights[i].position));
        offset += 4 * N;
        // 2.5 constant
        glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(point_lights[i].constant), &point_lights[i].constant);
        offset += N;
        // 2.6 linear
        glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(point_lights[i].linear), &point_lights[i].linear);
        offset += N;
        // 2.7 quadratic
        glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(point_lights[i].quadratic), &point_lights[i].quadratic);
        offset += N;
        // 2.8 padding
        offset += N;
    }
    /* 3. spot_light */
    {
        // 3.1 ambient
        glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(spot_light.ambient), glm::value_ptr(spot_light.ambient));
        offset += 4 * N;
        // 3.2 diffuse
        glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(spot_light.diffuse), glm::value_ptr(spot_light.diffuse));
        offset += 4 * N;
        // 3.3 specular
        glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(spot_light.specular), glm::value_ptr(spot_light.specular));
        offset += 4 * N;
        // 3.4 position
        glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(spot_light.position), glm::value_ptr(spot_light.position));
        offset += 4 * N;
        // 3.5 direction
        glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(spot_light.direction), glm::value_ptr(spot_light.direction));
        offset += 3 * N;
        // 3.7 inner_cut_off
        glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(spot_light.inner_cut_off), &spot_light.inner_cut_off);
        offset += N;
        // 3.8 outer_cut_off
        glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(spot_light.outer_cut_off), &spot_light.outer_cut_off);
        offset += N;
        // 3.9 constant
        glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(spot_light.constant), &spot_light.constant);
        offset += N;
        // 3.10 linear
        glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(spot_light.linear), &spot_light.linear);
        offset += N;
        // 3.11 quadratic
        glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(spot_light.quadratic), &spot_light.quadratic);
        offset += N;
    }

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

#include "engine/basic/UniformBuffer.h"

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

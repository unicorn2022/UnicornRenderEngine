#pragma once
/* 标准库 */
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

/* OpenGL 库 */
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/* ASSIMP 模型加载 */
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

/* 移动方向 */
enum MovementDirection {
    FORWARD,    // 前
    BACKWARD,   // 后
    LEFT,       // 左
    RIGHT,      // 右
    UP,         // 上
    DOWN,       // 下
    NONE        // 无
};

/* 读写文件相关 */
namespace Utils {
    /**
     * 从文件中读取内容
     * \param path 文件路径
     * \return 读取到的内容
     */
    std::string ReadFile(std::string path);

    /**
     * 从文件中读取图片
     * \param path 文件路径
     * \return 读取到的stbi图片
     */
    unsigned char* ReadPicture(std::string path, int& width, int& height, int& channel);

    /**
     * 读取图片之后释放内存
     * \param data stbi_image读取到的图片数据
     */
    void FreePicture(unsigned char* data);
}

/* 调试相关 */
namespace Utils {
    void Debug(std::string msg, glm::vec3 data);
    void Debug(std::string msg, glm::vec4 data);
    void Debug(std::string msg, glm::mat4 data);
    /* 判断当前OpenGL调用是否存在问题 */
    void Check(std::string msg);
}

/* 颜色相关 */
namespace Utils{
    namespace Color {
        const glm::vec4 black   = glm::vec4(0, 0, 0, 1.0f);     // 黑
        const glm::vec4 red     =   glm::vec4(1, 0, 0, 1.0f);   // 红
        const glm::vec4 green   = glm::vec4(0, 1, 0, 1.0f);     // 绿
        const glm::vec4 blue    =  glm::vec4(0, 0, 1, 1.0f);    // 蓝
        const glm::vec4 yellow  = glm::vec4(1, 1, 0, 1.0f);     // 黄
        const glm::vec4 purple  = glm::vec4(1, 0, 1, 1.0f);     // 紫
        const glm::vec4 cyan    = glm::vec4(0, 1, 1, 1.0f);     // 青
        const glm::vec4 white   = glm::vec4(1, 1, 1, 1.0f);     // 白
        const glm::vec4 gray =  glm::vec4(0.5, 0.5, 0.5, 1.0f); // 灰
        const glm::vec4 blue_tianyi = glm::vec4(0.4, 0.8, 1, 1.0f); // 天依蓝
    }
}

/* 数学相关 */
namespace Utils {
    const float PI = 3.1415926535;

    /* 生成 [L, R] 的随机数 */
    int Random(int L, int R);

    /* 生成 [L, R] 的随机数 */
    float Random(float L, float R);

    /* 将 (1, 0, 0), 旋转 (x, y, z) 后, 结果为 direction */
    glm::vec3 Convert_Direction_To_Euler(glm::vec3 direction);
    
    /* 判断两个向量是否共线 */
    bool IsSameDirection(glm::vec3 dir1, glm::vec3 dir2);
}

/* 其他模板工具函数 */
namespace Utils {
    /** 从一个vector中删除一个item
     * \param vec 待删除元素的vector
     * \param item 待删除元素
     * \return 原vec中是否存在item
     */
    template<typename T>
    bool RemoveItemInVector(std::vector<T> vec, T item) {
        int index = 0;
        bool exist = false;
        for(; index < vec.size(); index++)
            if (item == vec[index]) {
                exist = true;
                break;
            }
        for (; index < vec.size() - 1; index++)
            vec[index] = vec[index + 1];
        vec.pop_back();
        return exist;
    }
}
#pragma once
#include "Utils.h"
#include "engine/basic/Light.h"

/* 窗口信息 */
static const char* window_name = "UnicornRenderEngine";
static const int window_width = 800;
static const int window_height = 800;

/* 相机信息 */
// static const glm::vec3 CAMERA_POSITION = glm::vec3(0.0f, 9.0f, 26.0f);
static const glm::vec3 main_camera_position = glm::vec3(0.0f, 0.0f, 5.0f);
static const glm::vec3 main_camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
static const float main_camera_znear = 0.1f;
static const float main_camera_zfar = 100.0f;

/* 颜色信息 */
static const glm::vec4 color_background = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
static const glm::vec3 color_border = glm::vec3(1.0f, 0.0f, 0.0f);

/* 后期处理效果个数 */
static const int num_post_process = 9;

/* 路径信息 */
// 所有章节公用资源
static const std::string root_path = "E:/MyProject/GameEngine/UnicornRenderEngine/URE/";
static const std::string root_path_resource = root_path + "resource/";
static const std::string root_path_texture = root_path_resource + "texture/";
static const std::string root_path_skybox = root_path_resource + "skybox/";
static const std::string root_path_model = root_path_resource + "model/";
static const std::string root_path_shader = root_path + "engine/shader/";


/* 运行时全局变量 */
class GlobalValue {
public:
    static GlobalValue& GetInstance(){
        static GlobalValue instance;
        return instance;
    }

public:
    void SetValue(std::string name, float value, float min = -1e10, float max = 1e10) {
        if (value < min) value = min;
        if (value > max) value = max;
        float_value[name] = value;
    }
    void SetValue(std::string name, int value, int min = -1e10, int max = 1e10) {
        if (value < min) value = min;
        if (value > max) value = max;
        int_value[name] = value;
    }
    float GetFloatValue(std::string name) {
        if (float_value.find(name) == float_value.end())
            float_value[name] = 0;
        return float_value[name];
    }   
    int GetIntValue(std::string name) {
        if (int_value.find(name) == int_value.end())
            int_value[name] = 0;
        return int_value[name];
    }   

private:
    GlobalValue(){}
    std::map<std::string, float> float_value;
    std::map<std::string, int> int_value;
};

/* 场景物体信息 */
// 1. 2个箱子的初始位置
std::vector<glm::vec3> container_position {
    glm::vec3(-1.0f, 0.0f, -1.0f),
    glm::vec3(2.0f, 0.0f, 0.0f)
};
// 2. 5个窗户的初始位置
std::vector<glm::vec3> window_position {
    glm::vec3(-1.5f, 0.0f, -0.48f),
    glm::vec3( 1.5f, 0.0f, 0.51f),
    glm::vec3( 0.0f, 0.0f, 0.7f),
    glm::vec3(-0.3f, 0.0f, -2.3f),
    glm::vec3( 0.5f, 0.0f, -0.6f)
};
// 3. 1个定向光, 5个点光源, 1个聚光源的属性
Light* light_datas[] = {
    new DirectLight(
        glm::vec3(-0.2f, -1.0f, -0.3f),     // 方向
        glm::vec3(0.05f, 0.05f, 0.05f),     // 环境光
        glm::vec3(0.5f, 0.5f, 0.5f),        // 漫反射
        glm::vec3(1.0f, 1.0f, 1.0f)         // 高光
    ),
    new PointLight(
        glm::vec3(0.7f,  0.2f,  2.0f),      // 位置
        glm::vec3(0.05f, 0.05f, 0.05f),     // 环境光
        glm::vec3(0.8f, 0.8f, 0.8f),        // 漫反射
        glm::vec3(1.0f, 1.0f, 1.0f)         // 高光
    ),
    new PointLight(
        glm::vec3(2.3f, -3.3f, -4.0f),      // 位置
        glm::vec3(0.05f, 0.05f, 0.05f),     // 环境光
        glm::vec3(0.8f, 0.8f, 0.8f),        // 漫反射
        glm::vec3(1.0f, 1.0f, 1.0f)         // 高光
    ),
    new PointLight(
        glm::vec3(-4.0f,  2.0f, -12.0f),    // 位置
        glm::vec3(0.05f, 0.05f, 0.05f),     // 环境光
        glm::vec3(0.8f, 0.8f, 0.8f),        // 漫反射
        glm::vec3(1.0f, 1.0f, 1.0f)         // 高光
    ),
    new PointLight(
        glm::vec3(0.0f,  0.0f, -3.0f),      // 位置
        glm::vec3(0.05f, 0.05f, 0.05f),     // 环境光
        glm::vec3(0.8f, 0.8f, 0.8f),        // 漫反射
        glm::vec3(1.0f, 1.0f, 1.0f)         // 高光
    ),
    new SpotLight(
        glm::vec3(0.0f,  0.0f, 0.0f),       // 位置
        glm::vec3(-0.2f, -1.0f, -0.3f),     // 方向
        glm::vec3(0, 0, 0),                 // 环境光
        glm::vec3(1.0f, 1.0f, 1.0f),        // 漫反射
        glm::vec3(1.0f, 1.0f, 1.0f),        // 高光
        12.5f,                              // 内切角
        15.0f                               // 外切角
    )
};


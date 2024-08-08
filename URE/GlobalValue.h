#pragma once
#include "Utils.h"

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
#pragma once
#include "Utils.h"
#include "engine/basic/Light.h"

/* 路径信息 */
static const std::string root_path = "E:/MyProject/GameEngine/UnicornRenderEngine/URE/";
static const std::string root_path_shader = root_path + "shader/";
static const std::string root_path_resource = root_path + "resource/";
static const std::string root_path_texture = root_path_resource + "texture/";
static const std::string root_path_skybox = root_path_resource + "skybox/";
static const std::string root_path_model = root_path_resource + "model/";

/* 窗口信息 */
extern const char* window_name;
extern const int window_width;
extern const int window_height;

/* 颜色信息 */
extern const glm::vec4 color_background;
extern const glm::vec3 color_border;

/* 是否使用OpenGL功能 */
extern const bool use_opengl_blend;         // 混合
extern const bool use_opengl_cull_face;    // 面剔除

/* 后期处理效果个数 */
static const int num_post_process = 9;


/* 运行时全局变量 */
class GlobalValue {
public:
    static GlobalValue& GetInstance();

public:
    void SetValue(std::string name, float value, float min = -1e10, float max = 1e10);
    void SetValue(std::string name, int value, int min = -1e10, int max = 1e10);
    void SwitchValue(std::string name, int state_count = 2);
    float GetFloatValue(std::string name);
    int GetIntValue(std::string name);

private:
    GlobalValue(){}
    std::map<std::string, float> float_value;
    std::map<std::string, int> int_value;
};

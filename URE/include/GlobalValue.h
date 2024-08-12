#pragma once
#include "Utils.h"
#include "engine/basic/Light.h"

/* 窗口信息 */
extern const char* window_name;
extern const int window_width;
extern const int window_height;

/* 相机信息 */
extern const glm::vec3 main_camera_position;
extern const float main_camera_znear;
extern const float main_camera_zfar;
extern const float main_camera_move_speed_min;
extern const float main_camera_move_speed_max;

/* 颜色信息 */
extern const glm::vec4 color_background;
extern const glm::vec3 color_border;

/* 后期处理效果个数 */
extern const int num_post_process;

/* 路径信息 */
// 所有章节公用资源
extern const std::string root_path;
extern const std::string root_path_resource;
extern const std::string root_path_texture;
extern const std::string root_path_skybox;
extern const std::string root_path_model;
extern const std::string root_path_shader;


/* 运行时全局变量 */
class GlobalValue {
public:
    static GlobalValue& GetInstance();

public:
    void SetValue(std::string name, float value, float min = -1e10, float max = 1e10);
    void SetValue(std::string name, int value, int min = -1e10, int max = 1e10);
    float GetFloatValue(std::string name);
    int GetIntValue(std::string name);

private:
    GlobalValue(){}
    std::map<std::string, float> float_value;
    std::map<std::string, int> int_value;
};

/* 场景物体信息 */
// 1. 2个箱子的初始位置
extern std::vector<glm::vec3> container_position;
// 2. 5个窗户的初始位置
extern std::vector<glm::vec3> window_position;
// 3. 1个定向光, 4个点光源, 1个聚光源的属性
extern Light* light_datas[6];

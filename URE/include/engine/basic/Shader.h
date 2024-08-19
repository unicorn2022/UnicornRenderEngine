#pragma once
#include "Utils.h"
#include "GlobalValue.h"

class Shader {
public:
    /** 着色器程序
     * \param name 着色器名称
     * \param use_geometry_shader 是否使用几何着色器
     * \param root_directory 根文件夹路径, 含`/`
     */
    Shader(std::string name, bool use_geometry_shader = false, std::string root_directory = root_path_shader);
    ~Shader();
public:
    /* 使用着色器 */
    void Use();

    /* 设置 uniform 变量值: bool */
    void SetUniform(const std::string& name, bool value) const;
    /* 设置 uniform 变量值: int */
    void SetUniform(const std::string& name, int value) const;
    /* 设置 uniform 变量值: float */
    void SetUniform(const std::string& name, float value) const;
    /* 设置 uniform 变量值: vec3 */
    void SetUniform(const std::string& name, glm::vec3 vec) const;
    /* 设置 uniform 变量值: vec4 */
    void SetUniform(const std::string& name, glm::vec4 vec) const;
    /* 设置 uniform 变量值: mat4 */
    void SetUniform(const std::string& name, glm::mat4& mat) const;
public:
    /* 着色器程序ID */
    unsigned int ID;
    /* 着色器程序名称 */
    std::string name;

private:
    /** 着色器程序
     * \param vertex_shader_path 顶点着色器文件路径
     * \param fragment_shader_path 片段着色器文件路径
     */
    void CreateShader(std::string vertex_shader_path, std::string fragment_shader_path);

    /** 着色器程序
     * \param vertex_shader_path 顶点着色器文件路径
     * \param geometry_shader_path 几何着色器文件路径
     * \param fragment_shader_path 片段着色器文件路径
     */
    void CreateShader(std::string vertex_shader_path, std::string geometry_shader_path, std::string fragment_shader_path);
    
    /* 判断 shader 编译是否成功 */
    bool Check(int shaderID, std::string message = "") const;

    bool CheckVarLocation(GLint location, const std::string& var_name) const;
};

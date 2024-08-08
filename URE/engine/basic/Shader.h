#pragma once
#include "Utils.h"
#include "GlobalValue.h"

class Shader {
public:
    /** 着色器程序
     * \param name 着色器名称
     */
    Shader(std::string name){
        this->name = name;
        CreateShader(root_path_shader + name + ".vert", root_path_shader + name + ".frag");
    }

    ~Shader() {
        glDeleteProgram(ID);
    }

public:
    /* 使用着色器 */
    void Use() {
        glUseProgram(ID);
    }

    /* 设置 uniform 变量值: bool */
    void SetUniform(const std::string& name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    /* 设置 uniform 变量值: int */
    void SetUniform(const std::string& name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    /* 设置 uniform 变量值: float */
    void SetUniform(const std::string& name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    /* 设置 uniform 变量值: vec3 */
    void SetUniform(const std::string& name, glm::vec3 vec) const {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), vec.x, vec.y, vec.z);
    }
    /* 设置 uniform 变量值: float4 */
    void SetUniform(const std::string& name, float value_1, float value_2, float value_3, float value_4) const {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), value_1, value_2, value_3, value_4);
    }
    /* 设置 uniform 变量值: float4 */
    void SetUniform(const std::string& name, glm::vec4 vec) const {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), vec.x, vec.y, vec.z, vec.w);
    }
    /* 设置 uniform 变量值: mat4 */
    void SetUniform(const std::string& name, glm::mat4& mat) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }

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
    void CreateShader(std::string vertex_shader_path, std::string fragment_shader_path) {
        /* 1. 从文件中读取源代码 */
        std::string vertex_shader_source = Utils::ReadFile(vertex_shader_path);
        std::string fragment_shader_source = Utils::ReadFile(fragment_shader_path);
        const char* vertex_shader_code = vertex_shader_source.c_str();
        const char* fragment_shader_code = fragment_shader_source.c_str();

        /* 2.1 顶点着色器 */
        unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &vertex_shader_code, NULL);
        glCompileShader(vertex_shader);
        Check(vertex_shader, "顶点着色器编译失败 [" + vertex_shader_path + "]");
        
        /* 2.2 片段着色器 */
        unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &fragment_shader_code, NULL);
        glCompileShader(fragment_shader);
        Check(fragment_shader, "片段着色器编译失败 [" + fragment_shader_path + "]");
        
        /* 3. 着色器程序 */
        ID = glCreateProgram();
        glAttachShader(ID, vertex_shader);
        glAttachShader(ID, fragment_shader);
        glLinkProgram(ID);
        Check(ID, "着色器链接失败");

        /* 4. 删除无用的着色器 */
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
    }
    

    /* 判断 shader 编译是否成功 */
    bool Check(int shaderID, std::string message = "") {
        int  success;
        char infoLog[512];
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
            std::cout << "[ERROR::Shader.h::Check()] " << message << infoLog << std::endl;
        }
        return success;
    }
};

#include "engine/basic/ALL.h"
#include "engine/mesh/ALL.h"
#include "engine/material/ALL.h"
#include "engine/component/ALL.h"
#include "engine/gameobject/ALL.h"
#include "GameWorld.h"
#include "GameComponent.h"
#include "GlobalValue.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void keyboard_callback(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void InitOpenGL();
void Run();

/* GLFW 窗口对象 */
GLFWwindow* window;
/* 帧时间 */
float last_time = 0.0f;
float delta_time = 0.0f;
/* 鼠标灵敏度 */
float mouse_sensitivity = 0.05f;

int main() {
    InitOpenGL();
    Run();
    return 0;
}

void Run() {
    /* 1. 启动各种功能 */
    // 1.1 深度测试
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // 1.2 模板测试
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); // 通过模板&深度测试时, 将模板值设置为 glStencilFunc 指定的 ref 值
    // 1.3 混合
    if (use_opengl_blend) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // 混合因子: src-源图像alpha值, dst-1-源图像alpha值
    }
    // 1.4 面剔除
    if (use_opengl_cull_face) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);    // 剔除背面
        glFrontFace(GL_CCW);    // 正面为逆时针方向
    }
    // 1.5 屏幕对象
    GOScreen* screen = new GOScreen("screen");

    /* 2. 渲染循环 */
    while (!glfwWindowShouldClose(window)) {
        /* 2.0 预处理 */
        float current_time = glfwGetTime();
        delta_time = current_time - last_time;
        last_time = current_time;
        
        /* 2.1 处理输入 */ 
        keyboard_callback(window);
        GameWorld::GetInstance().GameTick();
        
        /* 2.2 渲染过程 */
        GameWorld::GetInstance().RenderTick();

        /* 2.3 选择一个帧缓冲, 绘制到屏幕上 */
        {
            // 2.3.1 禁用深度测试, 面剔除
            if (use_opengl_cull_face) glDisable(GL_CULL_FACE);
            glDisable(GL_DEPTH_TEST);
            glClearColor(0, 0, 0, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            
            // 2.3.2 绘制屏幕对象
            glViewport(0, 0, window_width, window_height);
            FrameBuffer2D* target_frame_buffer_2D;

            // 2.3.3 判断是否显示阴影贴图
            {
                int show_shadow = GlobalValue::GetInstance().GetIntValue("show_shadow");
                int use_direct_light_num = UniformBufferLight::GetInstance().use_direct_light_num;
                int use_point_light_num = UniformBufferLight::GetInstance().use_point_light_num;
                // 方向光源阴影
                if (show_shadow < use_direct_light_num) {
                    GameWorld::GetInstance().ResetSkyboxTexture();
                    GameWorld::GetInstance().show_only_skybox = false;
                    auto shadow_component = GameComponent::GetInstance().GetComponentShadowDirectLight()[show_shadow];
                    target_frame_buffer_2D = shadow_component->frame_buffer_2D;
                } 
                // 点光源阴影
                else if (show_shadow < use_direct_light_num + use_point_light_num) {
                    int index = show_shadow - use_direct_light_num;
                    auto shadow_component = GameComponent::GetInstance().GetComponentShadowPointLight()[index];
                    GameWorld::GetInstance().skybox->SetSkyboxTexture(shadow_component->frame_buffer_cube->color_texture);
                    // GameWorld::GetInstance().show_only_skybox = true;
                    // GameWorld::GetInstance().main_camera->gameobject->GetComponents<ComponentTransform>()[0]->position = UniformBufferLight::GetInstance().point_lights[index].position;
                    target_frame_buffer_2D = GameWorld::GetInstance().main_camera->frame_buffer_2D;
                } 
                // 显示渲染画面
                else {
                    GameWorld::GetInstance().ResetSkyboxTexture();
                    GameWorld::GetInstance().show_only_skybox = false;
                    target_frame_buffer_2D = GameWorld::GetInstance().main_camera->frame_buffer_2D;
                }
            }

            // 2.3.4 绘制屏幕
            screen->SetTargetFrameBuffer2D(target_frame_buffer_2D);
            screen->Draw();
            
            // 2.3.5 重新启用深度测试, 面剔除
            glEnable(GL_DEPTH_TEST);
            if (use_opengl_cull_face) glEnable(GL_CULL_FACE);
        }
        
        /* 2.4 检查并调用事件, 交换缓存 */
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}

void InitOpenGL(){
    /* 初始化窗口, 设置窗口属性 */
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* 创建窗口对象 */
    window = glfwCreateWindow(window_width, window_height, window_name, NULL, NULL);
    if (window == NULL) {
        std::cout << "[ERROR::Engine.h::Engine()] 创建GLFW窗口对象失败\n";
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);

    /* 初始化GLAD */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "[ERROR::Engine.h::Engine()] 初始化GLAD失败\n";
        exit(-1);
    }

    /* 设置视口大小, 注册回调函数 */
    glViewport(0, 0, window_width, window_height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    /* 设置鼠标捕捉 */
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    /* 检查扩展是否支持 */
    const std::string extension_name[1] = {"GL_ARB_shading_language_include" };
    GLint n;
    glGetIntegerv(GL_NUM_EXTENSIONS, &n);
    std::cout << "支持的扩展总数: " << n << "\n";
    for(int i = 0; i < n; i++){
        std::string extension = (char*)glGetStringi(GL_EXTENSIONS, i);
        for (auto name : extension_name) {
            if (extension == name) {
                std::cout << "支持扩展: " << name << std::endl;
            }
        }
    }

    /* 检查最大 inout 变量个数 */
    GLint max_vertex_output_component;
    glGetIntegerv(GL_MAX_VERTEX_OUTPUT_COMPONENTS, &max_vertex_output_component);
    std::cout << "顶点着色器输出最大为: " << max_vertex_output_component << "N\n"; 
    GLint max_fragment_input_component;
    glGetIntegerv(GL_MAX_FRAGMENT_INPUT_COMPONENTS, &max_fragment_input_component);
    std::cout << "片段着色器输入最大为: " << max_fragment_input_component << "N\n"; 

    GlobalValue::GetInstance().SetValue("show_shadow", 12);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void keyboard_callback(GLFWwindow* window) {
    /* ESC 退出程序 */
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    /* 长按 TAB 显示 Border */
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
        GlobalValue::GetInstance().SetValue("show_border", 1);
    } else {
        GlobalValue::GetInstance().SetValue("show_border", 0);
    }
    
    /* 0~9 选择后处理效果 */
    for (int i = 0; i <= num_post_process; i++) {
        if (glfwGetKey(window, GLFW_KEY_0 + i) == GLFW_PRESS) 
            GlobalValue::GetInstance().SetValue("choose_post_process", i);
    }

    /* F1 ~ F12 选择显示阴影贴图 */
    for (int i = 0; i < 12; i++) {
        if (glfwGetKey(window, GLFW_KEY_F1 + i) == GLFW_PRESS) 
            GlobalValue::GetInstance().SetValue("show_shadow", i);
    }

    /* G 选择是否显示调试对象 */
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
        GlobalValue::GetInstance().SetValue("debug", 1);
    } else {
        GlobalValue::GetInstance().SetValue("debug", 0);
    }

    /* B 选择是否使用 blinn_phong 模型*/
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
        GlobalValue::GetInstance().SetValue("use_blinn_phong", 0);
    } else {
        GlobalValue::GetInstance().SetValue("use_blinn_phong", 1);
    }

    /* R 选择是否使用 Gamma 矫正 */
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        GlobalValue::GetInstance().SetValue("gamma", 1.0f);
    } else {
        GlobalValue::GetInstance().SetValue("gamma", 2.2f);
    }
    
    /* 控制GO */
    GameWorld::GetInstance().HandleKeyBoard(window, delta_time);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static bool first_mouse = true;
    static float x_last = 0.0f, y_last = 0.0f;

    if (first_mouse) {
        x_last = xpos;
        y_last = ypos;
        first_mouse = false;
    }

    float x_offset = (xpos - x_last) * mouse_sensitivity;
    float y_offset = (y_last - ypos) * mouse_sensitivity;
    x_last = xpos;
    y_last = ypos;

    /* 控制GO */
    GameWorld::GetInstance().HandleMouseMovement(x_offset, y_offset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    /* 控制GO */
    GameWorld::GetInstance().HandleMouseScroll(yoffset);
}
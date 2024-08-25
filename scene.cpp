#include "GameWorld.h"
#include "GameComponent.h"
#include "GlobalValue.h"
#include "engine/material/ALL.h"
#include "engine/basic/UniformBuffer.h"

/* 窗口信息 */
const char* window_name = "UnicornRenderEngine";
const int window_width = 800;
const int window_height = 800;

/* 颜色信息 */
const glm::vec4 color_background = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
const glm::vec3 color_border = glm::vec3(1.0f, 0.0f, 0.0f);

/* 是否使用OpenGL功能 */
const bool use_opengl_blend = true;         // 混合
const bool use_opengl_cull_face = true;    // 面剔除

/* 相机信息 */
static const float main_camera_move_speed_min = 2.0f;
static const float main_camera_move_speed_max = 20.0f;
static const int main_camera_samples = 8;


enum SceneChoice {
    Capture2D_Box_Window_Reflect_Refract,
    Planet,
    Blinn_Phong,
    Shadow_Map,
};
const SceneChoice scene = SceneChoice::Shadow_Map;

/* 基础场景 */
static void Scene_Skybox() {
    /* 天空盒 */
    {
        std::string skybox_file_name[6] = {"right", "left", "top", "bottom", "front", "back"};
        GameWorld::GetInstance().skybox_texture = new TextureCube(skybox_file_name, "jpg", true);
        GameWorld::GetInstance().skybox = new GOSkybox("skybox", new MaterialSkybox(GameWorld::GetInstance().skybox_texture));
    }
}
static void Scene_Light() {
    /* 3种灯光 */
    {   
        // 1. 方向光
        {
            // 1.1 方向光源方向
            std::vector<glm::vec3> direct_light_direction {
                glm::normalize(glm::vec3(1.0f, -1.0f, 0.0f)),
                glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)),
            };
            // 1.2 方向光源个数
            int num = UniformBufferLight::GetInstance().use_direct_light_num;
            // 1.3 方向光源可视化
            auto direct_light_cube = new GOCube("direct_light_cube", new MaterialConstantColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)), num, true);
            GameWorld::GetInstance().all_game_object.push_back(direct_light_cube);
            // 1.4 配置方向光源
            for (int i = 0; i < num; i++) {
                // 方向光源属性
                UniformBufferLight::GetInstance().direct_light[i] = DirectLight(
                    direct_light_direction[i],          // 方向
                    glm::vec3(0.01f, 0.01f, 0.01f),     // 环境光
                    glm::vec3(0.5f, 0.5f, 0.5f),        // 漫反射
                    glm::vec3(1.0f, 1.0f, 1.0f)         // 高光
                );
                // 可视化
                direct_light_cube->GetComponents<ComponentTransform>()[i]->TransformTranslate(glm::vec3(0.0f, 5.0f, 0.0f));
                direct_light_cube->GetComponents<ComponentTransform>()[i]->TransformRotate(Utils::Convert_Direction_To_Euler(direct_light_direction[i]));
                direct_light_cube->GetComponents<ComponentTransform>()[i]->TransformScale(glm::vec3(2, 0.05, 0.05));
                // 阴影
                auto direct_light_shadow = new GOShadowDirectLight(
                    "direct_light_shadow_" + std::to_string(i),
                    &UniformBufferLight::GetInstance().direct_light[i],
                    &UniformBufferShadow::GetInstance().direct_light_matrix[i],
                    &UniformBufferShadow::GetInstance().direct_light_shadow_map_index[i],
                    2048, 2048, 1,
                    0.1, 100.0,
                    -5.0f, 5.0f, -5.0f, 5.0f
                );
                direct_light_shadow->GetComponents<ComponentTransform>()[0]->TransformTranslate(direct_light_direction[i]);
                GameWorld::GetInstance().all_game_object.push_back(direct_light_shadow);
            }
        }
        // 2. 点光源
        {
            // 2.1 点光源位置
            std::vector<glm::vec3> point_light_position {
                glm::vec3(0.0f,  0.0f, 3.0f),
                glm::vec3(0.7f,  0.2f, 2.0f),
                glm::vec3(2.3f, -3.3f, -4.0f),
                glm::vec3(-4.0f,  2.0f, -12.0f),
            };
            // 2.2 点光源个数
            int num = UniformBufferLight::GetInstance().use_point_light_num;
            // 2.3 点光源可视化
            auto point_light_cube = new GOCube("point_light_cube", new MaterialConstantColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)), num, true);
            GameWorld::GetInstance().all_game_object.push_back(point_light_cube);
            // 2.4 配置点光源
            for (int i = 0; i < num; i++) {
                UniformBufferLight::GetInstance().point_lights[i] = PointLight(
                    point_light_position[i],            // 位置
                    glm::vec3(0.05f, 0.05f, 0.05f),     // 环境光
                    glm::vec3(0.2f, 0.2f, 0.2f),        // 漫反射
                    glm::vec3(0.5f, 0.5f, 0.5f)         // 高光
                );
                // 可视化
                point_light_cube->GetComponents<ComponentTransform>()[i]->TransformTranslate(point_light_position[i]);
                point_light_cube->GetComponents<ComponentTransform>()[i]->TransformScale(glm::vec3(0.1f));
                // 阴影
                auto point_light_shadow = new GOShadowPointLight(
                    "point_light_shadow_" + std::to_string(i),
                    &UniformBufferLight::GetInstance().point_lights[i],
                    &UniformBufferShadow::GetInstance().point_light_shadow_map_index[i],
                    2048, 2048
                );
                point_light_shadow->GetComponents<ComponentTransform>()[0]->TransformTranslate(point_light_position[i]);
                GameWorld::GetInstance().all_game_object.push_back(point_light_shadow);
            }
        }
        // 3. 聚光源
        {   
            // 3.1 聚光源位置&方向
            std::vector<glm::vec3> spot_light_position {
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 10.0f, 0.0f),
            };
            std::vector<glm::vec3> spot_light_direction {
                glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)),
                glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f)),
            };
            // 3.2 聚光源个数
            int num = UniformBufferLight::GetInstance().use_spot_light_num;
            // 3.3 点光源可视化
            auto spot_light_cube = new GOCube("spot_light_cube", new MaterialConstantColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)), num, true);
            GameWorld::GetInstance().all_game_object.push_back(spot_light_cube);
            // 3.4 配置聚光源
            for (int i = 0; i < num; i++) {
                UniformBufferLight::GetInstance().spot_light[i] = SpotLight(
                    spot_light_position[i],         // 位置
                    spot_light_direction[i],        // 方向
                    glm::vec3(0, 0, 0),             // 环境光
                    glm::vec3(1.0f, 1.0f, 1.0f),    // 漫反射
                    glm::vec3(1.0f, 1.0f, 1.0f),    // 高光
                    glm::cos(glm::radians(12.5f)),  // 内切角
                    glm::cos(glm::radians(15.0f))   // 外切角
                );
                // 第1个 spot light 为相机光源, 不需要可视化
                if (i == 0) spot_light_cube->GetComponents<ComponentTransform>()[i]->TransformScale(glm::vec3(0));
                else {
                    spot_light_cube->GetComponents<ComponentTransform>()[i]->TransformTranslate(spot_light_position[i]);
                    spot_light_cube->GetComponents<ComponentTransform>()[i]->TransformRotate(Utils::Convert_Direction_To_Euler(spot_light_direction[i]));
                    spot_light_cube->GetComponents<ComponentTransform>()[i]->TransformScale(glm::vec3(1.0, 0.05, 0.05));
                }
            }
            GameWorld::GetInstance().spot_light = &UniformBufferLight::GetInstance().spot_light[0];
        }
    }

    
}


/* 场景1: 测试场景 */
static void Test_Capture2D_Blend_Reflect_Scene() {
    UniformBufferLight::GetInstance().use_direct_light_num = MAX_DIRECT_LIGHT_COUNT;
    UniformBufferLight::GetInstance().use_point_light_num = MAX_POINT_LIGHT_COUNT;
    UniformBufferLight::GetInstance().use_spot_light_num = MAX_SPOT_LIGHT_COUNT;
    /* 主相机 */
    {
        GOCamera* camera = new GOCamera("main_camera", 45, 0.1f, 1000.0f, window_width, window_height, main_camera_samples);
        camera->GetComponents<ComponentTransform>()[0]->SetMoveSpeedClamp(main_camera_move_speed_min, main_camera_move_speed_max);
        camera->GetComponents<ComponentTransform>()[0]->TransformTranslate(glm::vec3(0.0f, 0.0f, 20.0f));
        camera->GetComponents<ComponentTransform>()[0]->TransformRotate(glm::vec3(0.0f, 0.0f, -90.0f));
        GameWorld::GetInstance().all_game_object.push_back(camera);
        GameWorld::GetInstance().main_camera = camera->GetComponents<ComponentCamera>()[0];
    }

    /* 场景捕获对象 */
    {
        // 场景捕获对象
        GOCapture2D* capture = new GOCapture2D("camera", 89, 0.1f, 1000.0f, 1920, 1080, 1);
        capture->GetComponents<ComponentTransform>()[0]->TransformTranslate(glm::vec3(0.0f, 2.0f, 5.0f));
        capture->GetComponents<ComponentTransform>()[0]->TransformRotate(glm::vec3(0.0f, -90.0f, 0.0f));
        GameWorld::GetInstance().all_game_object.push_back(capture);
        GameWorld::GetInstance().test_camera = capture->GetComponents<ComponentCamera>()[0];
        // 显示相机捕获画面
        GOSquare* screen = new GOSquare("camera_capture", new MaterialNoLight(capture->GetCaptureTexture()));
        screen->GetComponents<ComponentTransform>()[0]->TransformTranslate(glm::vec3(0, 0, -50));
        screen->GetComponents<ComponentTransform>()[0]->TransformScale(glm::vec3(16, 9, 1));
        GameWorld::GetInstance().all_game_object.push_back(screen);
        GameWorld::GetInstance().test_camera_screen = screen->GetComponents<ComponentMesh>()[0];
    }

    /* 2个箱子 */
    {
        const std::vector<glm::vec3> container_position {
            glm::vec3(-1.0f, 0.0f, -1.0f),
            glm::vec3(2.0f, 0.0f, 0.0f)
        };
        GOCube* container = new GOCube("container",
            new MaterialPhongLight(new Texture("container_diffuse.png"), new Texture("container_specular.png")), 
            container_position.size()
        );
        container->AddComponent(new ComponentBorder(container, container->GetComponents<ComponentMesh>()[0]));
        for (int i = 0; i < container_position.size(); i++) {
            auto position = container_position[i];
            container->GetComponents<ComponentTransform>()[i]->TransformTranslate(position);
            container->GetComponents<ComponentTransform>()[i]->TransformScale(glm::vec3(0.5f));
        }
        GameWorld::GetInstance().all_game_object.push_back(container);
    }

    /* 5个窗户 */
    {
        const std::vector<glm::vec3> window_position {
            glm::vec3(-1.5f, 0.0f, -0.48f),
            glm::vec3( 1.5f, 0.0f, 0.51f),
            glm::vec3( 0.0f, 0.0f, 0.7f),
            glm::vec3(-0.3f, 0.0f, -2.3f),
            glm::vec3( 0.5f, 0.0f, -0.6f)
        };
        GO* windows = new GOSquare("windows",
            new MaterialPhongLight(new Texture("window.png"), new Texture("window.png")),
            window_position.size(),
            false,
            true
        );
        for (int i = 0; i < window_position.size(); i++) {
            auto position = window_position[i];
            windows->GetComponents<ComponentTransform>()[i]->TransformTranslate(position);
            windows->GetComponents<ComponentTransform>()[i]->TransformScale(glm::vec3(0.5f));
        }
        GameWorld::GetInstance().all_game_object.push_back(windows);
    }

    /* 1个反射天空盒的物体 */
    {
        GO* reflect_item = new GOOBJModel("reflect_item", "nanosuit", new MaterialSkyboxReflect(GameWorld::GetInstance().skybox->GetSkyboxTexture()));
        reflect_item->GetComponents<ComponentTransform>()[0]->TransformScale(glm::vec3(0.5));
        reflect_item->GetComponents<ComponentTransform>()[0]->TransformTranslate(glm::vec3(3, 0, -5));
        reflect_item->AddComponent(new ComponentBorder(reflect_item, reflect_item->GetComponents<ComponentMesh>()[0]));
        GameWorld::GetInstance().all_game_object.push_back(reflect_item);
    }
    
    /* 1个折射天空盒的物体 */
    {
        GO* refract_item = new GOOBJModel("reflect_item", "nanosuit", new MaterialSkyboxRefract(GameWorld::GetInstance().skybox->GetSkyboxTexture()));
        refract_item->GetComponents<ComponentTransform>()[0]->TransformScale(glm::vec3(0.5));
        refract_item->GetComponents<ComponentTransform>()[0]->TransformTranslate(glm::vec3(-3, 0, -5));
        GameWorld::GetInstance().all_game_object.push_back(refract_item);
    }
}
static void Test_Capture2D_Blend_Reflect_GameTick() {
    GameWorld::GetInstance().test_camera->gameobject->GetComponents<ComponentTransform>()[0]->TransformRotate(glm::vec3(0, 1, 0));
}


/* 场景2: 行星带 */
static void Test_Planet_Scene() {
    UniformBufferLight::GetInstance().use_direct_light_num = MAX_DIRECT_LIGHT_COUNT;
    UniformBufferLight::GetInstance().use_point_light_num = MAX_POINT_LIGHT_COUNT;
    UniformBufferLight::GetInstance().use_spot_light_num = MAX_SPOT_LIGHT_COUNT;
    /* 主相机 */
    {
        GOCamera* camera = new GOCamera("main_camera", 45, 0.1f, 1000.0f, window_width, window_height, main_camera_samples);
        camera->GetComponents<ComponentTransform>()[0]->SetMoveSpeedClamp(main_camera_move_speed_min, main_camera_move_speed_max);
        camera->GetComponents<ComponentTransform>()[0]->TransformTranslate(glm::vec3(0.0f, 0.0f, 100.0f));
        camera->GetComponents<ComponentTransform>()[0]->TransformRotate(glm::vec3(0.0f, 0.0f, -90.0f));
        GameWorld::GetInstance().all_game_object.push_back(camera);
        GameWorld::GetInstance().main_camera = camera->GetComponents<ComponentCamera>()[0];
    }

    /* 1个小行星 & 岩石 */
    {
        float a = 100.0f, b = 80.0f, c = 2.5f;
        GOOBJModel* planet = new GOOBJModel("planet", "planet", NULL);
        planet->GetComponents<ComponentTransform>()[0]->TransformTranslate(glm::vec3(a, 0.0f, 0.0f));
        planet->GetComponents<ComponentTransform>()[0]->TransformScale(glm::vec3(5.0f));
        planet->AddComponent(new ComponentBorder(planet, planet->GetComponents<ComponentMesh>()[0]));
        GameWorld::GetInstance().all_game_object.push_back(planet);
        GameWorld::GetInstance().planet = planet->GetComponents<ComponentTransform>()[0];

        int amount = 1000;
        GOOBJModel* rock = new GOOBJModel("rock", "rock", NULL, amount);
        for (int i = 0; i < amount; i++) {
            auto transform = rock->GetComponents<ComponentTransform>()[i];
            transform->SetParent(planet);
            // transform->TransformTranslate(glm::vec3(-a, 0.0f, 0.0f)); // 抵消行星的位移
            // 1. 位移
            float t = Utils::Random(0.0f, 2.0f * Utils::PI);
            float x = a * cos(t);
            float z = b * sin(t);
            float y = Utils::Random(-c, c);
            transform->TransformTranslate(glm::vec3(x, y, z) * Utils::Random(0.5f, 1.0f));
            // 2. 缩放
            transform->TransformScale(glm::vec3(Utils::Random(0.2f, 0.5f)));
            // 3. 旋转
            transform->TransformRotate(glm::vec3(Utils::Random(0.f, 360.0f), Utils::Random(0.f, 360.0f), Utils::Random(0.f, 360.0f)));
        }
        GameWorld::GetInstance().all_game_object.push_back(rock);
    }
}
static void Test_Planet_GameTick() {
    GameWorld::GetInstance().planet->TransformRotate(glm::vec3(0, 0.5, 0));
}


/* 场景3: 测试 Blinn-Phong 模型 */
static void Test_Blinn_Phong_Scene() {
    UniformBufferLight::GetInstance().use_direct_light_num = MAX_DIRECT_LIGHT_COUNT;
    UniformBufferLight::GetInstance().use_point_light_num = MAX_POINT_LIGHT_COUNT;
    UniformBufferLight::GetInstance().use_spot_light_num = MAX_SPOT_LIGHT_COUNT;
    /* 主相机 */
    {
        GOCamera* camera = new GOCamera("main_camera", 45, 0.1f, 1000.0f, window_width, window_height, main_camera_samples);
        camera->GetComponents<ComponentTransform>()[0]->SetMoveSpeedClamp(main_camera_move_speed_min, main_camera_move_speed_max);
        camera->GetComponents<ComponentTransform>()[0]->TransformTranslate(glm::vec3(0.0f, 0.0f, 20.0f));
        camera->GetComponents<ComponentTransform>()[0]->TransformRotate(glm::vec3(0.0f, 0.0f, -90.0f));
        GameWorld::GetInstance().all_game_object.push_back(camera);
        GameWorld::GetInstance().main_camera = camera->GetComponents<ComponentCamera>()[0];
    }

    /* 平面 */
    {
        auto plane = new GOSquare("plane", new MaterialPhongLight(new Texture("wood.png"), new Texture("wood.png")));
        plane->GetComponents<ComponentTransform>()[0]->TransformScale(glm::vec3(5.0f, 5.0f, 5.0f));
        GameWorld::GetInstance().all_game_object.push_back(plane);
    }
}
static void Test_Blinn_Phong_GameTick() {}

/* 场景4: 测试 Shadow Map 算法 */
static void Test_Shadow_Map_Scene() {
    UniformBufferLight::GetInstance().use_direct_light_num = 1;
    UniformBufferLight::GetInstance().use_point_light_num = 1;
    UniformBufferLight::GetInstance().use_spot_light_num = 0;
    /* 主相机 */
    {
        GOCamera* camera = new GOCamera("main_camera", 45, 0.1f, 1000.0f, window_width, window_height, main_camera_samples);
        camera->GetComponents<ComponentTransform>()[0]->SetMoveSpeedClamp(main_camera_move_speed_min, main_camera_move_speed_max);
        camera->GetComponents<ComponentTransform>()[0]->TransformTranslate(glm::vec3(0.0f, 2.5f, 10.0f));
        camera->GetComponents<ComponentTransform>()[0]->TransformRotate(glm::vec3(0.0f, 0.0f, -90.0f));
        GameWorld::GetInstance().all_game_object.push_back(camera);
        GameWorld::GetInstance().main_camera = camera->GetComponents<ComponentCamera>()[0];
    }

    /* 平面 */
    {
        auto plane = new GOCube("plane", new MaterialPhongLight(new Texture("wood.png"), new Texture("wood.png")));
        plane->GetComponents<ComponentTransform>()[0]->TransformTranslate(glm::vec3(0.0f, -0.6f, 0.0f));
        plane->GetComponents<ComponentTransform>()[0]->TransformScale(glm::vec3(5.0f, 0.1f, 5.0f));
        GameWorld::GetInstance().all_game_object.push_back(plane);
    }

    /* 箱子 */
    {
        const std::vector<glm::vec3> container_position {
            glm::vec3(-2.0f, 1.0f, 0.0f),
            glm::vec3(3.0f, 0.0f, 0.5f),
            glm::vec3(3.0f, 0.0f, -0.5f)
        };
        const std::vector<glm::vec3> container_rotate {
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f)
        };
        GOCube* container = new GOCube("container",
            new MaterialPhongLight(new Texture("container_diffuse.png"), new Texture("container_specular.png")), 
            container_position.size()
        );
        // container->AddComponent(new ComponentBorder(container, container->GetComponents<ComponentMesh>()[0]));
        for (int i = 0; i < container_position.size(); i++) {            
            container->GetComponents<ComponentTransform>()[i]->TransformTranslate(container_position[i]);
            container->GetComponents<ComponentTransform>()[i]->TransformRotate(container_rotate[i]);
            container->GetComponents<ComponentTransform>()[i]->TransformScale(glm::vec3(0.5f));
        }
        GameWorld::GetInstance().all_game_object.push_back(container);
    }
}
static void Test_Shadow_Map_GameTick() {
    // GameWorld::GetInstance().main_camera->gameobject->GetComponents<ComponentTransform>()[0]->position = (10.0f * UniformBufferLight::GetInstance().direct_light[0].direction);
    // std::cout << "main_camera: " 
    //           << "yaw:"  << GameWorld::GetInstance().main_camera->gameobject->GetComponents<ComponentTransform>()[0]->GetYaw() << " " 
    //           << "pitch: " << GameWorld::GetInstance().main_camera->gameobject->GetComponents<ComponentTransform>()[0]->GetPitch() << "\n"; 
}



/* 实现 GameWorld 的 GameTick() 和 SceneCreate() */
void GameWorld::SceneCreate() {
    /* 天空盒 */
    Scene_Skybox();

    /* 场景配置 */
    switch (scene) {
    case SceneChoice::Capture2D_Box_Window_Reflect_Refract: {
        Test_Capture2D_Blend_Reflect_Scene();
        break;
    }
    case SceneChoice::Planet: {
        Test_Planet_Scene();
        break;
    }
    case SceneChoice::Blinn_Phong: {
        Test_Blinn_Phong_Scene();
        break;
    }
    case SceneChoice::Shadow_Map: {
        Test_Shadow_Map_Scene();
        break;
    }
    default:
        break;
    }

    /* 灯光 */
    Scene_Light();
}
void GameWorld::GameTick() {
    /* 设置聚光源属性 */
    spot_light->position = main_camera->camera->position;
    spot_light->direction = main_camera->camera->front;

    /* 更新 camera 组件中的相机状态 */
    auto camera_component = GameComponent::GetInstance().GetComponentCamera();
        for (auto camera_component : camera_component)
            camera_component->UpdateCameraState();

    /* 更新 shadow 组件中的相机状态 */
    auto component_shadow_direct_lights = GameComponent::GetInstance().GetComponentShadowDirectLight();
        for (auto shadow_component : component_shadow_direct_lights)
            shadow_component->UpdateCameraState();
    
    /* 场景 GameTick */
    switch (scene) {
    case SceneChoice::Capture2D_Box_Window_Reflect_Refract: {
        Test_Capture2D_Blend_Reflect_GameTick();
        break;
    }
    case SceneChoice::Planet: {
        Test_Planet_GameTick();
        break;
    }
    case SceneChoice::Blinn_Phong: {
        Test_Blinn_Phong_GameTick();
        break;
    }
    case SceneChoice::Shadow_Map: {
        Test_Shadow_Map_GameTick();
        break;
    }
    default:
        break;
    }
}
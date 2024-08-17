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
const glm::vec4 color_background = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f);
const glm::vec3 color_border = glm::vec3(1.0f, 0.0f, 0.0f);

/* 相机信息 */
static const float main_camera_move_speed_min = 2.0f;
static const float main_camera_move_speed_max = 20.0f;
static const int main_camera_samples = 8;

enum SceneChoice {
    Capture2D_Box_Window_Reflect_Refract,
    Planet,
    Blinn_Phong,
};
const SceneChoice scene = SceneChoice::Blinn_Phong;


static void Light_Skybox() {
    /* 6个灯光 */
    {
        std::vector<glm::vec3> point_light_position {
            glm::vec3(0.7f,  0.2f,  2.0f),
            glm::vec3(2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f,  2.0f, -12.0f),
            glm::vec3(0.0f,  0.0f, -3.0f)
        };
        UniformBufferLight::GetInstance().direct_light = DirectLight(
            glm::vec3(-0.2f, -1.0f, -0.3f),     // 方向
            glm::vec3(0.05f, 0.05f, 0.05f),     // 环境光
            glm::vec3(0.5f, 0.5f, 0.5f),        // 漫反射
            glm::vec3(1.0f, 1.0f, 1.0f)         // 高光
        );
        auto point_light_cube = new GOCube("point_light_cube", new MaterialConstantColor(glm::vec4(1.0f)), 4);
        for (int i = 0; i < MAX_POINT_LIGHT_COUNT; i++) {
            UniformBufferLight::GetInstance().point_lights[i] = PointLight(
                point_light_position[i],            // 位置
                glm::vec3(0.05f, 0.05f, 0.05f),     // 环境光
                glm::vec3(0.8f, 0.8f, 0.8f),        // 漫反射
                glm::vec3(1.0f, 1.0f, 1.0f)         // 高光
            );
            point_light_cube->GetComponents<ComponentTransform>()[i]->TransformTranslate(point_light_position[i]);
            point_light_cube->GetComponents<ComponentTransform>()[i]->TransformScale(glm::vec3(0.1f));
        }
        UniformBufferLight::GetInstance().spot_light = SpotLight(
            glm::vec3(0.0f, 0.0f, 0.0f),    // 位置
            glm::vec3(1.0f, 0.0f, 0.0f),    // 方向
            glm::vec3(0, 0, 0),             // 环境光
            glm::vec3(1.0f, 1.0f, 1.0f),    // 漫反射
            glm::vec3(1.0f, 1.0f, 1.0f),    // 高光
            glm::cos(glm::radians(12.5f)),  // 内切角
            glm::cos(glm::radians(15.0f))   // 外切角
        );
        GameWorld::GetInstance().spot_light = &UniformBufferLight::GetInstance().spot_light;
    }

    /* 天空盒 */
    {
        std::string skybox_file_name[6] = {"right", "left", "top", "bottom", "front", "back"};
        GameWorld::GetInstance().skybox = new GOSkybox("skybox", new MaterialSkybox(new TextureCube(skybox_file_name, "jpg")));
    }
}

/* 场景1: 测试场景 */
static void Test_Capture2D_Blend_Reflect_Scene() {
    /* 主相机 */
    {
        GOCamera* camera = new GOCamera("main_camera", 45, 0.1f, 1000.0f, window_width, window_height, main_camera_samples);
        camera->GetComponents<ComponentTransform>()[0]->SetMoveSpeedClamp(main_camera_move_speed_min, main_camera_move_speed_max);
        camera->GetComponents<ComponentTransform>()[0]->TransformTranslate(glm::vec3(0.0f, 0.0f, 20.0f));
        camera->GetComponents<ComponentTransform>()[0]->TransformRotate(glm::vec3(0.0f, -90.0f, 0.0f));
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
    /* 主相机 */
    {
        GOCamera* camera = new GOCamera("main_camera", 45, 0.1f, 1000.0f, window_width, window_height, main_camera_samples);
        camera->GetComponents<ComponentTransform>()[0]->SetMoveSpeedClamp(main_camera_move_speed_min, main_camera_move_speed_max);
        camera->GetComponents<ComponentTransform>()[0]->TransformTranslate(glm::vec3(0.0f, 0.0f, 100.0f));
        camera->GetComponents<ComponentTransform>()[0]->TransformRotate(glm::vec3(0.0f, -90.0f, 0.0f));
        GameWorld::GetInstance().all_game_object.push_back(camera);
        GameWorld::GetInstance().main_camera = camera->GetComponents<ComponentCamera>()[0];
    }

    /* 1个小行星 & 岩石 */
    {
        float a = 100.0f, b = 80.0f, c = 2.5f;
        GOOBJModel* planet = new GOOBJModel("planet", "planet", NULL);
        planet->GetComponents<ComponentTransform>()[0]->TransformTranslate(glm::vec3(a, 0.0f, 0.0f));
        planet->GetComponents<ComponentTransform>()[0]->TransformScale(glm::vec3(5.0f));
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

/* 场景3: 测试 Blinn-Phong 模型*/
static void Test_Blinn_Phong_Scene() {
    /* 主相机 */
    {
        GOCamera* camera = new GOCamera("main_camera", 45, 0.1f, 1000.0f, window_width, window_height, main_camera_samples);
        camera->GetComponents<ComponentTransform>()[0]->SetMoveSpeedClamp(main_camera_move_speed_min, main_camera_move_speed_max);
        camera->GetComponents<ComponentTransform>()[0]->TransformTranslate(glm::vec3(0.0f, 0.0f, 5.0f));
        camera->GetComponents<ComponentTransform>()[0]->TransformRotate(glm::vec3(0.0f, -90.0f, 0.0f));
        GameWorld::GetInstance().all_game_object.push_back(camera);
        GameWorld::GetInstance().main_camera = camera->GetComponents<ComponentCamera>()[0];
    }

    /* 平面 */
    {
        auto plane = new GOSquare("plane", new MaterialPhongLight(new Texture("wood.png"), new Texture("wood.png")));
        plane->GetComponents<ComponentTransform>()[0]->TransformScale(glm::vec3(10.0f, 0.5f, 10.0f));
        GameWorld::GetInstance().all_game_object.push_back(plane);
    }
}
static void Test_Blinn_Phong_GameTick() {}


/* 实现 GameWorld 的 GameTick() 和 SceneCreate() */
void GameWorld::SceneCreate() {
    /* 灯光、天空盒 */
    Light_Skybox();

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
    default:
        break;
    }
}
void GameWorld::GameTick() {
    /* 设置聚光源属性 */
    spot_light->position = main_camera->camera->position;
    spot_light->direction = main_camera->camera->front;

    /* 更新相机状态 */
    auto camera_component = GameComponent::GetInstance().GetComponentCamera();
        for (auto camera_component : camera_component)
            camera_component->UpdateCameraState();
    
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
    default:
        break;
    }
}
#include "GameWorld.h"
#include "GameComponent.h"
#include "GlobalValue.h"
#include "engine/material/ALL.h"
#include "engine/basic/UniformBuffer.h"

static void GameTick_Test_Capture2D() {
    GameWorld::GetInstance().test_camera->gameobject->GetComponent<ComponentTransform>()->TransformRotate(glm::vec3(0, 1, 0));
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
    // GameTick_Test_Capture2D();
}

static void Scene_MainCamera_Light_Skybox() {
    /* 主相机 */
    {
        GOCamera* camera = new GOCamera("main_camera", 45, main_camera_znear, main_camera_zfar);
        camera->GetComponent<ComponentTransform>()->SetMoveSpeedClamp(main_camera_move_speed_min, main_camera_move_speed_max);
        camera->GetComponent<ComponentTransform>()->TransformTranslate(main_camera_position);
        camera->GetComponent<ComponentTransform>()->TransformRotate(glm::vec3(0.0f, -90.0f, 0.0f));
        GameWorld::GetInstance().all_game_object.push_back(camera);
        GameWorld::GetInstance().main_camera = camera->GetComponent<ComponentCamera>();
    }

    /* 6个灯光 */
    {
        UniformBufferLight::GetInstance().direct_light = DirectLight(
            glm::vec3(-0.2f, -1.0f, -0.3f),     // 方向
            glm::vec3(0.05f, 0.05f, 0.05f),     // 环境光
            glm::vec3(0.5f, 0.5f, 0.5f),        // 漫反射
            glm::vec3(1.0f, 1.0f, 1.0f)         // 高光
        );
        for (int i = 0; i < MAX_POINT_LIGHT_COUNT; i++) {
            UniformBufferLight::GetInstance().point_lights[i] = PointLight(
                point_light_position[i],            // 位置
                glm::vec3(0.05f, 0.05f, 0.05f),     // 环境光
                glm::vec3(0.8f, 0.8f, 0.8f),        // 漫反射
                glm::vec3(1.0f, 1.0f, 1.0f)         // 高光
            );
        }
        UniformBufferLight::GetInstance().spot_light = SpotLight(
            glm::vec3(0.0f, 0.0f, 0.0f),    // 位置
            glm::vec3(1.0f, 0.0f, 0.0f),    // 方向
            glm::vec3(0, 0, 0),             // 环境光
            glm::vec3(1.0f, 1.0f, 1.0f), // 漫反射
            glm::vec3(1.0f, 1.0f, 1.0f), // 高光
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

static void Scene_Test_Capture2D_Box_Window_Reflect_Refract() {
    /* 场景捕获对象 + 相机捕获画面(square) */
    {
        GOCapture2D* capture = new GOCapture2D("camera", 89, main_camera_znear, main_camera_zfar, 1920, 1080);
        capture->GetComponent<ComponentTransform>()->TransformTranslate(glm::vec3(0.0f, 2.0f, 5.0f));
        capture->GetComponent<ComponentTransform>()->TransformRotate(glm::vec3(0.0f, -90.0f, 0.0f));
        GameWorld::GetInstance().all_game_object.push_back(capture);
        GameWorld::GetInstance().test_camera = capture->GetComponent<ComponentCamera>();
        GOSquare* screen = new GOSquare("camera_capture", new MaterialNoLight(capture->GetCaptureTexture()));
        screen->GetComponent<ComponentTransform>()->TransformTranslate(glm::vec3(0, 0, -50));
        screen->GetComponent<ComponentTransform>()->TransformScale(glm::vec3(16, 9, 1));
        GameWorld::GetInstance().all_game_object.push_back(screen);
        GameWorld::GetInstance().test_camera_screen = screen->GetComponent<ComponentMesh>();
    }

    /* 2个箱子 */
    {
        int cnt = 0;
        for (auto position : container_position) {
            GOCube* container = new GOCube("container[" + std::to_string(cnt++) + "]",
                new MaterialPhongLight(
                    new Texture("container_diffuse.png"), 
                    new Texture("container_specular.png")
                )
            );
            container->GetComponent<ComponentTransform>()->TransformTranslate(position);
            container->GetComponent<ComponentTransform>()->TransformScale(glm::vec3(0.5f));
            container->AddComponent(new ComponentBorder(container, container->GetComponent<ComponentMesh>()));
            GameWorld::GetInstance().all_game_object.push_back(container);
        }
    }

    /* 5个窗户 */
    {
        int cnt = 0;
        for (auto position : window_position) {
            GO* windows = new GOSquare("windows[" + std::to_string(cnt++) + "]",
                new MaterialPhongLight(
                    new Texture("window.png"), 
                    new Texture("window.png")
                ),
                true
            );
            windows->GetComponent<ComponentTransform>()->TransformTranslate(position);
            windows->GetComponent<ComponentTransform>()->TransformScale(glm::vec3(0.5f));
            GameWorld::GetInstance().all_game_object.push_back(windows);
        }
    }

    /* 1个反射天空盒的物体 */
    {
        GO* reflect_item = new GOOBJModel("reflect_item", "nanosuit", new MaterialSkyboxReflect(GameWorld::GetInstance().skybox->GetSkyboxTexture()));
        reflect_item->GetComponent<ComponentTransform>()->TransformScale(glm::vec3(0.5));
        reflect_item->GetComponent<ComponentTransform>()->TransformTranslate(glm::vec3(3, 0, -5));
        reflect_item->AddComponent(new ComponentBorder(reflect_item, reflect_item->GetComponent<ComponentMesh>()));
        GameWorld::GetInstance().all_game_object.push_back(reflect_item);
    }
    
    /* 1个折射天空盒的物体 */
    {
        GO* refract_item = new GOOBJModel("reflect_item", "nanosuit", new MaterialSkyboxRefract(GameWorld::GetInstance().skybox->GetSkyboxTexture()));
        refract_item->GetComponent<ComponentTransform>()->TransformScale(glm::vec3(0.5));
        refract_item->GetComponent<ComponentTransform>()->TransformTranslate(glm::vec3(-3, 0, -5));
        GameWorld::GetInstance().all_game_object.push_back(refract_item);
    }
}

static void Scene_Test_Planet() {
    /* 1个小行星 & 岩石 */
    {
        GOOBJModel* planet = new GOOBJModel("planet", "planet", NULL);
        planet->GetComponent<ComponentTransform>()->TransformTranslate(glm::vec3(3, 0, -10));
        GameWorld::GetInstance().all_game_object.push_back(planet);

        GOOBJModel* rock = new GOOBJModel("rock", "rock", NULL);
        rock->GetComponent<ComponentTransform>()->TransformTranslate(glm::vec3(-3, 0, -10));
        GameWorld::GetInstance().all_game_object.push_back(rock);
    }
}

void GameWorld::GameInit() {
    Scene_MainCamera_Light_Skybox();
    // Scene_Test_Capture2D_Box_Window_Reflect_Refract();
    Scene_Test_Planet();
}
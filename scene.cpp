#include "GameWorld.h"
#include "GameComponent.h"
#include "engine/material/ALL.h"

void GameWorld::GameTick() {
    /* 设置聚光源属性 */
    SpotLight* light_data = dynamic_cast<SpotLight*>(spot_light->light_data);
    light_data->position = main_camera->camera->position;
    light_data->direction = main_camera->camera->front;
    /* 更新相机状态 */
    // test_camera->gameobject->GetComponent<ComponentTransform>()->TransformRotate(glm::vec3(0, 1, 0));
    auto camera_component = GameComponent::GetInstance().GetComponentCamera();
        for (auto camera_component : camera_component)
            camera_component->UpdateCameraState();

}

GameWorld::GameWorld() {
    /* 主相机 */
    GO* camera = new GOCamera("main_camera", 45, main_camera_znear, main_camera_zfar);
    camera->GetComponent<ComponentTransform>()->SetMoveSpeedClamp(main_camera_move_speed_min, main_camera_move_speed_max);
    camera->GetComponent<ComponentTransform>()->TransformTranslate(main_camera_position);
    camera->GetComponent<ComponentTransform>()->TransformRotate(glm::vec3(0.0f, -90.0f, 0.0f));
    all_game_object.push_back(camera);
    main_camera = camera->GetComponent<ComponentCamera>();

    /* 副相机 + 相机捕获画面(square) + 相机可视化(cube) */
    camera = new GOCamera("camera", 89, main_camera_znear, main_camera_zfar, 1920, 1080);
    camera->GetComponent<ComponentTransform>()->TransformTranslate(glm::vec3(0, 0, 3));
    camera->GetComponent<ComponentTransform>()->TransformRotate(glm::vec3(0.0f, -90.0f, 0.0f));
    all_game_object.push_back(camera);
    test_camera = camera->GetComponent<ComponentCamera>();
    GO* screen = new GOSquare("camera_capture", new MaterialNoLight(camera->GetComponent<ComponentCamera>()->frame_buffer->color_texture) );
    screen->GetComponent<ComponentTransform>()->TransformTranslate(glm::vec3(0, 0, -50));
    screen->GetComponent<ComponentTransform>()->TransformScale(glm::vec3(16, 9, 1));
    all_game_object.push_back(screen);
    GO* camera_cube = new GOCube("main_camera_cube", new MaterialConstantColor(glm::vec4(1, 0, 0, 1)) );
    camera_cube->GetComponent<ComponentTransform>()->SetParent(camera);
    camera_cube->GetComponent<ComponentTransform>()->TransformScale(glm::vec3(0.1, 0.1, 0.1));
    all_game_object.push_back(camera_cube);

    /* 2个箱子 */
    int cnt = 0;
    for (auto position : container_position) {
        GO* container = new GOCube("container" + std::to_string(cnt++),
            new MaterialPhongLight(
                new Texture("container_diffuse.png"), 
                new Texture("container_specular.png")
            )
        );
        container->GetComponent<ComponentTransform>()->TransformTranslate(position);
        container->GetComponent<ComponentTransform>()->TransformScale(glm::vec3(0.5f));
        all_game_object.push_back(container);
    }

    /* 5个窗户 */
    cnt = 0;
    for (auto position : window_position) {
        GO* windows = new GOSquare("windows" + std::to_string(cnt++),
            new MaterialPhongLight(
                new Texture("window.png"), 
                new Texture("window.png")
            ),
            true
        );
        windows->GetComponent<ComponentTransform>()->TransformTranslate(position);
        windows->GetComponent<ComponentTransform>()->TransformScale(glm::vec3(0.5f));
        all_game_object.push_back(windows);
    }

    /* 6个灯光 */
    cnt = 0;
    for (auto light_data : light_datas) {
        GO* light = new GOLight("light" + std::to_string(cnt++), light_data);
        all_game_object.push_back(light);
        if (dynamic_cast<SpotLight*>(light_data) != NULL)
            spot_light = light->GetComponent<ComponentLight>();
    }

    /* 天空盒 */
    std::string skybox_file_name[6] = {"right", "left", "top", "bottom", "front", "back"};
    skybox = new GOSkybox("skybox", 
        new MaterialSkybox(new TextureCube(skybox_file_name, "jpg"))
    );
}
#include "GameWorld.h"
#include "GameComponent.h"

GameWorld& GameWorld::GetInstance() {
    static GameWorld* instance;
    if (instance == NULL) instance = new GameWorld(), instance->SceneCreate();
    return *instance;
}

GameWorld::GameWorld() {}

GameWorld::~GameWorld() {
    for (auto game_object : all_game_object) delete game_object;
    delete skybox;
}

void GameWorld::RenderTick() {
    /* 1. 渲染阴影贴图 */
    ShowAllObject();
    // 1.1 正面剔除
    if (use_opengl_cull_face) glCullFace(GL_FRONT);
    // 1.2 定向光源阴影
    auto component_shadow_direct_lights = GameComponent::GetInstance().GetComponentShadowDirectLight();
    for (auto shadow_component : component_shadow_direct_lights) 
        shadow_component->RenderTick();
    // 1.3 点光源阴影
    auto component_shadow_point_lights = GameComponent::GetInstance().GetComponentShadowPointLight();
    for (auto shadow_component : component_shadow_point_lights) 
        shadow_component->RenderTick();
    // 1.4 取消正面剔除
    if (use_opengl_cull_face) glCullFace(GL_BACK);

    /* 2. 每个相机渲染一次 */ 
    if (show_only_skybox) ShowOnlySkybox();
    auto camera_components = GameComponent::GetInstance().GetComponentCamera();
    for (auto camera_component : camera_components) 
        camera_component->RenderTick();
}

void GameWorld::HandleMouseMovement(float x_offset, float y_offset) {
    // 更新 main_camera 的旋转
    main_camera->gameobject->GetComponents<ComponentTransform>()[0]->ProcessMouseMovement(x_offset, y_offset);
}

void GameWorld::HandleMouseScroll(float y_offset) {
    // 更新 main_camera 的视野
    main_camera->ProcessMouseScroll(y_offset);
}

void GameWorld::ShowOnlySkybox() {
    auto mesh_components = GameComponent::GetInstance().GetComponentMesh(NULL, false);
    for (auto mesh_component : mesh_components) 
        if (!mesh_component->IsDebug()) mesh_component->enable = false;
}

void GameWorld::ShowAllObject() {
    auto mesh_components = GameComponent::GetInstance().GetComponentMesh(NULL, false);
    for (auto mesh_component : mesh_components) 
        if (!mesh_component->IsDebug()) mesh_component->enable = true;
}

void GameWorld::ResetSkyboxTexture() {
    skybox->SetSkyboxTexture(skybox_texture);
}
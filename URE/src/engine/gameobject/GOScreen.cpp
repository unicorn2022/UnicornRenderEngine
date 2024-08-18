#include "engine/gameobject/GOScreen.h"

GOScreen::GOScreen(std::string name) : GO(name) {
    screen = new MeshSquare();
    screen_mat = new MaterialPostProcess(NULL, NULL);
}
GOScreen::~GOScreen() {
    delete screen;
    delete screen_mat;
}


void GOScreen::SetTargetFrameBuffer(FrameBuffer* frame_buffer) {
    screen_mat->screen_texture = frame_buffer->color_texture;
    screen_mat->screen_texture_multisample = frame_buffer->color_texture_multisample;
}

void GOScreen::Draw() {
    screen_mat->choose_post_process = GlobalValue::GetInstance().GetIntValue("choose_post_process");
    screen_mat->Use();
    screen->Draw(1);
}
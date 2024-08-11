#include "engine/material/Material.h"

Material::~Material() {
    delete shader;
}

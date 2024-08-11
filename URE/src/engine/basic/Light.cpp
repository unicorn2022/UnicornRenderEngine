#include "engine/basic/Light.h"

Light::Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
}
Light::~Light() {}


DirectLight::DirectLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) 
: Light(ambient, diffuse, specular) {
    this->direction = direction;
}
DirectLight::~DirectLight() {}


PointLight::PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) 
: Light(ambient, diffuse, specular) {
    this->position = position;
    this->constant = 1.0f;
    this->linear = 0.09f;
    this->quadratic = 0.032f;
}
PointLight::~PointLight() {}


SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float inner_cut_off, float outer_cut_off) 
: Light(ambient, diffuse, specular) {
    this->position = position;
    this->direction = direction;
    this->inner_cut_off = inner_cut_off;
    this->outer_cut_off = outer_cut_off;
    this->constant = 1.0f;
    this->linear = 0.09f;
    this->quadratic = 0.032f;
}
SpotLight::~SpotLight() {}
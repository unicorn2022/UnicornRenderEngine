#version 460 core
#extension GL_ARB_shading_language_include : enable

/* Phong 材质*/
struct PhongMaterial {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
/* 定向光源 */
struct DirectLight {
    // 光源颜色
    vec3 ambient;   // 00-04N
    vec3 diffuse;   // 05-08N
    vec3 specular;  // 09-12N
    // 光源属性
    vec3 direction; // 13-16N
    // 共16N
};
/* 点光源 */
struct PointLight {
    // 光源颜色
    vec3 ambient;   // 00-04N
    vec3 diffuse;   // 05-08N
    vec3 specular;  // 09-12N
    // 光源属性
    vec3 position;  // 13-16N
    // 衰减
    float constant; // 17N
    float linear;   // 18N
    float quadratic;// 19N
    // 共20N
};
/* 聚光源 */
struct SpotLight {
    // 光源颜色
    vec3 ambient;   // 00-04N 
    vec3 diffuse;   // 05-08N
    vec3 specular;  // 09-12N
    // 光源属性
    vec3 position;  // 13-16N
    vec3 direction; // 17-19N
    float inner_cut_off; // 20N
    float outer_cut_off; // 21N   
    // 衰减
    float constant; // 22N
    float linear;   // 23N
    float quadratic;// 24N
    // 共24N
};

/* 光源计算函数 */
vec3 CalcDirectLight(DirectLight light, vec3 normal_dir, vec3 view_dir);
vec3 CalcPointLight(PointLight light, vec3 normal_dir, vec3 frag_position, vec3 view_dir);
vec3 CalcSpotLight(SpotLight light, vec3 normal_dir, vec3 frag_position, vec3 view_dir);

/* 输入输出变量 */
out vec4 FragColor;
in VS_OUT {
    vec3 Position;
    vec3 Normal;
    vec2 TexCoord;
    vec3 ViewPosition;
} fs_in;

/* uniform 变量 */
#define MAX_POINT_LIGHT_COUNT 4
layout (std140, binding = 1) uniform Light {
    DirectLight direct_light;
    PointLight point_lights[MAX_POINT_LIGHT_COUNT];
    SpotLight spot_light;
};
// 材质
uniform PhongMaterial material;
// 是否使用 Blinn-Phong 模型
uniform int use_blinn_phong = 1;

void main() {
    vec3 normal_dir = normalize(fs_in.Normal);
    vec3 view_dir = normalize(fs_in.ViewPosition - fs_in.Position);
    float alpha = texture(material.diffuse, fs_in.TexCoord).a;

    // 计算三种光照
    vec3 color = CalcDirectLight(direct_light, normal_dir, view_dir);
    for(int i = 0; i < MAX_POINT_LIGHT_COUNT; i++)
        color += CalcPointLight(point_lights[i], normal_dir, fs_in.Position, view_dir);
    color += CalcSpotLight(spot_light, normal_dir, fs_in.Position, view_dir);

    FragColor = vec4(color, alpha);
}

vec3 CalcDirectLight(DirectLight light, vec3 normal_dir, vec3 view_dir) {
    // 1. 环境光
    vec3 ambient = light.ambient * texture(material.diffuse, fs_in.TexCoord).rgb;

    // 2. 漫反射光
    vec3 light_dir = normalize(-light.direction);
    vec3 diffuse = max(dot(normal_dir, light_dir), 0.0) * light.diffuse * texture(material.diffuse, fs_in.TexCoord).rgb;

    // 3. 高光
    vec3 specular;
    if (use_blinn_phong == 0) {
        vec3 reflect_dir = reflect(-light_dir, normal_dir);
        specular = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess)  * light.specular * texture(material.specular, fs_in.TexCoord).rgb;
    } else {
        vec3 half_way_dir = normalize(light_dir + view_dir);
        specular = pow(max(dot(normal_dir, half_way_dir), 0.0), material.shininess)  * light.specular * texture(material.specular, fs_in.TexCoord).rgb;
    }

    // 最终颜色
    vec3 color = ambient + diffuse + specular;
    return color;
}

vec3 CalcPointLight(PointLight light, vec3 normal_dir, vec3 frag_position, vec3 view_dir) {
    // 1. 环境光
    vec3 ambient = light.ambient * texture(material.diffuse, fs_in.TexCoord).rgb;

    // 2. 漫反射光
    vec3 light_dir = normalize(light.position - frag_position);
    vec3 diffuse = max(dot(normal_dir, light_dir), 0.0) * light.diffuse * texture(material.diffuse, fs_in.TexCoord).rgb;

    // 3. 高光
    vec3 specular;
    if (use_blinn_phong == 0) {
        vec3 reflect_dir = reflect(-light_dir, normal_dir);
        specular = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess)  * light.specular * texture(material.specular, fs_in.TexCoord).rgb;
    } else {
        vec3 half_way_dir = normalize(light_dir + view_dir);
        specular = pow(max(dot(normal_dir, half_way_dir), 0.0), material.shininess)  * light.specular * texture(material.specular, fs_in.TexCoord).rgb;
    }
    
    // 4. 衰减
    float dist = length(light.position - frag_position);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);

    // 最终颜色
    vec3 color = ambient + diffuse + specular;
    return color * attenuation;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal_dir, vec3 frag_position, vec3 view_dir) {
    // 1. 环境光
    vec3 ambient = light.ambient * texture(material.diffuse, fs_in.TexCoord).rgb;

    // 2. 漫反射光
    vec3 light_dir = normalize(light.position - frag_position);
    vec3 diffuse = max(dot(normal_dir, light_dir), 0.0) * light.diffuse * texture(material.diffuse, fs_in.TexCoord).rgb;

    // 3. 高光
    vec3 specular;
    if (use_blinn_phong == 0) {
        vec3 reflect_dir = reflect(-light_dir, normal_dir);
        specular = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess)  * light.specular * texture(material.specular, fs_in.TexCoord).rgb;
    } else {
        vec3 half_way_dir = normalize(light_dir + view_dir);
        specular = pow(max(dot(normal_dir, half_way_dir), 0.0), material.shininess)  * light.specular * texture(material.specular, fs_in.TexCoord).rgb;
    }

    // 4. 衰减
    float dist = length(light.position - frag_position);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);

    // 5. 聚光效果
    float theta = dot(light_dir, normalize(-light.direction));
    float epsilon = light.inner_cut_off - light.outer_cut_off;
    float instensity = clamp((theta - light.inner_cut_off) / epsilon, 0.0, 1.0);

    // 最终颜色
    vec3 color = ambient + diffuse + specular;
    return color * attenuation * instensity;
}
#version 460 core
#extension GL_ARB_shading_language_include : enable

#define MAX_DIRECT_LIGHT_COUNT 2
#define MAX_POINT_LIGHT_COUNT 4
#define MAX_SPOT_LIGHT_COUNT 2

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
vec3 CalcDirectLight(DirectLight light, vec3 normal_dir, vec3 view_dir, float shadow);
vec3 CalcPointLight(PointLight light, vec3 normal_dir, vec3 frag_position, vec3 view_dir, float shadow);
vec3 CalcSpotLight(SpotLight light, vec3 normal_dir, vec3 frag_position, vec3 view_dir, float shadow);

/* 阴影计算函数 */
float CalcDirectLightShadow(int index, vec4 light_space_position, float bias);

/* 输入输出变量 */
out vec4 FragColor;
in VS_OUT {
    vec3 Position;
    vec3 Normal;
    vec2 TexCoord;
    vec3 ViewPosition;
    // 光源视角的坐标
    vec4 direct_light_position[MAX_DIRECT_LIGHT_COUNT];
    vec4 point_light_position[MAX_POINT_LIGHT_COUNT];
    vec3 debug_color;
} fs_in;

/* uniform 变量 */
layout (std140, binding = 1) uniform Light {
    // 1. 光源数据
    DirectLight direct_lights[MAX_DIRECT_LIGHT_COUNT];
    PointLight point_lights[MAX_POINT_LIGHT_COUNT];
    SpotLight spot_lights[MAX_SPOT_LIGHT_COUNT];
    // 2. 使用的光源个数
    int use_direct_light_num;
    int use_point_light_num;
    int use_spot_light_num;
};

// 阴影贴图
uniform sampler2D direct_light_shadow_map_0;
uniform sampler2D direct_light_shadow_map_1;
uniform sampler2D point_light_shadow_map_0;
uniform sampler2D point_light_shadow_map_1;
uniform sampler2D point_light_shadow_map_2;
uniform sampler2D point_light_shadow_map_3;

// 材质
uniform PhongMaterial material;
// 是否使用 Blinn-Phong 模型
uniform int use_blinn_phong = 1;

const float SHADOW_BIAS_MIN = 0.00001; // 阴影计算 bias: 最小值
const float SHADOW_BIAS_MAX = 0.00001; // 阴影计算 bias: 最大值
const int PCF_SAMPLE_RANGE = 1;      // PCF采样范围 [-x, x]
const int PCF_SAMPLE_NUM = (PCF_SAMPLE_RANGE * 2 + 1) * (PCF_SAMPLE_RANGE * 2 + 1); // PCF采样点个数


void main() {
    vec3 normal_dir = normalize(fs_in.Normal);
    vec3 view_dir = normalize(fs_in.ViewPosition - fs_in.Position);
    float alpha = texture(material.diffuse, fs_in.TexCoord).a;

    /* 计算三种光照 */
    vec3 color = vec3(0.0f);
    // 1. 方向光
    for(int i = 0; i < use_direct_light_num; i++) {
        vec3 light_dir = normalize(direct_lights[i].direction);
        float bias = (SHADOW_BIAS_MAX - SHADOW_BIAS_MIN) * abs(1.0 - dot(normal_dir, light_dir)) + SHADOW_BIAS_MIN;
        float shadow = CalcDirectLightShadow(i, fs_in.direct_light_position[i], bias);
        color += CalcDirectLight(direct_lights[i], normal_dir, view_dir, shadow);
    }
    // 2. 点光源
    for(int i = 0; i < use_point_light_num; i++)
        color += CalcPointLight(point_lights[i], normal_dir, fs_in.Position, view_dir, 0.0);
    // 3. 聚光源
    for(int i = 0; i < use_spot_light_num; i++)
        color += CalcSpotLight(spot_lights[i], normal_dir, fs_in.Position, view_dir, 0.0);

    FragColor = vec4(color, alpha);
}

vec3 CalcDirectLight(DirectLight light, vec3 normal_dir, vec3 view_dir, float shadow) {
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
    vec3 color = ambient + (1 - shadow) * (diffuse + specular);
    return color;
}

vec3 CalcPointLight(PointLight light, vec3 normal_dir, vec3 frag_position, vec3 view_dir, float shadow) {
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

vec3 CalcSpotLight(SpotLight light, vec3 normal_dir, vec3 frag_position, vec3 view_dir, float shadow) {
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

float CalcDirectLightShadow(int index, vec4 light_space_position, float bias) {
    /* 执行透视除法, 计算像素点在光源视角下的坐标 */
    vec3 tex_coord = light_space_position.xyz / light_space_position.w;
    tex_coord = tex_coord * 0.5 + 0.5;
    float current_depth = tex_coord.z;
    if (current_depth > 1.0) return 0.0;

    /* 计算阴影值 */
    // 1.1 计算阴影贴图的单个像素大小
    vec2 texel_size = vec2(1.0);
    if (index == 0) texel_size = 1.0 / textureSize(direct_light_shadow_map_0, 0);
    else if(index == 1) texel_size = 1.0 / textureSize(direct_light_shadow_map_1, 0);
    // 1.2 PCF采样阴影贴图
    float shadow = 0.0;
    for (int x = -PCF_SAMPLE_RANGE; x <= PCF_SAMPLE_RANGE; x++) {
        for (int y = -PCF_SAMPLE_RANGE; y <= PCF_SAMPLE_RANGE; y++) {
            // 取得当前点的深度
            float shadow_depth;
            if (index == 0) shadow_depth = texture(direct_light_shadow_map_0, tex_coord.xy + vec2(x, y) * texel_size).r;
            else if(index == 1) shadow_depth = texture(direct_light_shadow_map_1, tex_coord.xy + vec2(x, y) * texel_size).r;
            shadow += current_depth - bias > shadow_depth ? 1.0 : 0.0;
        }
    }
    shadow /= PCF_SAMPLE_NUM;
    // FragColor = vec4(shadow, texel_size.x, texel_size.y, 1.0);
    return shadow;
}
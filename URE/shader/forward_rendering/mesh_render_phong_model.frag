#version 460 core
#extension GL_ARB_shading_language_include : enable

#define MAX_DIRECT_LIGHT_COUNT 2
#define MAX_POINT_LIGHT_COUNT 4
#define MAX_SPOT_LIGHT_COUNT 2

/* Phong 材质*/
struct PhongMaterial {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
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
vec3 CalcDirectLight(DirectLight light, vec3 normal_dir, vec3 view_dir, vec3 color_diffuse, vec3 color_specular, float shadow);
vec3 CalcPointLight(PointLight light, vec3 normal_dir, vec3 frag_position, vec3 view_dir, vec3 color_diffuse, vec3 color_specular, float shadow);
vec3 CalcSpotLight(SpotLight light, vec3 normal_dir, vec3 frag_position, vec3 view_dir, vec3 color_diffuse, vec3 color_specular, float shadow);

/* 阴影计算函数 */
float CalcDirectLightShadow(int index, vec4 light_space_position, float bias);
float CalcPointLightShadow(int index);

/* 输入输出变量 */
out vec4 FragColor;
in VS_OUT {
    // 位置、纹理坐标、观察点
    vec3 Position;
    vec2 TexCoord;
    vec3 ViewPosition;
    // 法线、切线、副切线
    vec3 Normal;
    vec3 Tangent;
    vec3 Bitangent;
    // 光源相关
    vec4 direct_light_position[MAX_DIRECT_LIGHT_COUNT]; // 定向光源视角的坐标
    // 调试相关
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
uniform samplerCube point_light_shadow_map_0;
uniform samplerCube point_light_shadow_map_1;
uniform samplerCube point_light_shadow_map_2;
uniform samplerCube point_light_shadow_map_3;

// 材质
uniform PhongMaterial material;
// 是否使用 Blinn-Phong 模型
uniform int use_blinn_phong;
// 是否显示阴影
uniform int show_render_shadow;
// 是否使用高光贴图
uniform int use_specular_map;
// 是否使用法线贴图
uniform int use_normal_map;

const float SHADOW_BIAS_MIN = 0.0001; // 阴影计算 bias: 最小值
const float SHADOW_BIAS_MAX = 0.0001; // 阴影计算 bias: 最大值
const int PCF_SAMPLE_RANGE = 1;      // PCF采样范围 [-x, x]
const int PCF_SAMPLE_DIRECT_NUM = (PCF_SAMPLE_RANGE * 2 + 1) * (PCF_SAMPLE_RANGE * 2 + 1); // PCF采样点个数
const float POINT_LIGHT_SHADOW_ZFAR = 100.0f;
const int PCF_SAMPLE_POINT_NUM = 21;
const vec3 PCF_SAMPLE_POINT_DIRECTIONS[21] = {
    vec3( 0,  0,  0),
    vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
    vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
    vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
    vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
    vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
};

void main() {
    /* 计算观察方向 */
    vec3 view_dir = normalize(fs_in.ViewPosition - fs_in.Position);
    /* 计算透明度 */
    float alpha = texture(material.diffuse, fs_in.TexCoord).a;
    /* 采样 diffuse 贴图*/
    vec3 color_diffuse = texture(material.diffuse, fs_in.TexCoord).rgb;
    /* 采样 specular 贴图 */
    vec3 color_specular = vec3(0.0, 0.0, 0.0);
    if (use_specular_map == 1) color_specular = texture(material.specular, fs_in.TexCoord).rgb;
    /* 采样 normal 贴图 */
    vec3 normal_dir;
    if (use_normal_map == 1) {
        // 采样法线, 法线定义在切线空间
        normal_dir = texture(material.normal, fs_in.TexCoord).rgb;
        normal_dir = normalize(normal_dir * 2.0 - 1.0);
        // 计算世界坐标系下的TBN矩阵
        vec3 T = normalize(fs_in.Tangent);
        vec3 B = normalize(fs_in.Bitangent);
        vec3 N = normalize(fs_in.Normal);
        mat3 TBN = mat3(T, B, N);
        // 将法线通过TBN变换到世界坐标系
        normal_dir = normalize(TBN * normal_dir);
    } else {
        normal_dir = normalize(fs_in.Normal);
    }
    
    /* 计算三种光照 */
    vec3 color = vec3(0.0f);
    // 1. 方向光
    for(int i = 0; i < use_direct_light_num; i++) {
        vec3 light_dir = normalize(direct_lights[i].direction);
        if (show_render_shadow == 1) {
            float bias = (SHADOW_BIAS_MAX - SHADOW_BIAS_MIN) * abs(1.0 - dot(normal_dir, light_dir)) + SHADOW_BIAS_MIN;
            float shadow = CalcDirectLightShadow(i, fs_in.direct_light_position[i], bias);
            color += CalcDirectLight(direct_lights[i], normal_dir, view_dir, color_diffuse, color_specular, shadow);
        } else {
            color += CalcDirectLight(direct_lights[i], normal_dir, view_dir, color_diffuse, color_specular, 0.0);
        }
    }
    // 2. 点光源
    for(int i = 0; i < use_point_light_num; i++) {
        if (show_render_shadow == 1) {
            float shadow = CalcPointLightShadow(i);
            color += CalcPointLight(point_lights[i], normal_dir, fs_in.Position, view_dir, color_diffuse, color_specular, shadow);
        } else {
            color += CalcPointLight(point_lights[i], normal_dir, fs_in.Position, view_dir, color_diffuse, color_specular, 0.0);
        }
    }
    // 3. 聚光源
    for(int i = 0; i < use_spot_light_num; i++)
        color += CalcSpotLight(spot_lights[i], normal_dir, fs_in.Position, view_dir, color_diffuse, color_specular, 0.0);

    FragColor = vec4(color, alpha);
}

vec3 CalcDirectLight(DirectLight light, vec3 normal_dir, vec3 view_dir, vec3 color_diffuse, vec3 color_specular, float shadow) {
    // 1. 环境光
    vec3 ambient = light.ambient * color_diffuse;

    // 2. 漫反射光
    vec3 light_dir = normalize(-light.direction);
    vec3 diffuse = max(dot(normal_dir, light_dir), 0.0) * light.diffuse * color_diffuse;

    // 3. 高光
    vec3 specular;
    if (use_blinn_phong == 0) {
        vec3 reflect_dir = reflect(-light_dir, normal_dir);
        specular = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess)  * light.specular * color_specular;
    } else {
        vec3 half_way_dir = normalize(light_dir + view_dir);
        specular = pow(max(dot(normal_dir, half_way_dir), 0.0), material.shininess)  * light.specular * color_specular;
    }

    // 最终颜色
    vec3 color = ambient + (1 - shadow) * (diffuse + specular);
    return color;
}

vec3 CalcPointLight(PointLight light, vec3 normal_dir, vec3 frag_position, vec3 view_dir, vec3 color_diffuse, vec3 color_specular, float shadow) {
    // 1. 环境光
    vec3 ambient = light.ambient * color_diffuse;

    // 2. 漫反射光
    vec3 light_dir = normalize(light.position - frag_position);
    vec3 diffuse = max(dot(normal_dir, light_dir), 0.0) * light.diffuse * color_diffuse;

    // 3. 高光
    vec3 specular;
    if (use_blinn_phong == 0) {
        vec3 reflect_dir = reflect(-light_dir, normal_dir);
        specular = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess)  * light.specular * color_specular;
    } else {
        vec3 half_way_dir = normalize(light_dir + view_dir);
        specular = pow(max(dot(normal_dir, half_way_dir), 0.0), material.shininess)  * light.specular * color_specular;
    }
    
    // 4. 衰减
    float dist = length(light.position - frag_position);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);

    // 最终颜色
    vec3 color = ambient + (1 - shadow) * (diffuse + specular);
    return color * attenuation;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal_dir, vec3 frag_position, vec3 view_dir, vec3 color_diffuse, vec3 color_specular, float shadow) {
    // 1. 环境光
    vec3 ambient = light.ambient * color_diffuse;

    // 2. 漫反射光
    vec3 light_dir = normalize(light.position - frag_position);
    vec3 diffuse = max(dot(normal_dir, light_dir), 0.0) * light.diffuse * color_diffuse;

    // 3. 高光
    vec3 specular;
    if (use_blinn_phong == 0) {
        vec3 reflect_dir = reflect(-light_dir, normal_dir);
        specular = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess)  * light.specular * color_specular;
    } else {
        vec3 half_way_dir = normalize(light_dir + view_dir);
        specular = pow(max(dot(normal_dir, half_way_dir), 0.0), material.shininess)  * light.specular * color_specular;
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
            if (index == 1) shadow_depth = texture(direct_light_shadow_map_1, tex_coord.xy + vec2(x, y) * texel_size).r;

            // 计算阴影值
            if (abs(current_depth - shadow_depth) > bias && current_depth - shadow_depth > bias)
                shadow += 1.0;
        }
    }
    shadow /= PCF_SAMPLE_DIRECT_NUM;
    return shadow;
}

float CalcPointLightShadow(int index) {
    const float bias = 0.0005;
    vec3 frag_to_light = fs_in.Position - point_lights[index].position;
    float current_depth = length(frag_to_light) / POINT_LIGHT_SHADOW_ZFAR; // 映射到[0, 1]之间

    float view_distance = length(fs_in.ViewPosition - fs_in.Position);
    float disk_radius = (1.0 + (view_distance / POINT_LIGHT_SHADOW_ZFAR)) / 25.0;
    float shadow = 0.0;
    for (int i = 0; i < PCF_SAMPLE_POINT_NUM; i++) {
        // 取得当前点的深度
        float shadow_depth = 1.0;
        if (index == 0) shadow_depth = texture(point_light_shadow_map_0, frag_to_light + PCF_SAMPLE_POINT_DIRECTIONS[i] * disk_radius).r;
        if (index == 1) shadow_depth = texture(point_light_shadow_map_1, frag_to_light + PCF_SAMPLE_POINT_DIRECTIONS[i] * disk_radius).r;
        if (index == 2) shadow_depth = texture(point_light_shadow_map_2, frag_to_light + PCF_SAMPLE_POINT_DIRECTIONS[i] * disk_radius).r;
        if (index == 3) shadow_depth = texture(point_light_shadow_map_3, frag_to_light + PCF_SAMPLE_POINT_DIRECTIONS[i] * disk_radius).r;
        if (abs(current_depth - shadow_depth) > bias && current_depth - shadow_depth > bias)
            shadow += 1.0;
    }
    shadow /= PCF_SAMPLE_POINT_NUM;
    return shadow;
}
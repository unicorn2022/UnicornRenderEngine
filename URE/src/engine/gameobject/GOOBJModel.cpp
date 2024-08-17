#include "engine/gameobject/GOOBJModel.h"
#include "engine/mesh/MeshOBJ.h"
#include "engine/material/MaterialPhongLight.h"
#include "engine/component/ALL.h"


#include "engine/mesh/ALL.h"
#include "engine/material/ALL.h"

GOOBJModel::GOOBJModel(std::string name, std::string file_name, Material* material, unsigned int num, bool is_debug) : GO(name) {
    for (int i = 0; i < num; i++)
        AddComponent(new ComponentTransform(this));

    /* 记录OBJ模型的所有 submesh */
    auto model = new OBJModel(root_path_model, file_name);
    std::vector<Mesh*> meshs;
    std::vector<Material*> materials;
    for (int i = 0; i < model->sub_meshs.size(); i++) {
        meshs.push_back(model->sub_meshs[i]);
        if (material != NULL) materials.push_back(material);
        else materials.push_back(new MaterialPhongLight(model->sub_meshs_diffuse[i], model->sub_meshs_specular[i]));
    }
    AddComponent(new ComponentMesh(this, meshs, materials, is_debug));
}
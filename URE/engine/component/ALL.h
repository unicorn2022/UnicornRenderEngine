#pragma once
#include "engine/component/Component.h" // 依赖于 basic/ALL.h, mesh/Mesh.h, material/ALL.h
#include "engine/component/ComponentTransform.h"
#include "engine/component/ComponentLight.h"    
#include "engine/component/ComponentMesh.h"     // 依赖于 ComponentTransform.h, ComponentLight.h
#include "engine/component/ComponentCamera.h"   // 依赖于 ComponentTransform.h, ComponentLight.h, ComponentMesh.h
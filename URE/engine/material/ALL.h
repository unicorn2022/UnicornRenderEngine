#pragma once
#include "engine/material/Material.h" // 依赖于 basic/Shader.h
#include "engine/material/MaterialConstantColor.h"  // 依赖于 Material.h
#include "engine/material/MaterialDepth.h"          // 依赖于 Material.h
#include "engine/material/MaterialNoLight.h"        // 依赖于 Material.h, basic/Texture.h
#include "engine/material/MaterialPhongLight.h"     // 依赖于 Material.h, basic/Texture.h, basic/Light.h
#include "engine/material/MaterialPostProcess.h"    // 依赖于 Material.h, basic/Texture.h
#include "engine/material/MaterialSkybox.h"         // 依赖于 Material.h, basic/TextureCube.h
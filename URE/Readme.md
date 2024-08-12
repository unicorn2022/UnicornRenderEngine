[TOC]

# 一、URE引擎架构

## 1.0	全局类

### 1.0.1	Utils.h

**Utils**：定义工具类，以及对第三方库的引用

### 1.0.2	GlobalValue.h

**GlobalValue**：定义全局属性信息

### 1.0.3	GameWorld.h

整个场景由多个游戏对象GO组成，所有游戏对象由**GameWorld**统一管理

### 1.0.4	GameComponent.h

每个GO会有多个组件，组件表示游戏对象的功能，所有组件的引用存放在**GameComponent**，由GO销毁

## 1.1	基础类 Basic

定义基础数据类型，封装OpenGL接口

### 1.1.1	Camera.h

相机类

- **Camera**：相机接口，负责提供`view, projection`矩阵
- **RoamingCamera**：漫游相机，通过`position, yaw, pitch`控制相机移动
- **RoamingCameraPerspective**：透视投影相机类，引入`aspect, fov, near, far`属性

### 1.1.2	FrameBuffer.h

> **TODO：自定义宽高**

帧缓冲类，封装了一个帧缓冲，包含2个附件

- 一个颜色附件：纹理对象，`color_texture`
- 一个深度模板附件：渲染缓冲对象，仅存储ID

### 1.1.3	Light.h

光源类

- **Light**：不同光源的基类，定义了光源的颜色`ambient, diffuse, specular`
- **DirectLight**：定向光源，引入`direction`属性
- **PointLight**：点光源，引入`position`属性，衰减为`1 + 0.09r + 0.032r^2`
- **SpotLight**：聚光源，引入`position, direction, inner_cut_off, outer_cut_off`属性，衰减为`1 + 0.09r + 0.032r^2`

### 1.1.4	Shader.h

着色器类，封装OpenGL中与着色器有关的接口

- 通过传入着色器名称`name`，找到对应的shader源代码：`name.vert, name.frag`
- 在渲染`mesh`之前，调用`material->Use()`，此时会首先调用`shader->Use()`
- 通过`SetUniform()`设置shader中`uniform`变量的值

### 1.1.5	Texture.h

纹理类，封装OpenGL中与纹理有关的接口

- 通过传入纹理文件名`file_name`，找到对应的图片：`root_directory + file_name`
- 如果材质包含纹理，则在`material->Use()`中，需要进行如下步骤

```c++
tex->Use(index);
shader->SetUniform("tex", index);
```

### 1.1.6	TextureCube.h

立方纹理类，封装OpenGL中与立方纹理有关的接口

- 通过传入6张纹理文件名`file_name[6]`，找到对应的图片：`root_directory + file_name`
- 如果材质包含立方纹理，则在`material->Use()`中，需要进行如下步骤

```c++
tex->Use(index);
shader->SetUniform("tex", index);
```

## 1.2	组件类 Component

定义不同种组件，组件表示游戏对象的功能。

组件依赖于游戏对象而存在，游戏对象消失时自动销毁其所有组件。

### 1.2.1	Component.h

组件的基类，存放组件所属的游戏对象

### 1.2.2	ComponentCamera.h

相机组件，表示摄像机的功能

- 一个透视投影相机`camera`
- 一个帧缓冲`frame_buffer`：存放每个tick，该相机的渲染结果

接口信息：

- 每一个`GameTick`，调用`UpdateCameraState()`更新相机状态
- 每一个`RenderTick`，调用`RenderTick()`进行渲染
- 处理**滚轮回调**事件时，调用`ProcessMouseScroll()`更新相机的`fov`

### 1.2.3	ComponentLight.h

光源组件，表示光源的功能

- 光源数据`light_data`

### 1.2.4	ComponentMesh.h

网格体组件，表示游戏对象的可见性

- 网格体的顶点属性：`mesh`
- 网格体的材质：`material`
- 是否为半透明物体：`is_transport`

接口信息：

- `Draw()`：绘制该网格体，需要传入相机和光源数据

### 1.2.5	ComponentTransform.h

变换组件，表示游戏对象的变换信息，提供`model`矩阵

- `position`：位置
- `rotate`：XYZ轴的旋转
- `scale`：XYZ轴的缩放
- `parent`：维护父子关系
- `move_speed`：移动速度

接口信息：

- `GetModelMatrix()`：获取`model`矩阵
- 处理**键盘回调**事件时，调用`ProcessKeyboard()`更新GO的位置
- 处理**鼠标回调**事件时，调用`ProcessMouseMovement()`更新GO的朝向

## 1.3	内建游戏对象 GameObject

定义一些游戏对象，主要是定义GO的组件

### 1.3.1	GO.h

游戏对象的基类

- 维护当前GO包含的组件
- 在GO被销毁时，销毁组件

### 1.3.2	GOCamera

相机游戏对象，包含：

- 一个`Transform`组件
- 一个`Camera`组件

### 1.3.3	GOCube

立方体游戏对象，包含：

- 一个`Transform`组件
- 一个`Mesh`组件，形状为`cube`

### 1.3.3	GOLight

光源游戏对象，包含：

- 一个`Transform`组件
- 一个`Light`组件

### 1.3.4	GOSkybox

天空盒游戏对象，包含：

- 一个`Transform`组件
- 一个`Mesh`组件，形状为`cube`

### 1.3.5	GOSquare

长方形游戏对象，包含：

- 一个`Transform`组件
- 一个`Mesh`组件，形状为`square`

## 1.4	材质 Material

定义一些材质，每一个材质对应唯一一个`Shader`，在销毁材质时销毁`Shader`

### 1.4.1	Material.h

材质的接口类，包含对`Shader`的引用，在销毁材质时销毁`Shader`

### 1.4.2	MaterialConstantColor.h

固定颜色材质

- 变化信息：`model, view, projection`
- 材质信息：`color`

### 1.4.3	MaterialDepth.h

深度可视化

- 变化信息：`model, view, projection`
- 材质信息：`z_near, z_far `

### 1.4.4	MaterialNoLight.h

无光照材质，仅显示diffuse贴图

- 变化信息：`model, view, projection`
- 材质信息：`diffuse`

### 1.4.5	MaterialPhongLight.h

Phong光照材质

- 变化信息：`model, view, projection`
- 光照信息：`direct_light, point_lights[MAX_POINT_LIGHT_COUNT], spot_light`
- 材质信息：`diffuse, specular, shininess`

### 1.4.6	MaterialPostProcess.h

后处理材质

- 材质信息：`screen_texture, choose_post_process`

### 1.4.7	MaterialDepth.h

天空盒可视化

- 变化信息：`view, projection`
- 材质信息：`skybox_texture `

## 1.5	网格体 Mesh

存储网格体的顶点信息

### 1.5.1	Mesh.h

网格体的接口类，定义了`Draw()`接口

- 绘制之前应先调用`Material->Use()`

### 1.5.2	MeshCube

标准正方体：`[-1, 1]^3`

### 1.5.3	MeshOBJ

从文件中加载OBJ模型

### 1.5.4	MeshSquare

标准正方形：`[-1, 1]^2`

# 二、URE引擎绘制过程

## 2.1	定义游戏对象&游戏逻辑：`scene.cpp`

- 实现`GameWorld::GameWorld()`：定义游戏对象

- 实现`GameWorld::GameTick()`：定义游戏逻辑

## 2.2	渲染过程：`main.cpp`

- 初始化OpenGL：`InitOpenGL()`
- 创建屏幕对象`screen`
  - 网格体形状为：`MeshSquare()`
  - 网格体材质为：`MaterialPostProcess(NULL)`
- 执行渲染循环：`Run()`
  - 计算帧间隔
  - 处理输入，并调用`GameTick()`
  - 对于所有的相机组件，调用`camera_component->RenderTick()`
  - 将`main_camera`的帧缓冲绑定到`screen`上，并绘制屏幕对象
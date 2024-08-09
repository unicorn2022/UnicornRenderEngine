[TOC]

# UnicornRenderEngine

基于OpenGL3.3的渲染引擎

# 一、引擎架构

## 1.1	总览

- 整个场景由多个游戏对象GO组成
- 每个GO会有多个组件

单例模式

- **GlobalValue**单例：存放全局配置信息
- **GameWorld**单例：管理所有的Component、Mesh、Material、GO

## 1.2	Basic

- **light**：光源
- **shader**：从文件中加载着色器，并编译
- **texture**：从文件中加载纹理，并分配空间
- **textureCube**：从文件中加载立方纹理，并分配空间
- **framebuffer**：创建帧缓冲，大小自定义
  - 包含：一个颜色附件，一个深度模板附件

## 1.3	网格体 Mesh：依赖于`basic/Texture.h`

包含一个形状的VAO，顶点属性为：位置、法线、纹理坐标

- **MeshSquare**：四边形
- **MeshCube**：立方体
- **MeshOBJ**：从文件中导入的obj模型
  - 包含：多个**MeshOBJSubMesh**

## 1.4	材质 Material：依赖于`basic/ALL.h`

每个材质绑定唯一shader程序，纹理的改变放在mesh组件中

- **MaterialConstantColor**：指定颜色
- **MaterialDepth**：深度可视化
- **MaterialNoLight**：仅显示diffuse贴图
- **MaterialPhongLight**：phong光照模型材质
- **MaterialPostProcess**：后处理材质，仅用于帧缓冲
- **MaterialSkybox**：天空盒材质

## 1.5	组件 Component：依赖于`basic/ALL.h, material/ALL.h, mesh/Mesh.h`

每次tick，只tick组件

- **ComponentCamera**：表示观察者所在位置。
  - 包含：虚拟相机，帧缓冲
  - 每个相机观察到的图片存储在帧缓冲中
  - 生成最终的project，view矩阵
- **ComponenTransform**：表示GO的位置，旋转，缩放，父子关系。
  - 旋转：绕XYZ轴分别旋转了多少度
  - 生成最终的model矩阵。
- **ComponentMesh**：表示GO的可见形状
  - 包含：VAO，material，texture
- **ComponentLight**：表示发光体
  - 包含：光源类型

## 1.6	内建游戏对象 GO：依赖于`component/ALL.h`

管理者为GameWorld

- **GOSquare**：四边形
  - 包含mesh组件，transform组件
- **GOSquare**：立方体
  - 包含mesh组件，transform组件
- **GONanosuit**：机器人
  - 包含mesh组件，transform组件
- **GOSkybox**：天空盒
  - 包含mesh组件（cube），材质为天空盒材质
- **GOCamera**：摄像机
  - 包含camera组件，transform组件

# 二、绘制过程

- 配置，添加物体到GameWorld中
- 对每个带有camera组件的GO，渲染GameWorld中带有Mesh组件的GO，结果存储在camera组件的帧缓冲中
- 选中一个camera，将其帧缓冲绘制到屏幕上

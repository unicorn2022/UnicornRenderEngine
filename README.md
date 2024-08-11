[TOC]

# UnicornRenderEngine

基于OpenGL3.3的渲染引擎

# 一、知识点

## 1.1	OpenGL渲染测试的顺序

顶点着色器 => 面剔除 => 提前深度测试 => 片段着色器 => 模板测试 => 深度测试

- 使用**提前深度测试**时，片段着色器就不能写入片段的深度值
- **面剔除**：逆时针方向的三角形，视为正向三角形
  - 环绕顺序是在**光栅化**阶段进行的，此时顶点就是**观察者视角**所见的


# 二、环境配置

## 2.1	对VSCode的配置

### 2.1.1	设置编译工具&构建工具

> ctrl + shift + p > cmake: scan for kits

- 扫描工具包

> ctrl + shift + p > cmake: select a kit

- 设置工具包为：**VS x86_amd64**

### 2.1.2	解决输出乱码问题

> ctrl + , > cmake: output log encoding

- 设置输出日志编码为：**utf-8**

### 2.1.3	解决代码补全问题

- 安装**C/C++**扩展
- 设置扩展的编译器路径为：**D:\Program\VS\Community\VC\Tools\MSVC\14.40.33807\bin\Hostx64\x64\cl.exe**
- 设置扩展的包含路径为：**E:\MyProject\GameEngine\OpenGL_Learning\3rd-party\\\*\***

### 2.1.4	解决终端乱码问题

- 在vscode的终端中，将编码临时修改为utf-8

```bash
chcp 650001
```

## 2.2	对ASSIMP库的配置

### 2.2.1	dll动态连接文件

将`assimp-vc143-mtd.dll`拷贝到`C:\Windows\System32`和`C:\Windows\SysWOW64`中

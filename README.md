[TOC]

# UnicornRenderEngine

基于OpenGL4.6的渲染引擎

# 一、运行方法

- 将`assimp-vc143-mtd.dll`拷贝到`C:\Windows\System32`和`C:\Windows\SysWOW64`中
- 使用CMake编译运行

# 二、环境配置

修改`URE/include/GlobalValue.h`中的`root_path`变量为`URE`目录的绝对地址

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

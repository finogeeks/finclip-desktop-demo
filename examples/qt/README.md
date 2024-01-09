# FinClipQtDemo is a cross-platform C++ project based on CMake.

# 总体思路介绍
1. 在编译机搭建好环境后, 首先下载 FinClip DeskTop SDK并解压后. 解压路径就是CMakeLists.txt中的FINCLIP_SDK_PATH
2. 不同系统编译所依赖的文件略有差异
    - windows 需要 头文件, .dll, .lib
    - Linux / Mac 需要 头文件, .so
3. 创建一个 cmake library, finclip_wrapper, 不同系统分别设置相关属性, CMakeLists.txt 中包含了编译通过的最少属性

# demo 使用方法

## 1. 下载 FinClip DeskTop SDK

[下载finclip二进制包](https://www.finclip.com/downloads/)到`vendor`的对应目录下，如`vendor/win/x86_64`并解压


## 2. 生成cmake 构建目录

下面以 mac 为例
需要注意指定目标架构 PROJECT_ARCH

- windows支持: x86_64, x86
- linux支持: x86_64, arm64
- mac支持: x86_64, arm64

具体以 CMakeLists.txt 为准
```shell
mkdir build
cd build
# 生成 x86_64 位的build
cmake .. -DPROJECT_ARCH=x86_64
```

## 3. 填写参数

准备好[从管理后台获取的参数](https://www.finclip.com/mop/document/introduce/functionDescription/application-management.html#_1-%E5%8A%9F%E8%83%BD%E4%BB%8B%E7%BB%8D)

```C
// 修改main.cc中的以下变量:
  const string appkey = "";
  const string secret = "";
  const string domain = "";
  const string appid = "";
```

设置exe_path
```C
// windows
const string exe_path = "path/to/FinClip.exe";

// Linux
const string exe_path = "path/to/FinClip";

// Macos
const string exe_path = "path/to/FinClip.app";
```

## 4. 编译项目并启动

下面以 mac 为例

```shell
cd build
make
./FinClipQtDemo
```
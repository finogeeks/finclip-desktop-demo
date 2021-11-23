<p align="center">
    <a href="https://www.finclip.com?from=github">
    <img width="auto" src="https://www.finclip.com/mop/document/images/logo.png">
    </a>
</p>

<p align="center"> 
    <strong>FinClip Windows DEMO</strong></br>
<p>
<p align="center"> 
        Windows 小程序 DEMO
<p>

<p align="center"> 
	👉 <a href="https://www.finclip.com?from=github">https://www.finclip.com/</a> 👈
</p>

<div align="center">

<a href="#"><img src="https://img.shields.io/badge/%E4%B8%93%E5%B1%9E%E5%BC%80%E5%8F%91%E8%80%85-20000%2B-brightgreen"></a>
<a href="#"><img src="https://img.shields.io/badge/%E5%B7%B2%E4%B8%8A%E6%9E%B6%E5%B0%8F%E7%A8%8B%E5%BA%8F-6000%2B-blue"></a>
<a href="#"><img src="https://img.shields.io/badge/%E5%B7%B2%E9%9B%86%E6%88%90%E5%B0%8F%E7%A8%8B%E5%BA%8F%E5%BA%94%E7%94%A8-75%2B-yellow"></a>
<a href="#"><img src="https://img.shields.io/badge/%E5%AE%9E%E9%99%85%E8%A6%86%E7%9B%96%E7%94%A8%E6%88%B7-2500%20%E4%B8%87%2B-orange"></a>

<a href="https://www.zhihu.com/org/finchat"><img src="https://img.shields.io/badge/FinClip--lightgrey?logo=zhihu&style=social"></a>
<a href="https://www.finclip.com/blog/"><img src="https://img.shields.io/badge/FinClip%20Blog--lightgrey?logo=ghost&style=social"></a>



</div>

<p align="center">

<div align="center">

[官方网站](https://www.finclip.com/) | [示例小程序](https://www.finclip.com/#/market) | [开发文档](https://www.finclip.com/mop/document/) | [部署指南](https://www.finclip.com/mop/document/introduce/quickStart/cloud-server-deployment-guide.html) | [SDK 集成指南](https://www.finclip.com/mop/document/introduce/quickStart/intergration-guide.html) | [API 列表](https://www.finclip.com/mop/document/develop/api/overview.html) | [组件列表](https://www.finclip.com/mop/document/develop/component/overview.html) | [隐私承诺](https://www.finclip.com/mop/document/operate/safety.html)

</div>

-----
## 🤔 FinClip 是什么?

有没有**想过**，开发好的微信小程序能放在自己的 APP 里直接运行，只需要开发一次小程序，就能在不同的应用中打开它，是不是很不可思议？

有没有**试过**，在自己的 APP 中引入一个 SDK ，应用中不仅可以打开小程序，还能自定义小程序接口，修改小程序样式，是不是觉得更不可思议？

这就是 FinClip ，就是有这么多不可思议！

## 🖥 FinClip SDK 是什么？
开发者可以使用 FinClip SDK 在宿主 APP 中快速实现小程序的能力。一般会用于以下场景：
- 宿主 APP 构建自己的应用生态，既可以面向广泛开发者，也可以定向部分开发者；
- 宿主 APP 通过小程序方式对模块进行解耦，让不同模块的开发团队独立发版，灵活更新；
- 宿主 APP 中部分功能使用 FinClip 小程序实现，降低开发成本，并且提升发布效率；

此外，SDK 还需要配合基础库一并使用，通过基础库提供的小程序生命周期钩子、API函数，页面 DOM创建、渲染以及事件传递等能力为用户提供更加良好的体验。

## 🤩 效果预览

**本项目是 FinClip 小程序在 Win32 环境下的 DEMO 演示，您可以按照下方流程测试，验证 FinClip 小程序在 Windows 环境下的实际效果。**

先看一下运行效果~

<p align="center">
    <a href="#">
    <img width="auto" src="./doc/images/450.gif">
    </a>
</p>

> 当前 Windows 版本 SDK 还在持续研发中，我们将及时同步桌面版本小程序的研发进度，如有需要请与我们联系。

## 📦 安装第三方依赖
Windows 小程序运行环境需要安装以下依赖，请提前配置环境：
- 请下载依赖包[点我下载](https://app.finogeeks.com/finclip-lib.tar.gz.bz2)
- 下载完后解压到vendor/finclip/lib目录中
- 建议使用 Visual Studio 2019 版本 [点我下载](https://support.microsoft.com/en-us/topic/the-latest-supported-visual-c-downloads-2647da03-1eea-4433-9aff-95f26a218cc0)

**注意：依赖包必须随缩主程序一同发布，并在独立目录中存放。**

解压后文件目录结构如下：

```
vendor
 |--finclip
 |   |--lib
 |        |--****
 |        |--****
 |        |--****
 ...
```


### 第一步 引入头文件

```c++
#include "finclip_wrapper.h" //引入头文件
#pragma comment(lib, "FinClipSDKWrapper.lib") //引入链接库
```

### 第二步 初始化SDK

```c++
  FinConfig config = {
		1,
		"https://api.finclip.com",
		"/api/v1/mop",
		"这里输入SDK KEY",
		"这里输入SDK SECRET",
		"",
		1
	};
	IFinConfigPacker* configpacker = NewFinConfigPacker();
	configpacker->AddConfig(config);
	Initialize(hInstance, configpacker);
```

- **SDK KEY** 和 **SDK SECRET** 可以从 [FinClip](https://finclip.com/#/home)  获取，点 [这里](https://finclip.com/#/register) 注册账号；
- 进入平台后，在「应用管理」页面添加你自己的包名后，点击「复制」即可获得  key\secret\apisever 字段；
- **apiServer** 和 **apiPrefix** 是固定字段，请直接参考本 DEMO ；
- **小程序 ID** 是管理后台上架的小程序 APP ID，需要在「小程序管理」中创建并在「应用管理」中关联；
> 小程序 ID 与 微信小程序ID 不一样哦！（这里是特指 FinClip 平台的 ID ）


### 第三步 打开小程序

```c++
  int server_type = 1;
  init_finclipsdk(server_type,wappkey, wsecret);
  IFinPacker* packer = NewFinPacker();
  packer->BeginPacker();
  packer->AddField("appId");
  packer->AddValue("appId");
  packer->EndPacker();
  StartApplet(server_type, utf8_encode(wappid).c_str(), packer, finclip_applet_callback);
  packer->Release();
```

### 查看 API 文档
您可以点击这里查看 [FinClipSDK WindowsAPI](https://docs.finogeeks.club/docs/finclip-win32/wrapper) 的开发文档

## 🔗 常用链接
以下内容是您在 FinClip 进行开发与体验时，常见的问题与指引信息

- [FinClip 官网](https://www.finclip.com/#/home)
- [示例小程序](https://www.finclip.com/#/market)
- [文档中心](https://www.finclip.com/mop/document/)
- [SDK 部署指南](https://www.finclip.com/mop/document/introduce/quickStart/intergration-guide.html)
- [小程序代码结构](https://www.finclip.com/mop/document/develop/guide/structure.html)
- [iOS 集成指引](https://www.finclip.com/mop/document/runtime-sdk/ios/ios-integrate.html)
- [Android 集成指引](https://www.finclip.com/mop/document/runtime-sdk/android/android-integrate.html)
- [Flutter 集成指引](https://www.finclip.com/mop/document/runtime-sdk/flutter/flutter-integrate.html)

## ☎️ 联系我们
微信扫描下面二维码，关注官方公众号 **「凡泰极客」**，获取更多精彩内容。<br>
<img width="150px" src="https://www.finclip.com/mop/document/images/ic_qr.svg">

微信扫描下面二维码，邀请进官方微信交流群（加好友备注：finclip 咨询），获取更多精彩内容。<br>
<img width="150px" src="https://finclip-homeweb-1251849568.cos.ap-guangzhou.myqcloud.com/images/ldy111.jpg">

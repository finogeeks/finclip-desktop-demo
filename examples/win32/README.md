## 📦 安装第三方依赖
Windows 小程序运行环境需要安装以下依赖，请提前配置环境：
- 请下载依赖包[点我下载](https://public-1251849568.cos.ap-guangzhou.myqcloud.com/sdk/lib.zip)
- 下载完后解压到vendor/finclip目录中
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
# finclip-electron-demo

## 通用步骤与功能介绍

 快速开始前请先阅读以下链接

[Finclip桌面版Demo集成指引与功能介绍](https://github.com/finogeeks/finclip-desktop-demo/tree/master/examples/README.md)

## 快速开始

```
npm i
npm run start
```

## 调用finclip api

1. 引入finclip依赖包

    注意，只能在electron的主进程使用

    ```
    const finclip = require('finclip');
    ```

2. 加载动态库

    ```
    finclip.load_library('xxx');
    ```

3. 打开finclip窗口

   finclipPath为finclip.exe所在位置，需转换成绝对路径
    ```
    const config = finclip.finclip_create_params();
    finclip.finclip_params_set(config, "appstore", APP_STORE);
    finclip.finclip_params_set(config, "appkey", APP_KEY);
    finclip.finclip_params_set(config, "secret", APP_SECRET);
    finclip.finclip_params_set(config, "domain", DOMAIN);
    finclip.finclip_params_set(config, "exe_path", finclipPath);
    finclip.finclip_init_with_config(APP_STORE, config);
    finclip.finclip_start_applet(APP_STORE, APPID);
    ```

4. 关闭finclip窗口

    ```
    finclip.finclip_close_applet(appid);
    ```

5. 设置finclip窗口的位置和大小

    ```
    finclip.finclip_set_position(appid, x, y, width, height);
    ```
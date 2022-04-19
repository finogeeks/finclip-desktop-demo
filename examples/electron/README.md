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

2. 设置启动参数

    ```
    finclip.setDomain('xxx');
    finclip.setAppkey('xxx');
    finclip.setAppid('xxx');
    finclip.setSecret('xxx');
    ```

3. 打开finclip窗口

   finclipPath为finclip.exe所在位置，需转换成绝对路径
    ```
    finclip.start({
        handle: 0,
        finclipPath,
    });
    ```

4. 关闭finclip窗口

    ```
    finclip.close();
    ```

5. 设置finclip窗口的位置和大小

    ```
    finclip.setPosition({ width: 800, height: 800, left: 0, top: 0 });
    ```

## 修改finclip依赖包

如果默认的finclip包无法满足需求，可以在此项目的`src/npm`下修改并编译，需要先配置C++环境
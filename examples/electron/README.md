# finclip-electron-demo

## 📦 安装第三方依赖

[下载finclip二进制包](https://github.com/finogeeks/finclip-win32-demo/releases)到`vendor`的对应目录下，如`vendor/win/x64`

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

2. 打开finclip窗口

   finclipPath为finclip.exe所在位置，需转换成绝对路径
    ```
    finclip.start({
        handle: 0,
        finclipPath,
    });
    ```

3. 关闭finclip窗口

    ```
    finclip.close();
    ```

4. 设置finclip窗口的位置和大小

    ```
    finclip.setPosition({ width: 800, height: 800, left: 0, top: 0 });
    ```

## 修改finclip依赖包

如果默认的finclip包无法满足需求，可以在此项目的`src/npm`下修改并编译，需要先配置C++环境
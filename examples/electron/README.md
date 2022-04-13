# finclip-electron-demo

## 快速开始

```
npm i
npm run start
```

## 调用finclip api

1. 引入finclip包

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
# electron 教程

### 环境准备

1. 需要预先有如下命令的显示
```shell
node -v
npm  -v
npm install -g cnpm --registry=https://registry.npm.taobao.org
```

2. 因为 Electron 将 Node.js 嵌入到其二进制文件中，你应用运行时的 Node.js 版本与你系统中运行的 Node.js 版本无关。
3. 创建第一个应用程序
    - 初始化一个 npm 项目环境,必须的字段是: `entry point: main.js`、`author`、`description`
        `mkdir test1 && cd test1 && npm init`
    - 安装 `electron` 到开发依赖中
        `cnpm install --save-dev electron`
    - 在 `package.json` 配置文件的 `scripts` 字段下增加如下命令, 方便执行 electron
        ```
        {
            "scripts": {
                "start": "electron ."
            }
        }
        ```


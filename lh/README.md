# 使用说明
1. 安装 python 3.8.6，下载并执行：https://www.python.org/ftp/python/3.8.6/python-3.8.6-amd64.exe
2. 安装 poetry，打开power shell 执行：(Invoke-WebRequest -Uri https://install.python-poetry.org -UseBasicParsing).Content | py - --version 1.8.5
3. 安装项目依赖: 在项目中打开 powershell 执行： poetry install
4. 安装 mt5setup.exe 并启动，然后在软件界面找到 "算法交易"并点击，使其变为绿色(按钮位置大概在 "窗口" 选项下边)
5. 运行此项目，执行：poetry run .\main.py

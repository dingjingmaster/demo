## 提交时候用GPG签名验证

1. 要在计算机上的任何本地存储库中默认签署所有提交，请运行
```shell
git config --global commit.gpgsign true
```

2. 当在本地分支中提交更改时，在 `git commit` 命令中添加 `-S` 标志:
```shell
git commit -S -m "<提交说明>"
```

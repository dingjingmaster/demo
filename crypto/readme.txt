使用例子
创建盒子，参数 1输出盒子文件 2密码  3要拷贝的盒子的初始目录
sudo ./boxcreate test1.vol 12345678 /home/wangtao/work/test

打开盒子  参数1 盒子文件  密码  挂载点
sudo ./boxopen test1.vol 12345678 /mnt


关闭盒子 参数是打开盒子返回的devmap，或查看 /dev/mapper下的微模块
sudo ./boxclose 25d7c1e2-b174-4154-9c48-71e77fd36a97



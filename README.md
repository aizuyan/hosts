#hosts使用指南

## 简介
这个工具实现了在命令行切换hosts配置的功能，目前是第一个版本，实现了基本的功能

## 安装
```shell
#也可以自己去下载
git clone https://github.com/aizuyan/hosts.git
cd hosts
#设置安装目录
./configure --prefix=/usr/local/hosts
make
#安装时候要是用root用户
make install
```

## 使用
```shell
#展示当前的hosts
hosts
```
![image](https://github.com/aizuyan/images/blob/master/aizuyan-hosts/hosts.png =80%)
```shell
#创建一个新的hosts文件
hosts make
```
![image](https://github.com/aizuyan/images/blob/master/aizuyan-hosts/hosts-make.png)

```shell
#切换hosts配置备份文件
hosts change tv107
```
![image](https://github.com/aizuyan/images/blob/master/aizuyan-hosts/hosts-change.png)

```shell
#添加hosts配置到当前正在使用的hosts备份中
hosts add 127.0.0.1 test.com localhost
```
![image](https://github.com/aizuyan/images/blob/master/aizuyan-hosts/hosts-add.png)

```shell
#删除指定的hosts配置，可以删除ip或者ip+web
hosts del 127.0.0.1 localhost
```
![image](https://github.com/aizuyan/images/blob/master/aizuyan-hosts/hosts-del.png)
```shell
hosts del 127.0.0.1
```
![image](https://github.com/aizuyan/images/blob/master/aizuyan-hosts/hosts-del-ip.png)

```shell
#列出所有的hosts配置,绿色字体表示正在使用的配置项
hosts names
```
![image](https://github.com/aizuyan/images/blob/master/aizuyan-hosts/hosts-names.png)

```shell
#列出所有的hosts详细信息
hosts all
```
![image](https://github.com/aizuyan/images/blob/master/aizuyan-hosts/hosts-all.png)

```shell
#删除一个hosts配置信息
hosts remove tv_107
```
![image](https://github.com/aizuyan/images/blob/master/aizuyan-hosts/hosts-remove.png)

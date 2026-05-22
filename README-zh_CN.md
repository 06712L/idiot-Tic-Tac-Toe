<div align="center">
<br />
<img src=./art/logo/iTTT-logo-1.png width=200>
<h1>idiot-Tic-Tac-Toe</h1>
<h3>i-TTT</h3>

一个很白痴的井字棋游戏
</div>

[[简体中文]](https://github.com/06712L/idiot-Tic-Tac-Toe/blob/main/README-zh_CN.md) [[繁體中文]](https://github.com/06712L/idiot-Tic-Tac-Toe/blob/main/README-zh_HK.md) [[English]](https://github.com/06712L/idiot-Tic-Tac-Toe/blob/main/README.md) [[what]](https://github.com/06712L/idiot-Tic-Tac-Toe/blob/main/README-what.md)

## 介绍

这是一个井字棋项目毫无意义,它没有GUI,但它是全英文的所以建议看得懂基础英文的再下载

## 已实现功能

- [x] 井字棋核心
    - [x] 双人模式
    - [x] AI vs 人类模式
    - [ ] 联网对战模式 *(没服务器我也不想做)*
- [x] 菜单
    - [x] 模式选择
    - [x] 设置 *(毫无作用)*
    - [x] 关于
    - [x] 如何玩教学
- [x] 发行版

## 特色功能

- 能双人对战
- 能和AI对战
- CLI界面
- 某些事正在发生...
- egg egg egg egg egg egg egg egg

## 如何玩？

<br />
<img src="./art/readme-how/how-to-play.jpg" width="250">
<br />

## 如何下载？

1. 前往[下载页面](https://github.com/06712L/idiot-Tic-Tac-Toe/releases/tag/V0.2.1)
2. 下载对应系统的文件

## 支持平台

- Linux *(无副档名)*
- windows *(`.exe`)*

## 如何编译？

### Makefile

- `make linux` / `make`: 编译linux可执行文件
- `make win`: 编译windows可执行文件
- `make cleanlinux`: 清理所有linux编译产物
- `make cleanwin`: 清理所有windows编译产物
- `make clean`: 清理所有编译产物
- `make *** DEBUG=1`: 编译/清理 调试版 可执行文件/编译产物
- `make *** STATIC=1`: 打包静态库

### CMake

NULL
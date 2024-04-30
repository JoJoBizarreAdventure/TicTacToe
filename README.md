# Tic-Tac-Toe

## 简介

简单的游戏Demo（cpp练手）

使用blend2d来生成一个texture，opengl显示

应用lua capi来读取配置文件（lua c api练手）

## 游戏操作

### 鼠标

点击选中格子 => 连线获胜

### 键盘

R： 局内重开

C： 清除所有分数

N：开启下一局游戏（只在一局游戏结束时启用）

## ui结构

- window
    - sidebar：侧边栏，显示分数和操作提示
    - chessboard：棋盘背景和网格线
    - grids：选中后显示O或X
    - end anim：连成线后，显示一段终止动画

## ai算法

### 简单ai

随机选择一个空格子

### MinMax ai

选择一个当前情况下尽可能不输的格子。

在有类对象初始化时，进行预计算

## Lua
### 配置项
- width,height : 窗口宽高
- fps ： 预期fps（目前上限大概在120左右）
- p1AI,p2AI : 玩家应用鼠标操作或ai操作
  - 第一轮时，p1先手，后续输家先手
- simpleAI : 应用简单ai或minmax ai

## 发现问题

blend2d和raylib颜色通道顺序不同（bgra => rgba） (已解决，替换raylib为opengl，shader中调换顺序)

wsl下无法禁止窗口Resize（2024.1）

目前所有逻辑全都集中在单一线程，对简单游戏来说，帧率足够用
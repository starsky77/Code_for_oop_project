# Basic framework of  a RPG game

The project of oop class;

一款RPG游戏的框架，基于SDL2图形库。

完整的报告里面有很多和程序无关的内容，所以就不上传了，这里简单说明一下程序结构。



## 基本框架

作为一个沙盒游戏，首先要实现的是一张大的地图，其次是实现静态物体的显示和动态人物的移动。因

此我们把主要的内容分为了Text、Entity、Utils三个模块：



**Text**模块实现了文本、静态图/边框的读入显示、时间系统与按钮功能；

**Entity**模块实现了静态道具、动态物体/角色显示、物体碰撞检测和地图显示；

**Utils**模块构建了两个类LTexture和LTimer用于处理游戏图片渲染和一个游戏事件的计时器。



## 数据结构设计

###  Text模块

class LButton :按钮类。实现按钮功能

class Frame :框架类，静态渲染图/对话边框的读入显示

class Text :文本类，文本的读入和显示

class timer :时间类，用于显示游戏时间多少

### Entity模块

class Entity :基础类，图片的渲染，用于构造道具和图片

class Tile :地图类，继承Entity，实现游戏地图

class StaticObj :静态道具类，实现静态道具或物体的实现

class MoveObj :基础动态类，实现动态图片的渲染

class Player :人物类，继承MoveObj，实现控制人物的移动和显示

### Utils模块

class LTexture :渲染图片打包类，里面包含对图片的各种处理

class LTimer :基于程序应用



## Reference

http://lazyfoo.net/tutorials/SDL/index.php

一个以游戏为例介绍SDL图形库使用的教程，除了对图形库的介绍以外，对游戏编程也有很多帮助。



## Additional Notice

程序存在一定的内存泄露问题（万恶C++），虽然经过再三修改还是存在一定问题。运行程序时占用的内存大致以每1M/5s的速度增加。可能是由于图形库的使用上还有一定问题，没有及时析构不使用的对象。
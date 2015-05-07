## 关于 Lua ##
Lua 是一门强大、快速、轻量的**嵌入式脚本语言。**它由巴西里约热内卢 Pontifical Catholic 大学的 PUC-Rio 团队 开发。 Lua 是一个自由软件， 广泛应用于世界上无数产品和项目。Lua 的[官方网站](http://www.lua.org)上提供了关于 Lua 的完整信息， 包括[综合概要](http://www.lua.org/about.html)和最新的[文档](http://www.lua.org/docs.html)。

**Lua 是一门扩展式程序设计语言**，被设计成支持通用过程式编程，并有相关数据描述设施。 **同时对面向对象编程、函数式编程和数据驱动式编程也提供了良好的支持。** 它作为一个强大、轻量的嵌入式脚本语言，可供任何需要的程序使用。 Lua 由 clean C（标准 C 和 C++ 间共通的子集） 实现成一个库。

**作为一门扩展式语言，Lua 没有 "main" 程序的概念**： 它只能 嵌入 一个宿主程序中工作， 该宿主程序被称为 被嵌入程序 或者简称 宿主 。 **宿主程序可以调用函数执行一小段 Lua 代码，可以读写 Lua 变量，可以注册 C 函数让 Lua 代码调用。** 依靠 C 函数，Lua 可以共享相同的语法框架来定制编程语言，从而适用不同的领域。 Lua 的官方发布版包含一个叫做 lua 的宿主程序示例， 它是一个利用 Lua 库实现的完整独立的 Lua 解释器，可用于交互式应用或批处理。

Lua 是一个自由软件，其使用许可证决定了它的使用过程无需任何担保。 本手册所描述的实现可以在 Lua 的[官方网站](www.lua.org) 找到。

## 1.1 安装lua ##

Lua 以[源代码](http://www.lua.org/ftp/)的形式发布，使用之前，你需要构建它。 构建 Lua 非常简单，因为 Lua 是用纯粹的 ANSI C 实现的，在所有具备 ANSI C 编译器的平台都可以直接编译。 同时，Lua 也可以直接以 C++ 形式编译。 下面介绍了类 Unix 平台上的构建流程。

**1.1.1 构建 Lua**

在大多数类 Unix 平台上，输入 "make" 加上合适的平台名即可。步骤如下：

1.获取lua源码

2.运行 "make" 并查看你的平台是否列在其中。 当前支持的平台有： 

- aix bsd c89 freebsd generic linux macosx mingw posix solaris

  如果你的平台在其中，运行 "make xxx" 即可，xxx 代表你的平台名。如果你的平台不在其中，先尝试最相近的平台，再按 posix generic c89 顺序依次尝试。

3.编译过程很短，最终在 src 目录下生成三个文件： lua (解释器), luac (编译器)和 liblua.a (静态库) 。

4.构建完成后，可以运行 "make test" 来检查是否成功。 它会运行解释器并打印版本号。

如果你是 Linux 系统并出现了编译错误，请确认你是否安装了 readline （也可能叫 libreadline-dev 或者 readline-devel）开发包。 之后，如果还有链接错误，尝试 "make linux MYLIBS=-ltermcap" 。

**1.1.2 安装 Lua**

一旦你构建完毕，可能希望把 Lua 安装到系统默认位置， 那么执行 "make install" 即可。 系统默认位置以及如何安装都定义在 Makefile 中。 这个过程可能需要有相关的权限。

运行 "make xxx install" 可以构建和安装一步到位，xxx 指你的平台名。

**1.1.3 ubuntu 安装 Lua5.3.0**


	1.wget http://www.lua.org/ftp/lua-5.3.0.tar.gz	
	2.tar -xvzf lua-5.3.0.tar.gz	
	3.cd lua-5.3.0/	
	4.sudo apt-get install libreadline6 libreadline6-dev	
	5.make linux	
	6.sudo make install 
	7.lua -v
	8.lua
	9.print("hello lua")



本教程内容主要来自：[《Pro Git》](http://git-scm.com/book/zh/v1)
## 1.关于版本控制 ##

**1.1 本地版本控制系统**

许多人习惯用复制整个项目目录的方式来保存不同的版本，或许还会改名加上备份时间以示区别。这么做唯一的好处就是简单。不过坏处也不少：有时候会混淆所在的工作目录，一旦弄错文件丢了数据就没法撤销恢复。

为了解决这个问题，人们很久以前就开发了许多种本地版本控制系统，大多都是采用某种简单的数据库来记录文件的历次更新差异（见图 1-1）。
 
![](https://raw.githubusercontent.com/peimin/me/master/07git-gogs入门/1.1.png)

**1.2 集中化的版本控制系统**

接下来人们又遇到一个问题，如何让在不同系统上的开发者协同工作？于是，集中化的版本控制系统（ Centralized Version Control Systems，简称 CVCS ）应运而生。这类系统，诸如 CVS，Subversion 以及 Perforce 等，都有一个单一的集中管理的服务器，保存所有文件的修订版本，而协同工作的人们都通过客户端连到这台服务器，取出最新的文件或者提交更新。多年以来，这已成为版本控制系统的标准做法（见图 1-2）。

![](https://raw.githubusercontent.com/peimin/me/master/07git-gogs入门/1.2.png)

事分两面，有好有坏。这么做最显而易见的缺点是中央服务器的单点故障。如果宕机一小时，那么在这一小时内，谁都无法提交更新，也就无法协同工作。要是中央服务器的磁盘发生故障，碰巧没做备份，或者备份不够及时，就会有丢失数据的风险。最坏的情况是彻底丢失整个项目的所有历史更改记录，而被客户端偶然提取出来的保存在本地的某些快照数据就成了恢复数据的希望。但这样的话依然是个问题，你不能保证所有的数据都已经有人事先完整提取出来过。本地版本控制系统也存在类似问题，只要整个项目的历史记录被保存在单一位置，就有丢失所有历史更新记录的风险。

**1.3 分布式版本控制系统**

于是分布式版本控制系统（ Distributed Version Control System，简称 DVCS ）面世了。在这类系统中，像 Git，Mercurial，Bazaar 以及 Darcs 等，客户端并不只提取最新版本的文件快照，而是把代码仓库完整地镜像下来。这么一来，任何一处协同工作用的服务器发生故障，事后都可以用任何一个镜像出来的本地仓库恢复。因为每一次的提取操作，实际上都是一次对代码仓库的完整备份（见图 1-3）。

![](https://raw.githubusercontent.com/peimin/me/master/07git-gogs入门/1.3.png)

更进一步，许多这类系统都可以指定和若干不同的远端代码仓库进行交互。籍此，你就可以在同一个项目中，分别和不同工作小组的人相互协作。你可以根据需要设定不同的协作流程，比如层次模型式的工作流，而这在以前的集中式系统中是无法实现的。

## 2.Git 简介与安装##

**2.1 Git 简介**

Linux 内核开源项目有着为数众广的参与者。绝大多数的 Linux 内核维护工作都花在了提交补丁和保存归档的繁琐事务上（1991－2002年间）。到 2002 年，整个项目组开始启用分布式版本控制系统 BitKeeper 来管理和维护代码。

到了 2005 年，开发 BitKeeper 的商业公司同 Linux 内核开源社区的合作关系结束，他们收回了免费使用 BitKeeper 的权力。这就迫使 Linux 开源社区（特别是 Linux 的缔造者 Linus Torvalds ）不得不吸取教训，只有开发一套属于自己的版本控制系统才不至于重蹈覆辙。他们对新的系统制订了若干目标：

- 速度
- 简单的设计
- 对非线性开发模式的强力支持（允许上千个并行开发的分支）
- 完全分布式
- 有能力高效管理类似 Linux 内核一样的超大规模项目（速度和数据量）

自诞生于 2005 年以来，Git 日臻成熟完善，在高度易用的同时，仍然保留着初期设定的目标。它的速度飞快，极其适合管理大项目，它还有着令人难以置信的非线性分支管理系统（见第三章），可以应付各种复杂的项目开发需求。

**2.2 Git 基础**

**2.2.1 直接记录快照，而非差异比较**

Git 并不保存这些前后变化的差异数据。实际上，Git 更像是把变化的文件作快照后，记录在一个微型的文件系统中。每次提交更新时，它会纵览一遍所有文件的指纹信息并对文件作一快照，然后保存一个指向这次快照的索引。为提高性能，若文件没有变化，Git 不会再次保存，而只对上次保存的快照作一链接。Git 的工作方式就像图 1-5 所示。

![](https://raw.githubusercontent.com/peimin/me/master/07git-gogs入门/1.5.png)

**2.2.2 近乎所有操作都是本地执行**

在 Git 中的绝大多数操作都只需要访问本地文件和资源，不用连网。但如果用 CVCS 的话，差不多所有操作都需要连接网络。因为 Git 在本地磁盘上就保存着所有当前项目的历史更新，所以处理起来速度飞快。

举个例子，如果要浏览项目的历史更新摘要，Git 不用跑到外面的服务器上去取数据回来，而直接从本地数据库读取后展示给你看。所以任何时候你都可以马上翻阅，无需等待。如果想要看当前版本的文件和一个月前的版本之间有何差异，Git 会取出一个月前的快照和当前文件作一次差异运算，而不用请求远程服务器来做这件事，或是把老版本的文件拉到本地来作比较。

用 CVCS 的话，没有网络或者断开 VPN 你就无法做任何事情。但用 Git 的话，就算你在飞机或者火车上，都可以非常愉快地频繁提交更新，等到了有网络的时候再上传到远程仓库。同样，在回家的路上，不用连接 VPN 你也可以继续工作。

**2.2.3 时刻保持数据完整性**

在保存到 Git 之前，所有数据都要进行内容的校验和（checksum）计算，并将此结果作为数据的唯一标识和索引。换句话说，不可能在你修改了文件或目录之后，Git 一无所知。这项特性作为 Git 的设计哲学，建在整体架构的最底层。所以如果文件在传输时变得不完整，或者磁盘损坏导致文件数据缺失，Git 都能立即察觉。

Git 使用 SHA-1 算法计算数据的校验和，通过对文件的内容或目录的结构计算出一个 SHA-1 哈希值，作为指纹字符串。该字串由 40 个十六进制字符（0-9 及 a-f）组成，看起来就像是：

	24b9da6552252987aa493b52f8696cd6d3b00373

Git 的工作完全依赖于这类指纹字串，所以你会经常看到这样的哈希值。实际上，所有保存在 Git 数据库中的东西都是用此哈希值来作索引的，而不是靠文件名。

**2.2.4 多数操作仅添加数据**

常用的 Git 操作大多仅仅是把数据添加到数据库。因为任何一种不可逆的操作，比如删除数据，都会使回退或重现历史版本变得困难重重。在别的 VCS 中，若还未提交更新，就有可能丢失或者混淆一些修改的内容，但在 Git 里，一旦提交快照之后就完全不用担心丢失数据，特别是养成定期推送到其他仓库的习惯的话。

这种高可靠性令我们的开发工作安心不少，尽管去做各种试验性的尝试好了，再怎样也不会弄丢数据。

**2.2.5 文件的三种状态**

对于任何一个文件，在 Git 内都只有三种状态：已提交（committed），已修改（modified）和已暂存（staged）。已提交表示该文件已经被安全地保存在本地数据库中了；已修改表示修改了某个文件，但还没有提交保存；已暂存表示把已修改的文件放在下次提交时要保存的清单中。

由此我们看到 Git 管理项目时，文件流转的三个工作区域：Git 的工作目录，暂存区域，以及本地仓库。

![](https://raw.githubusercontent.com/peimin/me/master/07git-gogs入门/1.6.png)

图 1-6. 工作目录，暂存区域，以及本地仓库

基本的 Git 工作流程如下：

1. 在工作目录中修改某些文件。
1. 对修改后的文件进行快照，然后保存到暂存区域。
1. 提交更新，将保存在暂存区域的文件快照永久转储到 Git 目录中。

所以，我们可以从文件所处的位置来判断状态：如果是 Git 目录中保存着的特定版本文件，就属于已提交状态；如果作了修改并已放入暂存区域，就属于已暂存状态；如果自上次取出后，作了修改但还没有放到暂存区域，就是已修改状态。到第二章的时候，我们会进一步了解其中细节，并学会如何根据文件状态实施后续操作，以及怎样跳过暂存直接提交。

**2.3 安装 Git**

在 ubuntu 上安装 

	$ apt-get install git

在 Windows 上安装

	http://msysgit.github.com/

另外 windows 下也有个tortoisegit工具方便我们使用git。

## 3.git 服务器安装 ##

目前团队中比较广泛使用的是一些带有web功能的git服务器，它们除了代码审核提交之外还有issue等功能，更加方便的对项目进行管理和维护。其中Github最为出名，目前是全球最大代码托管网站。与Github类似的开源项目有gitlib gogs等。

**ubuntu 服务器(x64)上 gogs 的安装**

[gogs官网](http://gogs.io)给出了二进制和源码安装的详细过程，推荐使用二进制包来安装。

	$ sudo adduser git
	$ sudo apt-get install git
	$ sudo apt-get install mysql-server

在mysql中创建一个gogs用的数据库

	$ mysql -u root -p
	$ mysql> CREATE DATABASE gogs CHARACTER SET utf8 COLLATE utf8_bin;
	$ mysql> GRANT ALL PRIVILEGES ON gogs.* TO ‘root’@‘localhost’ IDENTIFIED BY ‘password’;
	$ mysql> FLUSH PRIVILEGES;
	$ mysql> QUIT

获取gogs二进制安装包并且启动gogs服务器

	1.从[gogs Github仓库](https://github.com/gogits/gogs/releases)上获取二进制安装包。
	2.将或者到的gogs二进制安装包上传到服务器上。
	3.$ unzip linux_amd64.zip
	4.$ ./gogs web
	5.在浏览器中输入服务器 ip:port(默认3000) 访问并且对gogs服务器进行初始化设置。

## 4.git 使用 ##

**4.1 取得项目的 Git 仓库**

有两种取得 Git 项目仓库的方法。第一种是在现存的目录下，通过导入所有文件来创建新的 Git 仓库。第二种是从已有的 Git 仓库克隆出一个新的镜像仓库来。

**4.1.1 在工作目录中初始化新仓库**

要对现有的某个项目开始用 Git 管理，只需到此项目所在的目录，执行：

	git init

初始化后，在当前目录下会出现一个名为 .git 的目录，所有 Git 需要的数据和资源都存放在这个目录中。不过目前，仅仅是按照既有的结构框架初始化好了里边所有的文件和目录，但我们还没有开始跟踪管理项目中的任何一个文件。（在后面章节我们会详细说明刚才创建的 .git 目录中究竟有哪些文件，以及都起些什么作用。）

如果当前目录下有几个文件想要纳入版本控制，需要先用 git add 命令告诉 Git 开始对这些文件进行跟踪，然后提交：

	git add *.c
	git add README
	git commit -m 'initial project version'

**4.1.2 从现有仓库克隆**

先把该项目的 Git 仓库复制一份出来，这就需要用到 git clone 命令。如果你熟悉其他的 VCS 比如 Subversion，你可能已经注意到这里使用的是 clone 而不是 checkout。

	git clone git://github.com/schacon/grit.git

Git 支持许多数据传输协议。之前的例子使用的是 git:// 协议，不过你也可以用 http(s):// 或者 user@server:/path.git 表示的 SSH 传输协议。

**4.2 忽略某些文件**

一般我们总会有些文件无需纳入 Git 的管理，也不希望它们总出现在未跟踪文件列表。通常都是些自动生成的文件，比如日志文件，或者编译过程中创建的临时文件等。我们可以创建一个名为 .gitignore 的文件，列出要忽略的文件模式。来看一个实际的例子：

	$ cat .gitignore
	*.[oa]
	*~

第一行告诉 Git 忽略所有以 .o 或 .a 结尾的文件。一般这类对象文件和存档文件都是编译过程中出现的，我们用不着跟踪它们的版本。第二行告诉 Git 忽略所有以波浪符（~）结尾的文件，许多文本编辑软件（比如 Emacs）都用这样的文件名保存副本。此外，你可能还需要忽略 log，tmp 或者 pid 目录，以及自动生成的文档等等。要养成一开始就设置好 .gitignore 文件的习惯，以免将来误提交这类无用的文件。

文件 .gitignore 的格式规范如下：

- 所有空行或者以注释符号 ＃ 开头的行都会被 Git 忽略。
- 可以使用标准的 glob 模式匹配。
- 匹配模式最后跟反斜杠（/）说明要忽略的是目录。
- 要忽略指定模式以外的文件或目录，可以在模式前加上惊叹号（!）取反。 

## 5.使用 gogs 仓库 ##

**5.1 简单的使用**

fork一份到本地

	$ git clone http://git.yzhan.com/peimin/test.git

查看提交日志等

	$ git log
	$ git status

简单的提交文件到本地仓库：

	vi hello.lua
	git add hello.lua
	git commit -m "hello"
	git pull
	git push

**5.2 远程仓库的使用**

要查看当前配置有哪些远程仓库，可以用 git remote 命令，它会列出每个远程库的简短名字。在克隆完某个项目后，至少可以看到一个名为 origin 的远程库，Git 默认使用这个名字来标识你所克隆的原始仓库。

	$ git remote -v

	git remote add pbc  http://git.yzhan.com/peimin/test.git
	
	git fetch pbc

	git push origin master

	git remote show origin


**5.3 分支**

分支的新建与切换

	$ git branch iss53
	$ git checkout iss53
	$ vi index.html
	$ git add index.html
	$ git commit -a -m "add a new file"

分支的合并

在问题 #53 相关的工作完成之后，可以合并回 master 分支。只需回到 master 分支，运行 git merge 命令指定要合并进来的分支：

	git checkout master
	git merge iss53
	git branch -v

注意看 master 分支前的 * 字符：它表示当前所在的分支。

推送本地分支

将我们本地的iss53分支推送到远程 

	git push (远程仓库名) (分支名)

	git branch
	
	git remote
	
	git push origin iss53

	git push (默认为 origin/master master)

拉取远程分支信息：
	
	git fetch origin wpm-dev:wpm-dev 
  





























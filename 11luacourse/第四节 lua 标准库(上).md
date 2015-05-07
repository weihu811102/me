## lua 标准库 ##
标准库提供了一些有用的函数, 它们都是直接用 **C API 实现的**。 其中一些函数提供了原本语言就有的服务 （例如，type 与 getmetatable）； 另一些提供和“外部”打交道的服务（例如 I/O ）； 还有些本可以用 Lua 本身来实现，但在 C 中实现可以满足关键点上的性能需求 （例如 table.sort）。

所有的库都是直接用 C API 实现的，并以分离的 C 模块形式提供。 目前，Lua 有下列标准库：

- 基础库 
- 协程库 
- 包管理库 
- 字符串控制 
- 基础 UTF-8 支持 
- 表控制 
- 数学函数 (sin ，log 等)
- 输入输出 
- 操作系统库 
- 调试库  

除了基础库和包管理库， 其它库都把自己的函数放在一张全局表的域中， 或是以对象方法的形式提供。要使用这些库，**C 的宿主程序需要先调用一下 luaL_openlibs 这个函数**，这样就能打开所有的标准库。 或者宿主程序也可以用 luaL_requiref 分别打开这些库： 

- luaopen_base （基础库） 
- luaopen_package （包管理库） 
- luaopen_coroutine （协程库） 
- luaopen_string （字符串库） 
- luaopen_utf8 （UTF8 库） 
- luaopen_table （表处理库）
- luaopen_math （数学库）
- luaopen_io （I/O 库） 
- luaopen_os （操作系统库） 
- luaopen_debug （调试库） 

**这些函数都定义在 lualib.h 中。**

## 4.1基础函数 ##

基础库提供了 Lua 核心函数。 如果你不将这个库包含在你的程序中， 你就需要小心检查程序是否需要自己提供其中一些特性的实现。这里列出了一些常用的基础库函数：

**4.1.1 assert (v [, message])**

如果其参数 v 的值为假（nil 或 false）， 它就调用 error； 否则，返回所有的参数。 在错误情况时， message 指那个错误对象； 如果不提供这个参数，参数默认为 "assertion failed!" 。

案列：

	assert(5 == 67)

运行结果：

	lua: ll.lua:1: assertion failed!
	stack traceback:
	        [C]: in function 'assert'
	        ll.lua:1: in main chunk
	        [C]: in ?

**4.1.2 print (···)**

接收任意数量的参数，并将它们的值打印到 stdout。 它用 tostring 函数将每个参数都转换为字符串。 print 不用于做格式化输出。仅作为看一下某个值的快捷方式。 多用于调试。 完整的对输出的控制，请使用 string.format 以及 io.write。

**4.1.3 error (message [, level])**

中止上一次保护函数调用， 将错误对象 message 返回。 函数 error 永远不会返回。
当 message 是一个字符串时，通常 error 会把一些有关出错位置的信息附加在消息的前头。 level 参数指明了怎样获得出错位置。 对于 level 1 （默认值），出错位置指 error 函数调用的位置。 Level 2 将出错位置指向调用 error的函数的函数；以此类推。 传入 level 0 可以避免在消息前添加出错位置信息。

**4.1.4 getmetatable (object)**

如果 object 不包含元表，返回 nil 。 否则，如果在该对象的元表中有 "__metatable" 域时返回其关联值， 没有时返回该对象的元表。

**4.1.5 ipairs (t)**

返回三个值（迭代函数、表 t 以及 0 ）， 如此，以下代码

     for i,v in ipairs(t) do body end
将迭代键值对（1,t[1]) ，(2,t[2])， ... ，直到第一个空值。

示例代码：

	local nums = {11, 22, 33}
	for i,v in ipairs(nums) do 
		print(i..' -> '..v) 
	end   

**4.1.6 pcall (f [, arg1, ···])**

**传入参数，以 保护模式 调用函数 f 。 **这意味着 f 中的任何错误不会抛出； 取而代之的是，pcall 会将错误捕获到，并返回一个状态码。 第一个返回值是状态码（一个布尔量）， 当没有错误时，其为真。 此时，pcall 同样会在状态码后返回所有调用的结果。 在有错误时，pcall 返回 false 加错误消息。

**4.1.7 rawequal (v1, v2)**

在不触发任何元方法的情况下 检查 v1 是否和 v2 相等。 返回一个布尔量。

**4.1.8 rawlen (v)**

在不触发任何元方法的情况下 返回对象 v 的长度。 v 可以是表或字符串。 它返回一个整数。

**4.1.9 setmetatable (table, metatable)**

**给指定表设置元表。**（你不能在 Lua 中改变其它类型值的元表，那些只能在 C 里做。） 如果 metatable 是 nil， 将指定表的元表移除。 如果原来那张元表有 "__metatable" 域，抛出一个错误。

这个函数返回 table。

**4.1.10 tonumber (e [, base])**

如果调用的时候没有 base， tonumber 尝试把参数转换为一个数字。 如果参数已经是一个数字，或是一个可以转换为数字的字符串， tonumber 就返回这个数字； 否则返回 nil。

字符串的转换结果可能是整数也可能是浮点数， 这取决于 Lua 的转换文法。 （字符串可以有前置和后置的空格，可以带符号。）

**当传入 base 调用它时， e 必须是一个以该进制表示的整数字符串。** 进制可以是 2 到 36 （包含 2 和 36）之间的任何整数。 大于 10 进制时，字母 'A' （大小写均可）表示 10 ， 'B' 表示 11，依次到 'Z' 表示 35 。 如果字符串 e 不是该进制下的合法数字， 函数返回 nil。

**4.1.11 tostring (v)**

可以接收任何类型，它将其转换为人可阅读的字符串形式。 浮点数总被转换为浮点数的表现形式（小数点形式或是指数形式）。 （如果想完全控制数字如何被转换，可以使用 string.format。）
如果 v 有 "__tostring" 域的元表， tostring 会以 v 为参数调用它。 并用它的结果作为返回值。

**4.1.12 type (v)**

将参数的类型编码为一个字符串返回。 函数可能的返回值有 "nil" （一个字符串，而不是 nil 值）， "number"， "string"， "boolean"， "table"， "function"， "thread"， "userdata"。

示例代码：
	
	local num = tonumber("12.5")
	local str = tostring(13.2)
	
	print(type(num)) -- number
	print(type(str)) -- string
	print(tonumber("FF", 16)) -- 255

## 4.2 协程管理 ##

关于协程的操作作为基础库的一个子库， 被放在一个独立表 coroutine 中。 

**4.2.1 coroutine.create (f)**

创建一个主体函数为 f 的新协程。 f 必须是一个 Lua 的函数。 返回这个新协程，它是一个类型为 "thread" 的对象。

**4.2.2 coroutine.isyieldable ()**

如果正在运行的协程可以让出，则返回真。

不在主线程中或不在一个无法让出的 C 函数中时，当前协程是可让出的。

**4.2.3 coroutine.resume (co [, val1, ···])**

开始或继续协程 co 的运行。 当你第一次延续一个协程，它会从主体函数处开始运行。 val1, ... 这些值会以参数形式传入主体函数。 如果该协程被让出，resume 会重新启动它； val1, ... 这些参数会作为让出点的返回值。

如果协程运行起来没有错误， resume 返回 true 加上传给 yield 的所有值 （当协程让出）， 或是主体函数的所有返回值（当协程中止）。 如果有任何错误发生， resume 返回 false 加错误消息。

**4.2.4 coroutine.running ()**

返回当前正在运行的协程加一个布尔量。 如果当前运行的协程是主线程，其为真。

**4.2.5 coroutine.status (co)**

以字符串形式返回协程 co 的状态： 

- 当协程正在运行（它就是调用 status 的那个） ，返回 "running"； 
- 如果协程调用 yield 挂起或是还没有开始运行，返回 "suspended"；
- 如果协程是活动的，都并不在运行（即它正在延续其它协程），返回 "normal"； 
- 如果协程运行完主体函数或因错误停止，返回 "dead"。

**4.2.6 coroutine.wrap (f)**

创建一个主体函数为 f 的新协程。 f 必须是一个 Lua 的函数。 返回一个函数， 每次调用该函数都会延续该协程。 传给这个函数的参数都会作为 resume 的额外参数。 和 resume 返回相同的值， 只是没有第一个布尔量。 如果发生任何错误，抛出这个错误。

**4.2.7 coroutine.yield (···)**

**挂起正在调用的协程的执行。 传递给 yield 的参数都会转为 resume 的额外返回值。**

**示例1-简单的案例：**

	local co = coroutine.create(function () 
		print("Hello World") 
	end)
	
	print(coroutine.status(co))   -- suspended
	
	coroutine.resume(co)          -- Hello World


调用了resume函数，将协同程序co由suspended改为running状态，当打印了Hello World之后，协同程序co就处于死亡状态。

**示例2-yield与resume：**

	local co = coroutine.create(function ()
	     for i = 1, 10 do
	          print("co", i)
	          coroutine.yield()     -- 挂起
	     end
	end)
	
	
	print(coroutine.status(co))     -- suspended
	
	coroutine.resume(co)            -- 打印co 1
	
	print(coroutine.status(co))     -- suspended
	
	coroutine.resume(co)            -- 打印co 2
	
	print(coroutine.status(co))     -- suspended
	
	coroutine.resume(co)			-- 打印co 3
	coroutine.resume(co)			-- 打印co 4
	coroutine.resume(co)			-- 打印co 5
	coroutine.resume(co)			-- 打印co 6
	coroutine.resume(co)			-- 打印co 7
	coroutine.resume(co)			-- 打印co 8
	coroutine.resume(co)			-- 打印co 9
	coroutine.resume(co)			-- 打印co 10
	coroutine.resume(co)			-- 什么都不打印
	print(coroutine.status(co))     -- dead

## 4.3 模块 ##
包管理库提供了从 Lua 中加载模块的基础库。 只有一个导出函数直接放在全局环境中： require。 所有其它的部分都导出在表 package 中。

**4.3.1 require (modname)**

**加载一个模块。** 这个函数首先查找 package.loaded 表， 检测 modname 是否被加载过。 如果被加载过，require 返回 package.loaded[modname] 中保存的值。 否则，它试着为模块寻找 加载器 。

require 遵循 package.searchers 序列的指引来查找加载器。 如果改变这个序列，我们可以改变 require 如何查找一个模块。 下列说明基于 package.searchers 的默认配置。

首先 require 查找 package.preload[modname] 。 如果这里有一个值，这个值（必须是一个函数）就是那个加载器。 否则 require 使用 Lua 加载器去查找 package.path 的路径。 如果查找失败，接着使用 C 加载器去查找 package.cpath 的路径。 如果都失败了，再尝试 一体化 加载器 （参见 package.searchers）。

每次找到一个加载器，require 都用两个参数调用加载器： modname 和一个在获取加载器过程中得到的参数。 （如果通过查找文件得到的加载器，这个额外参数是文件名。） 如果加载器返回非空值， require 将这个值赋给 package.loaded[modname]。 如果加载器没能返回一个非空值用于赋给 package.loaded[modname]， require 会在那里设入 true 。 无论是什么情况，require 都会返回 package.loaded[modname] 的最终值。

如果在加载或运行模块时有错误， 或是无法为模块找到加载器， require 都会抛出错误。

**4.3.2 package.config**

一个描述有一些为包管理准备的编译期配置信息的串。 这个字符串由一系列行构成：

- 第一行是目录分割串。 对于 Windows 默认是 '\' ，对于其它系统是 '/' 。
- 第二行是用于路径中的分割符。默认值是 ';' 。
- 第三行是用于标记模板替换点的字符串。 默认是 '?' 。
- 第四行是在 Windows 中将被替换成执行程序所在目录的路径的字符串。 默认是 '!' 。
- 第五行是一个记号，该记号之后的所有文本将在构建 luaopen_ 函数名时被忽略掉。 默认是 '-'。

**4.3.3 package.cpath**

这个路径被 require 在 C 加载器中做搜索时用到。

Lua 用和初始化 Lua 路径 package.path 相同的方式初始化 C 路径 package.cpath 。 它会使用环境变量 LUA_CPATH_5_3 或 环境变量 LUA_CPATH 初始化。 要么就采用 luaconf.h 中定义的默认路径。

**4.3.4 package.path**

这个路径被 require 在 Lua 加载器中做搜索时用到。

在启动时，Lua 用环境变量 LUA_PATH_5_3 或环境变量 LUA_PATH 来初始化这个变量。 或采用 luaconf.h 中的默认路径。 环境变量中出现的所有 ";;" 都会被替换成默认路径。

**4.3.5 package.searchers**

用于 require 控制如何加载模块的表。

这张表内的每一项都是一个 查找器函数。 当查找一个模块时， require 按次序调用这些查找器， 并传入模块名（require 的参数）作为唯一的一个参数。 此函数可以返回另一个函数（模块的 加载器）加上另一个将传递给这个加载器的参数。 或是返回一个描述为何没有找到这个模块的字符串 （或是返回 nil 什么也不想说）。

Lua 用四个查找器函数初始化这张表。

- 第一个查找器就是简单的在 package.preload 表中查找加载器。

- 第二个查找器用于查找 Lua 库的加载库。 它使用储存在 package.path 中的路径来做查找工作。 查找过程和函数 package.searchpath 描述的一致。

- 第三个查找器用于查找 C 库的加载库。 它使用储存在 package.cpath 中的路径来做查找工作。 同样， 查找过程和函数 package.searchpath 描述的一致。 例如，如果 C 路径是这样一个字符串

     "./?.so;./?.dll;/usr/local/?/init.so"
查找器查找模块 foo 会依次尝试打开文件 ./foo.so，./foo.dll， 以及 /usr/local/foo/init.so。 一旦它找到一个 C 库， 查找器首先使用动态链接机制连接该库。 然后尝试在该库中找到可以用作加载器的 C 函数。 这个 C 函数的名字是 "luaopen_" 紧接模块名的字符串， 其中字符串中所有的下划线都会被替换成点。 此外，如果模块名中有横线， 横线后面的部分（包括横线）都被去掉。 例如，如果模块名为 a.b.c-v2.1， 函数名就是 luaopen_a_b_c。

- 第四个搜索器是　一体化加载器。 它从 C 路径中查找指定模块的根名字。 例如，当请求 a.b.c　时， 它将查找 a 这个 C 库。 如果找得到，它会在里面找子模块的加载函数。 在我们的例子中，就是找　luaopen_a_b_c。 利用这个机制，可以把若干 C 子模块打包进单个库。 每个子模块都可以有原本的加载函数名。

除了第一个（预加载）搜索器外，每个搜索器都会返回 它找到的模块的文件名。 这和 package.searchpath 的返回值一样。 第一个搜索器没有返回值。

**4.3.6 package.searchpath (name, path [, sep [, rep]])**

在指定 path 中搜索指定的 name 。

路径是一个包含有一系列以分号分割的 模板 构成的字符串。 对于每个模板，都会用 name 替换其中的每个问号（如果有的话）。 且将其中的 sep （默认是点）替换为 rep （默认是系统的目录分割符）。 然后尝试打开这个文件名。

例如，如果路径是字符串

     "./?.lua;./?.lc;/usr/local/?/init.lua"
搜索 foo.a　这个名字将 依次尝试打开文件 ./foo/a.lua　， ./foo/a.lc　，以及 /usr/local/foo/a/init.lua。

返回第一个可以用读模式打开（并马上关闭该文件）的文件的名字。 如果不存在这样的文件，返回 nil 加上错误消息。 （这条错误消息列出了所有尝试打开的文件名。）

**示例代码(复数案例)：**

complex.lua 

	complex = {}   
	
	function complex.new(r, i) 
		return {r = r, i = i} -- a table
	end
	
	function complex.add(c1, c2)
	    return complex.new(c1.r + c2.r, c1.i + c2.i)
	end
	
	function complex.sub(c1, c2)
	    return complex.new(c1.r - c2.r, c1.i - c2.i)
	end
	
	return complex  

test.lua

	local complex = require("complex")
	
	local c1 = complex.new(0, 1) -- a table
	local c2 = complex.new(1, 2)
	
	local ret = complex.add(c1, c2)
	
	print(ret.r, ret.i) -- 1 3


## 4.4 字符串处理 ##
这个库提供了字符串处理的通用函数。 例如字符串查找、子串、模式匹配等。 **当在 Lua 中对字符串做索引时，第一个字符从 1 开始计算（而不是 C 里的 0 ）。 索引可以是负数，它指从字符串末尾反向解析。 即，最后一个字符在 -1 位置处，等等。**

字符串库中的所有函数都在表 string 中。 它还将其设置为字符串元表的 __index 域。 因此，你可以以面向对象的形式使用字符串函数。 例如，string.byte(s,i) 可以写成 s:byte(i)。

字符串库假定采用单字节字符编码

**4.4.1 string.format (formatstring, ···)**

返回不定数量参数的格式化版本， 格式化串为第一个参数（必须是一个字符串）。 格式化字符串遵循 ISO C 函数 sprintf 的规则。 不同点在于选项 *, h, L, l, n, p 不支持， 另外还增加了一个选项 q。 q 选项将一个字符串格式化为两个双引号括起，对内部字符做恰当的转义处理的字符串。 该字符串可以安全的被 Lua 解释器读回来。 例如，调用

     string.format('%q', 'a string with "quotes" and \n new line')
会产生字符串：

     "a string with \"quotes\" and \
      new line"
选项 A and a （如果有的话）， E, e, f, G, and g 都期待一个对应的数字参数。 选项 c, d, i, o, u, X, and x 则期待一个整数。 选项 q 期待一个字符串； 选项 s 期待一个没有内嵌零的字符串。 如果选项 s 对应的参数不是字符串，它会用和 tostring 一致的规则转换成字符串。

**4.4.2 string.gmatch (s, pattern)**

返回一个迭代器函数。 每次调用这个函数都会继续以 pattern 对 s 做匹配，并返回所有捕获到的值。 如果 pattern 中没有指定捕获，则每次捕获整个 pattern。
下面这个例子会循环迭代字符串 s 中所有的单词， 并逐行打印：

     s = "hello world from Lua"
     for w in string.gmatch(s, "%a+") do
       print(w)
     end
下一个例子从指定的字符串中收集所有的键值对 key=value：

	local s = "from=world, to=Lua"
	
	for k, v in string.gmatch(s, "(%w+)=(%w+)") do
		print (k..' -> '..v)	
	end

**4.4.3 string.gsub (s, pattern, repl [, n])**

**将字符串 s 中，所有的（或是在 n 给出时的前 n 个） pattern 都替换成 repl ，并返回其副本。 repl 可以是字符串、表、或函数。 gsub 还会在第二个返回值返回一共发生了多少次匹配。** gsub 这个名字来源于 Global SUBstitution 。

-** 如果 repl 是一个字符串，**那么把这个字符串作为替换品。 字符 % 是一个转义符： repl 中的所有形式为 %d 的串表示 第 d 个捕获到的子串，d 可以是 1 到 9 。 串 %0 表示整个匹配。 串 %% 表示单个 %。
- **如果 repl 是张表，**每次匹配时都会用第一个捕获物作为键去查这张表。
- 如果 repl 是个函数，则在每次匹配发生时都会调用这个函数。 所有捕获到的子串依次作为参数传入。
任何情况下，模板中没有设定捕获都看成是捕获整个模板。
- **如果表的查询结果或函数的返回结果是一个字符串或是个数字，** 都将其作为替换用串； 而在返回 false 或 nil　时不作替换 （即保留匹配前的原始串）。

**示例代码：**
	
	x = string.gsub("hello world", "(%w+)", "%1 %1")
	print(x)
	-- x="hello hello world world"
            
	x = string.gsub("hello world", "%w+", "%0 %0", 1)
	print(x)
	-- x="hello hello world"
	
	x = string.gsub("hello world from Lua", "(%w+)%s*(%w+)", "%2 %1")
	print(x)
	-- x="world hello Lua from"
	
	x = string.gsub("home = $HOME, user = $USER", "%$(%w+)", os.getenv)
	print(x)
	-- x="home = /home/roberto, user = roberto"
	
	x = string.gsub("4+5 = $return 4+5$", "%$(.-)%$", function (s)
	   return load(s)()
	 end)
	print(x)
	-- x="4+5 = 9"
	
	local t = {name="lua", version="5.3"}
	x = string.gsub("$name-$version.tar.gz", "%$(%w+)", t)
	print(x)
	-- x="lua-5.3.tar.gz"

**4.4.4 string.len (s)**

接收一个字符串，返回其长度。 空串 "" 的长度为 0 。 内嵌零也统计在内，因此 "a\000bc\000" 的长度为 5 。

**4.4.5 string.lower (s)**

接收一个字符串，将其中的大写字符都转为小写后返回其副本。 其它的字符串不会更改。 对大写字符的定义取决于当前的区域设置。

**4.4.6 string.reverse (s)**

返回字符串 s 的翻转串。

**4.4.7 string.sub (s, i [, j])**

**返回 s 的子串**， 该子串从 i 开始到 j 为止； i 和 j 都可以为负数。 如果不给出 j ，就当它是 -1 （和字符串长度相同）。 特别是， 调用 string.sub(s,1,j) 可以返回 s 的长度为 j 的前缀串， 而 string.sub(s, -i) 返回长度为 i 的后缀串。
如果在对负数索引转义后 i 小于 1 的话，就修正回 1 。 如果 j 比字符串的长度还大，就修正为字符串长度。 如果在修正之后，i 大于 j， 函数返回空串。

**4.4.8 string.upper (s)**

接收一个字符串，将其中的小写字符都转为大写后返回其副本。 其它的字符串不会更改。 对小写字符的定义取决于当前的区域设置。

**示例代码：**

	local str = "hello lua"

	print(string.len(str))        -- 9
	print(string.rep(str, 2))     -- hello luahello lua
	print(string.upper(str))      -- HELLO LUA
	print(string.sub(str, 2, 6))  -- ello
	print(string.sub(str, 2, -2)) -- ello lu
	print(string.format("%s %d", "hello lua", 32)) -- hello lua 32

在Lua中，字符串的第一个字符的索引是1，但是，索引也可以是负数，表示从字符串的尾部开始计数，索引-1代表字符串的最后一个字符。

## 4.5 匹配模式 ##

Lua 中的匹配模式直接用常规的字符串来描述。 它用于模式匹配函数 string.find, string.gmatch, string.gsub, string.match。 这一节表述了这些字符串的语法及含义（即它能匹配到什么）。

**4.5.1 字符类**

字符类用于表示一个字符集合。 下列组合可用于字符类：

- x: （这里 x 不能是 魔法字符 ^$()%.[]*+-? 中的一员） 表示字符 x 自身。
- .: （一个点）可表示任何字符。
- %a: 表示任何字母。
- %c: 表示任何控制字符。
- %d: 表示任何数字。
- %g: 表示任何除空白符外的可打印字符。
- %l: 表示所有小写字母。
- %p: 表示所有标点符号。
- %s: 表示所有空白字符。
- %u: 表示所有大写字母。
- %w: 表示所有字母及数字。
- %x: 表示所有 16 进制数字符号。
- %x: （这里的 x 是任意非字母或数字的字符） 表示字符 x。 这是对魔法字符转义的标准方法。 所有非字母或数字的字符 （包括所有标点，也包括非魔法字符） 都可以用前置一个 '%' 放在模式串中表示自身。
- [set]: 表示 set　中所有字符的联合。 可以以 '-' 连接，升序书写范围两端的字符来表示一个范围的字符集。 上面提到的 %x 形式也可以在 set 中使用 表示其中的一个元素。 其它出现在 set 中的字符则代表它们自己。 例如，[%w_] （或 [_%w]） 表示所有的字母数字加下划线）， [0-7] 表示 8 进制数字， [0-7%l%-]　表示 8 进制数字加小写字母与 '-' 字符。
交叉使用类和范围的行为未定义。 因此，像 [%a-z] 或 [a-%%] 这样的模式串没有意义。

- [^set]: 表示 set 的补集， 其中 set 如上面的解释。
所有单个字母表示的类别（%a，%c，等）， 若将其字母改为大写，均表示对应的补集。 例如，%S 表示所有非空格的字符。

如何定义字母、空格、或是其他字符组取决于当前的区域设置。 特别注意：[a-z]　未必等价于 %l 。

**4.5.2 模式条目：**

模式条目可以是：

- 单个字符类匹配该类别中任意单个字符；
- 单个字符类跟一个 '*'， 将匹配零或多个该类的字符。 这个条目总是匹配尽可能长的串；
- 单个字符类跟一个 '+'， 将匹配一或更多个该类的字符。 这个条目总是匹配尽可能长的串；
- 单个字符类跟一个 '-'， 将匹配零或更多个该类的字符。 和 '*' 不同， 这个条目总是匹配尽可能短的串；
- 单个字符类跟一个 '?'， 将匹配零或一个该类的字符。 只要有可能，它会匹配一个；
- %n， 这里的 n 可以从 1 到 9； 这个条目匹配一个等于 n 号捕获物（后面有描述）的子串。
- %bxy， 这里的 x 和 y 是两个明确的字符； 这个条目匹配以 x 开始 y 结束， 且其中 x 和 y 保持 平衡 的字符串。 意思是，如果从左到右读这个字符串，对每次读到一个 x 就 +1 ，读到一个 y 就 -1， 最终结束处的那个 y 是第一个记数到 0 的 y。 举个例子，条目 %b() 可以匹配到括号平衡的表达式。
- %f[set]， 指 边境模式； 这个条目会匹配到一个位于 set 内某个字符之前的一个空串， 且这个位置的前一个字符不属于 set 。 集合 set 的含义如前面所述。 匹配出的那个空串之开始和结束点的计算就看成该处有个字符 '\0' 一样。

**4.5.3 模式：**

模式 指一个模式条目的序列。 在模式最前面加上符号 '^' 将锚定从字符串的开始处做匹配。 在模式最后面加上符号 '$' 将使匹配过程锚定到字符串的结尾。 如果 '^' 和 '$' 出现在其它位置，它们均没有特殊含义，只表示自身。

**4.5.4 捕获：**

模式可以在内部用小括号括起一个子模式； 这些子模式被称为 捕获物。 当匹配成功时，由 捕获物 匹配到的字符串中的子串被保存起来用于未来的用途。 捕获物以它们左括号的次序来编号。 例如，对于模式 "(a*(.)%w(%s*))" ， 字符串中匹配到 "a*(.)%w(%s*)" 的部分保存在第一个捕获物中 （因此是编号 1 ）； 由 "." 匹配到的字符是 2 号捕获物， 匹配到 "%s*" 的那部分是 3 号。

作为一个特例，空的捕获 () 将捕获到当前字符串的位置（它是一个数字）。 例如，如果将模式 "()aa()" 作用到字符串 "flaaap" 上，将产生两个捕获物： 3 和 5 。


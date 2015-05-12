## 5.1 UTF-8 支持 ##
这个库提供了对 UTF-8 编码的基础支持。 所有的函数都放在表 utf8 中。 此库不提供除编码处理之外的任何 Unicode 支持。 所有需要了解字符含义的操作，比如字符分类，都不在此范畴。

除非另有说明， 当一个函数需要一个字节位置的参数时， 都假定这个位置要么从字节序列的开始计算， 要么从字符串长度加一的位置算。 和字符串库一样，负的索引从字符串末尾计起。

**5.1.1 utf8.char (···)**

接收零或多个整数， 将每个整数转换成对应的 UTF-8 字节序列，并返回这些序列连接到一起的字符串。

**5.1.2 utf8.charpattern**

用于精确匹配到一个 UTF-8 字节序列的模式（是一个字符串，并非函数）"[\0-\x7F\xC2-\xF4][\x80-\xBF]*"。 它假定处理的对象是一个合法的 UTF-8 字符串。

**5.1.3 utf8.codes (s)**

返回一系列的值，可以让

     for p, c in utf8.codes(s) do body end
迭代出字符串 s 中所有的字符。 这里的 p 是位置（按字节数）而 c 是每个字符的编号。 如果处理到一个不合法的字节序列，将抛出一个错误。

**5.1.4 utf8.codepoint (s [, i [, j]])**

一整数形式返回 s 中 从位置 i 到 j 间（包括两端） 所有字符的编号。 默认的 i 为 1 ，默认的 j 为 i。 如果碰上不合法的字节序列，抛出一个错误。

**5.1.5 utf8.len (s [, i [, j]])**

返回字符串 s 中 从位置 i 到 j 间 （包括两端） UTF-8 字符的个数。 默认的 i 为 1 ，默认的 j 为 -1 。 如果它找到任何不合法的字节序列， 返回假值加上第一个不合法字节的位置。

**5.1.6 utf8.offset (s, n [, i])**

返回编码在 s 中的第 n 个字符的开始位置（按字节数） （从位置 i 处开始统计）。 负 n 则取在位置 i 前的字符。 当 n 是非负数时，默认的 i 是 1， 否则默认为 #s + 1。 因此，utf8.offset(s, -n) 取字符串的倒数第 n 个字符的位置。 如果指定的字符不在其中或在结束点之后，函数返回 nil。
作为特例，当 n 等于 0 时， 此函数返回含有 s 第 i 字节的那个字符的开始位置。

这个函数假定 s 是一个合法的 UTF-8 字符串。

## 5.2 表处理 ##
这个库提供了表处理的通用函数。 所有函数都放在表 table 中。

记住，无论何时，若一个操作需要取表的长度， 这张表必须是一个真序列，或是拥有 __len 元方法。 所有的函数都忽略传入参数的那张表中的非数字键。

**5.2.1 table.concat (list [, sep [, i [, j]]])**

提供一个列表，其所有元素都是字符串或数字，返回字符串 list[i]..sep..list[i+1] ··· sep..list[j]。 sep 的默认值是空串， i 的默认值是 1 ， j 的默认值是 #list 。 如果 i 比 j 大，返回空串。

**5.2.2 table.insert (list, [pos,] value)**

在 list 的位置 pos 处插入元素 value ， 并后移元素 list[pos], list[pos+1], ···, list[#list] 。 **pos 的默认值为 #list+1 ， 因此调用 table.insert(t,x) 会将 x 插在列表 t 的末尾。**

**5.2.3 table.move (a1, f, e, t [,a2])**

**将元素从表 a1 移到表 a2**。 这个函数做了次等价于后面这个多重赋值的等价操作： a2[t],··· = a1[f],···,a1[e]。 a2 的默认值为 a1。 目标区间可以和源区间重叠。 索引 f 必须是正数。

**5.2.4 table.pack (···)**

返回用所有参数以键 1,2, 等填充的新表， 并将 "n" 这个域设为参数的总数。 注意这张返回的表不一定是一个序列。

**5.2.5 table.remove (list [, pos])**

**移除 list 中 pos 位置上的元素，并返回这个被移除的值。** 当 pos 是在 1 到 #list 之间的整数时， 它向前移动元素　list[pos+1], list[pos+2], ···, list[#list] 并删除元素 list[#list]； 索引 pos 可以是 #list + 1 ，或在 #list 为 0 时可以是 0 ； 在这些情况下，函数删除元素 list[pos]。

pos 默认为 #list， 因此调用 table.remove(t) 将移除表t的最后一个元素。

**5.2.6 table.sort (list [, comp])**

在表内从 list[1] 到 list[#list] 原地 对其间元素按指定次序排序。 **如果提供了 comp ， 它必须是一个可以接收两个列表内元素为参数的函数。**当第一个元素需要排在第二个元素之前时，返回真 （因此 not comp(list[i+1],list[i]) 在排序结束后将为真）。 如果没有提供 comp， 将使用标准 Lua 操作 < 作为替代品。

**排序算法并不稳定； 即当两个元素次序相等时，它们在排序后的相对位置可能会改变。**

**5.2.7 table.unpack (list [, i [, j]])**

返回列表中的元素。 这个函数等价于

     return list[i], list[i+1], ···, list[j]
i 默认为 1 ，j 默认为 #list。

**示例代码1：**

	local tb = {10, 20, 30}
	
	table.insert(tb, 40) 
	for k, v in ipairs(tb) do
		print(k.." -> "..v) -- 在table的最后插入，结果为：{10, 20, 30, 40}
	end
	print("---")
	
	table.insert(tb, 2, 15) -- 在table的位置2插入，结果为：{10, 15, 20, 30, 40}
	for k, v in ipairs(tb) do
		print(k.." -> "..v)
	end
	print("---")
	
	print(table.remove(tb))  
	for k, v in ipairs(tb) do
		print(k.." -> "..v) -- 删除最后一个元素并且返回40，结果为：{10, 15, 20, 30}
	end
	print("---")
	
	print(table.remove(tb, 1))  
	for k, v in ipairs(tb) do
		print(k.." -> "..v) -- 删除第一个元素并返回10，结果为：{15, 20, 30}
	end

**示例代码2：**

	local tb = {20, 10, 2, 3, 4, 89, 20, 33, 2, 3}
	
	-- 默认是升序排序
	table.sort(tb)
	for _, v in ipairs(tb) do
		print(v)
	end
	
	print("---")
	
	-- 修改为降序排序
	table.sort(tb, function (a, b) if a > b then return true end end)
	for _, v in ipairs(tb) do
		print(v)
	end
运行结果

	2
	2
	3
	3
	4
	10
	20
	20
	33
	89
	---
	89
	33
	20
	20
	10
	4
	3
	3
	2
	2

**示例代码3：**

table.pack(···) - 将所有参数打包到一个表中

	tab = table.pack("hello", "world", "lua", 1, 4)
	for k,v in pairs(tab) do
	  print(k,v)
	end

运行结果：

	1       hello
	2       world
	3       lua
	4       1
	5       4
	n       5

接收任意多个参数，将所有参数打包到一个下标从1开始的表中，像上边，打包完后tab1 = {"hello", "world", "lua", 1, 4}但是不一定所有参数都是顺序打包的。

table.unpack (list [, i [, j]]) - 跟table.pack相反

**table.unpack 示例代码：**

	tab = {"hello", "world", "lua", 1, 4}
	a, b, c, d, e = table.unpack(tab)
	print(a) -- hello
	print(b) -- world
	print(c) -- lua
	print(d) -- 1
	print(e) -- 4

## 5.3 数学函数 ##

这个库提供了基本的数学函数。 所以函数都放在表 math 中。 注解有 "integer/float" 的函数会对整数参数返回整数结果， 对浮点（或混合）参数返回浮点结果。 圆整函数（math.ceil, math.floor, math.modf） 在结果在整数范围内时返回整数，否则返回浮点数。

**5.3.1 math.abs (x)**

返回 x 的绝对值。(integer/float)

**5.3.2 math.acos (x)**

返回 x 的反余弦值（用弧度表示）。

**5.3.3 math.asin (x)**

返回 x 的反正弦值（用弧度表示）。

**5.3.4 math.atan (y [, x])**

返回 y/x 的反正切值（用弧度表示）。 它会使用两个参数的符号来找到结果落在哪个象限中。 （即使 x 为零时，也可以正确的处理。）

默认的 x 是 1 ， 因此调用 math.atan(y) 将返回 y 的反正切值。

**5.3.5 math.ceil (x)**

返回不小于 x 的最小整数值。

**5.3.6 math.cos (x)**

返回 x 的余弦（假定参数是弧度）。

**5.3.7 math.deg (x)**

将角 x 从弧度转换为角度。

**5.3.8 math.exp (x)**

返回 ex 的值 （e 为自然对数的底）。

**5.3.9 math.floor (x)**

返回不大于 x 的最大整数值。

**5.3.10 math.fmod (x, y)**

返回 x 除以 y，将商向零圆整后的余数。 (integer/float)

**5.3.11 math.huge**

浮点数 HUGE_VAL， 这个数比任何数字值都大。

**5.3.12 math.log (x [, base])**

返回以指定底的 x 的对数。 默认的 base 是 e （因此此函数返回 x 的自然对数）。

**5.3.13 math.max (x, ···)**

返回参数中最大的值， 大小由 Lua 操作 < 决定。 (integer/float)

**5.3.14 math.maxinteger**

最大值的整数。

**5.3.15 math.min (x, ···)**

返回参数中最小的值， 大小由 Lua 操作 < 决定。 (integer/float)

**5.3.16 math.mininteger**

最小值的整数。

**5.3.17 math.modf (x)**

返回 x 的整数部分和小数部分。 第二个结果一定是浮点数。

**5.3.18 math.pi**

π 的值。

**5.3.19 math.rad (x)**

将角 x 从角度转换为弧度。

**5.3.20 math.random ([m [, n]])**

**当不带参数调用时， 返回一个 [0,1) 区间内一致分布的浮点伪随机数。** 

**当以两个整数 m 与 n 调用时， math.random 返回一个 [m, n] 区间 内一致分布的整数伪随机数。 （值 m-n 不能是负数，且必须在 Lua 整数的表示范围内。） 调用 math.random(n) 等价于 math.random(1,n)。**

这个函数是对 C 提供的位随机数函数的封装。 对其统计属性不作担保。

**5.3.21 math.randomseed (x)**

把 x 设为伪随机数发生器的“种子”： 相同的种子产生相同的随机数列。

**5.3.22 math.sin (x)**

返回 x 的正弦值（假定参数是弧度）。

**5.3.23 math.sqrt (x)**

返回 x 的平方根。 （你也可以使用乘方 x^0.5 来计算这个值。）

**5.3.24 math.tan (x)**

返回 x 的正切值（假定参数是弧度）。

**5.3.25 math.tointeger (x)**

如果 x 可以转换为一个整数， 返回该整数。 否则返回 nil。

**5.3.26 math.type (x)**

如果 x 是整数，返回 "integer"， 如果它是浮点数，返回 "float"， 如果 x 不是数字，返回 nil。

**5.3.27 math.ult (m, n)**

如果整数 m 和 n 以无符号整数形式比较， m 在 n 之下，返回布尔真否则返回假。

**示例代码：**

	print(math.sin(math.rad(30)))           -- 0.5
	print(math.floor(5.6))                  -- 5
	print(math.ceil(5.6))                   -- 6
	print(math.max(2, 3, 2, 14, 2, 30, -3)) -- 30
	print(math.min(2, 3, 2, 14, 2, 30, -3)) -- -3

随机数：

	print(math.random())     -- 输出一个大于等于0，小于1的值
	print(math.random(2))    -- 输出不是1就是2
	print(math.random(3, 4)) -- 输出不是3就是4

这里每次调用都是一样的值

使用了时间种子的随机数：

	math.randomseed(os.time())
	print(math.random())     -- 输出一个大于等于0，小于1的值
	print(math.random(2))    -- 输出不是1就是2
	print(math.random(3, 4)) -- 输出不是3就是4

## 5.4 输入输出库 ##

I/O 库提供了两套不同风格的文件处理接口。 第一种风格使用**隐式的文件句柄**； 它提供设置默认输入文件及默认输出文件的操作， 所有的输入输出操作都针对这些默认文件。 第二种风格使用**显式的文件句柄**。

**当使用隐式文件句柄时， 所有的操作都由表 io 提供。 若使用显式文件句柄， io.open 会返回一个文件句柄，且所有的操作都由该文件句柄的方法来提供。**

表 io 中也提供了三个 和 C 中含义相同的预定义文件句柄： io.stdin， io.stdout， 以及 io.stderr。 I/O 库永远不会关闭这些文件。

除非另有说明， I/O 函数在出错时都返回 nil （第二个返回值为错误消息，第三个返回值为系统相关的错误码）。 成功时返回与 nil 不同的值。 在非 POSIX 系统上， 根据错误码取出错误消息的过程可能并非线程安全的， 因为这使用了 C 的全局变量 errno 。

**5.4.1 io.close ([file])**

等价于 file:close()。 不给出 file 时将关闭默认输出文件。

**5.4.2 io.flush ()**

等价于 io.output():flush()。

**5.4.3 io.input ([file])**

用文件名调用它时，（以文本模式）来打开该名字的文件， 并将文件句柄设为默认输入文件。 如果用文件句柄去调用它， 就简单的将该句柄设为默认输入文件。 如果调用时不传参数，它返回当前的默认输入文件。

在出错的情况下，函数抛出错误而不是返回错误码。

**5.4.4 io.lines ([filename ···])**

以读模式打开指定的文件名并返回一个迭代函数。 此迭代函数的工作方式和用一个已打开的文件去调用 file:lines(···) 得到的迭代器相同。 当迭代函数检测到文件结束， 它不返回值（让循环结束）并自动关闭文件。

调用 io.lines() （不传文件名） 等价于 io.input():lines("*l")； 即，它将按行迭代标准输入文件。 在此情况下，循环结束后它不会关闭文件。

在出错的情况下，函数抛出错误而不是返回错误码。

**5.4.5 io.open (filename [, mode])**

这个函数用字符串 mode 指定的模式打开一个文件。 返回新的文件句柄。 当出错时，返回 nil 加错误消息。

mode 字符串可以是下列任意值：

- "r": 读模式（默认）；
- "w": 写模式；
- "a": 追加模式；
- "r+": 更新模式，所有之前的数据都保留；
- "w+": 更新模式，所有之前的数据都删除；
- "a+": 追加更新模式，所有之前的数据都保留，只允许在文件尾部做写入。

mode 字符串可以在最后加一个 'b' ， 这会在某些系统上以二进制方式打开文件。

**5.4.6 io.output ([file])**

类似于 io.input。 不过都针对默认输出文件操作。

**5.4.7 io.popen (prog [, mode])**

这个函数和系统有关，不是所有的平台都提供。

用一个分离进程开启程序 prog， 返回的文件句柄可用于从这个程序中读取数据 （如果 mode 为 "r"，这是默认值） 或是向这个程序写入输入（当 mode 为 "w" 时）。

**5.4.8 io.read (···)**

等价于 io.input():read(···)。从当前输入文件中读取字符串，它的参数决定了要读取的数据：

- “*all”	读取整个文件
- “*line”	读取下一行
- “*number”	读取一个数字
- <num>	    读取一个不超过<num>个字符的字符串

**5.4.9 io.tmpfile ()**

返回一个临时文件的句柄。 这个文件以更新模式打开，在程序结束时会自动删除。

**5.4.10 io.type (obj)**

检查 obj 是否是合法的文件句柄。 如果 obj 它是一个打开的文件句柄，返回字符串 "file"。 如果 obj 是一个关闭的文件句柄，返回字符串 "closed file"。 如果 obj 不是文件句柄，返回 nil 。

**5.4.11 io.write (···)**

等价于 io.output():write(···)。

**5.4.12 file:close ()**

关闭 file。 注意，文件在句柄被垃圾回收时会自动关闭， 但是多久以后发生，时间不可预期的。

当关闭用 io.popen 创建出来的文件句柄时， file:close 返回 os.execute 会返回的一样的值。

**5.4.13 file:flush ()**

将写入的数据保存到 file 中。

**5.4.14 file:lines (···)**

返回一个迭代器函数， 每次调用迭代器时，都从文件中按指定格式读数据。 如果没有指定格式，使用默认值 "l" 。 看一个例子

     for c in file:lines(1) do body end
会从文件当前位置开始，中不断读出字符。 和 io.lines 不同， 这个函数在循环结束后不会关闭文件。

在出错的情况下，函数抛出错误而不是返回错误码。

**5.4.15 file:read (···)**

读文件 file， 指定的格式决定了要读什么。 对于每种格式，函数返回读出的字符对应的字符串或数字。 若不能以该格式对应读出数据则返回 nil。 （对于最后这种情况， 函数不会读出后续的格式。） 当调用时不传格式，它会使用默认格式读下一行（见下面描述）。

提供的格式有

- "n": 读取一个数字，根据 Lua 的转换文法，可能返回浮点数或整数。 （数字可以有前置或后置的空格，以及符号。） 只要能构成合法的数字，这个格式总是去读尽量长的串； 如果读出来的前缀无法构成合法的数字 （比如空串，"0x" 或 "3.4e-"）， 就中止函数运行，返回 nil。
- "i": 读取一个整数，返回整数值。
- "a": 从当前位置开始读取整个文件。 如果已在文件末尾，返回空串。
- "l": 读取一行并忽略行结束标记。 当在文件末尾时，返回 nil 这是默认格式。
- "L": 读取一行并保留行结束标记（如果有的话）， 当在文件末尾时，返回 nil。
- number: 读取一个不超过这个数量字节数的字符串。 当在文件末尾时，返回 nil。 如果 number 为零， 它什么也不读，返回一个空串。 当在文件末尾时，返回 nil。
格式 "l" 和 "L" 只能用于文本文件。

**5.4.16 file:seek ([whence [, offset]])**

设置及获取基于文件开头处计算出的位置。 设置的位置由 offset 和 whence 字符串 whence 指定的基点决定。基点可以是：

- "set": 基点为 0 （文件开头）；
- "cur": 基点为当前位置了；
- "end": 基点为文件尾；

当 seek 成功时，返回最终从文件开头计算起的文件的位置。 当 seek 失败时，返回 nil 加上一个错误描述字符串。

whence 的默认值是 "cur"， offset 默认为 0 。 因此，调用 file:seek() 可以返回文件当前位置，并不改变它； 调用 file:seek("set") 将位置设为文件开头（并返回 0）； 调用 file:seek("end") 将位置设到文件末尾，并返回文件大小。

**5.4.17 file:setvbuf (mode [, size])**

设置输出文件的缓冲模式。 有三种模式：

- "no": 不缓冲；输出操作立刻生效。
- "full": 完全缓冲；只有在缓存满或当你显式的对文件调用 flush 时才真正做输出操作。
- "line": 行缓冲； 输出将到每次换行前， 对于某些特殊文件（例如终端设备）缓冲到任何输入前。
对于后两种情况，size 以字节数为单位 指定缓冲区大小。 默认会有一个恰当的大小。

**5.4.18 file:write (···)**

将参数的值逐个写入 file。 参数必须是字符串或数字。

成功时，函数返回 file。 否则返回 nil 加错误描述字符串。

**简单 IO 示例程序：**

数据写入文件：

	io.output("textfile")  
	io.write("hello", "\n")
	io.write("lua", "\n")

读取文件内容：

	io.input("textfile")   
	print(io.read("*all"))-- 读取所有

一行一行读取数据：

	io.input("textfile")   
	
	local mark = io.read(0)
	while mark do
		print(io.read("*line"))
		mark = io.read(0)
		if not mark then
			print("File end.")
			break
		end
	end

**完成 IO 模型示例程序：**

	local textfile = io.open("textfile", "w+")
	if textfile then
		textfile:write("omg lua\n")
		textfile:flush()	
		textfile:close()
	end
	
	local textfile = io.open("textfile", "r")
	if textfile then
		print(textfile:read("*all"))
		textfile:close()
	end


## 5.5 操作系统库 ##

这个库都通过表 os 实现。

**5.5.1 os.clock ()**

返回程序使用的按秒计 CPU 时间的近似值。

**5.5.2 os.date ([format [, time]])**

**返回一个包含日期及时刻的字符串或表。 格式化方法取决于所给字符串 format。**

如果提供了 time 参数， 格式化这个时间 （这个值的含义参见 os.time 函数）。 否则，date 格式化当前时间。

如果 format 以 '!' 打头， 日期以协调世界时格式化。 在这个可选字符项之后，

**如果 format 为字符串** "*t"， **date 返回有后续域的表： year （四位数字），month （1–12），day （1–31）， hour （0–23），min （0–59），sec （0–61）， wday （星期几，星期天为 1 ）， yday （当年的第几天）， 以及 isdst （夏令时标记，一个布尔量）。** 对于最后一个域，如果该信息不提供的话就不存在。

如果 format 并非 "*t"， date 以字符串形式返回， 格式化方法遵循 ISO C 函数 strftime 的规则。

如果不传参数调用， date 返回一个合理的日期时间串， 格式取决于宿主程序以及当前的区域设置 （即，os.date() 等价于 os.date("%c")）。

在非 POSIX 系统上， 由于这个函数依赖 C 函数 gmtime 和 localtime， 它可能并非线程安全的。

**5.5.3 os.difftime (t2, t1)**

返回以秒计算的时刻 t1 到 t2 的差值。 （这里的时刻是由 os.time 返回的值）。 在 POSIX，Windows，和其它一些系统中，这个值就等于 t2-t1。

**5.5.4 os.execute ([command])**

这个函数等价于 ISO C 函数 **system**。 它调用系统解释器执行 command。 如果命令成功运行完毕，第一个返回值就是 true， 否则是 nil otherwise。 在第一个返回值之后，函数返回一个字符串加一个数字。如下：

"exit": 命令正常结束； 接下来的数字是命令的退出状态码。
"signal": 命令被信号打断； 接下来的数字是打断该命令的信号。
如果不带参数调用， os.execute 在系统解释器存在的时候返回真。

**5.5.5 os.exit ([code [, close]])**

调用 ISO C 函数 exit 终止宿主程序。 如果 code 为 true， 返回的状态码是 EXIT_SUCCESS； 如果 code 为 false， 返回的状态码是 EXIT_FAILURE； 如果 code 是一个数字， 返回的状态码就是这个数字。 code 的默认值为 true。

如果第二个可选参数 close 为真， 在退出前关闭 Lua 状态机。

**5.5.6 os.getenv (varname)**

返回进程环境变量 varname 的值， 如果该变量未定义，返回 nil 。

**5.5.7 os.remove (filename)**

删除指定名字的文件（在 POSIX 系统上可以是一个空目录） 如果函数失败，返回 nil 加一个错误描述串及出错码。

**5.5.8 os.rename (oldname, newname)**

**将名字为 oldname 的文件或目录更名为 newname。** 如果函数失败，返回 nil 加一个错误描述串及出错码。

**5.5.9 os.setlocale (locale [, category])**

**设置程序的当前区域。** locale 是一个区域设置的系统相关字符串； category 是一个描述有改变哪个分类的可选字符串： "all"，"collate"， "ctype"， "monetary"， "numeric"， 或 "time"； 默认的分类为 "all"。 此函数返回新区域的名字。 如果请求未被获准，返回 nil 。

当 locale 是一个空串， 当前区域被设置为一个在实现中定义好的本地区域。 当 locale 为字符串 "C"， 当前区域被设置为标准 C 区域。

当第一个参数为 nil 时， 此函数仅返回当前区域指定分类的名字。

**由于这个函数依赖 C 函数 setlocale， 它可能并非线程安全的。**

**5.5.10 os.time ([table])**

**当不传参数时，返回当前时刻。 如果传入一张表，就返回由这张表表示的时刻。 这张表必须包含域 year，month，及 day； 可以包含有　hour （默认为 12 ）， min （默认为 0）， sec （默认为 0），以及 isdst （默认为 nil）。 关于这些域的详细描述，参见 os.date 函数。**

返回值是一个含义由你的系统决定的数字。 在 POSIX，Windows，和其它一些系统中， 这个数字统计了从指定时间（"epoch"）开始经历的秒数。 对于另外的系统，其含义未定义， 你只能把 time 的返回数字用于 os.date 和 os.difftime 的参数。

**5.5.11 os.tmpname ()**

返回一个可用于临时文件的文件名字符串。 这个文件在使用前必须显式打开，不再使用时需要显式删除。

在 POSIX 系统上， 这个函数会以此文件名创建一个文件以回避安全风险。 （别人可能未经允许在获取到这个文件名到创建该文件之间的时刻创建此文件。） 你依旧需要在使用它的时候先打开，并最后删除（即使你没使用到）。

只有有可能，你更应该使用 io.tmpfile， 因为该文件可以在程序结束时自动删除。

**示例1：**

	os.execute("ls")

**示例2 获取当前时间:**

	local tb = os.date("*t", os.time());
	for i, v in pairs(tb) do
	      print(i, v);
	end

运行结果：

	day     20
	min     4
	hour    23
	sec     51
	wday    2
	isdst   true
	month   4
	yday    110
	year    2015

对于其它的格式字符串，os.date会将日期格式化为一个字符串

示例：

	print(os.date("today is %A, in %B"))      -->today is Monday, in April
	print(os.date("%x", 906000490))           -->09/16/1998

所有格式化字符串如下：

- %a      一星期中天数的简写                      （Wed）
- %A      一星期中天数的全称                      （Wednesday）
- %b      月份的简写                                  （Sep）
- %B      月份的全称                                  （September）
- %c      日期和时间                                   （09/16/98 23:48:10）
- %d      一个月中的第几天                          （16）[0 ~ 31]
- %H      24小时制中的小时数                      （23）[00 ~ 23]
- %I      12小时制中的小时数                       （11）[01 ~ 12]
- %j      一年中的第几天                             （259）[01 ~ 366]
- %M      分钟数                                       （48）[00 ~ 59]
- %m      月份数                                       （09）[01 ~ 12]     
- %P      "上午(am)" 或 "下午(pm)"               (pm)
- %S      秒数                                          （10）[00 ~ 59]
- %w      一星期中的第几天                         （3）[0 ~ 6 = 星期天 ~ 星期六]
- %W　　一年中的第几个星期　　　　　　　　0 ~ 52
%x      日期                                          （09/16/98）
%X      时间                                          （23:48:10）
%y      两位数的年份                               （90）[00 ~ 99]
%Y      完整的年份                                 （2009）
%%      字符串'%'

## 5.6 调试库 ##

这个库提供了 Lua 程序调试接口的功能。 其中一些函数违反了 Lua 代码的基本假定 （例如，不会从函数之外访问函数的局部变量； 用户数据的元表不会被 Lua 代码修改； Lua 程序不会崩溃）， 因此它们有可能危害到其它代码的安全性。 此外，库里的一些函数可能运行的很慢。

这个库里的所有函数都提供在表 debug 内。 所有操作线程的函数，可选的第一个参数都是针对的线程。 默认值永远是当前线程。

**5.6.1 debug.debug ()**

进入一个用户交互模式，运行用户输入的每个字符串。 使用简单的命令以及其它调试设置，用户可以检阅全局变量和局部变量， 改变变量的值，计算一些表达式，等等。 输入一行仅包含 cont 的字符串将结束这个函数， 这样调用者就可以继续向下运行。

注意，debug.debug 输入的命令在文法上并没有内嵌到任何函数中， 因此不能直接去访问局部变量。

**5.6.2 debug.gethook ([thread])**

返回三个表示线程钩子设置的值： 当前钩子函数，当前钩子掩码，当前钩子计数 （debug.sethook 设置的那些）。

**5.6.3 debug.getinfo ([thread,] f [, what])**

返回关于一个函数信息的表。 你可以直接提供该函数， 也可以用一个数字 f 表示该函数。 数字 f 表示运行在指定线程的调用栈对应层次上的函数： 0 层表示当前函数（getinfo 自身）； 1 层表示调用 getinfo 的函数 （除非是尾调用，这种情况不计入栈）；等等。 如果 f 是一个比活动函数数量还大的数字， getinfo 返回 nil。

只有字符串 what 中有描述要填充哪些项， 返回的表可以包含 lua_getinfo 能返回的所有项。 what 默认是返回提供的除合法行号表外的所有信息。 对于选项 'f' ，会在可能的情况下，增加 func 域保存函数自身。 对于选项 'L' ，会在可能的情况下，增加 activelines 域保存合法行号表。

例如,表达式 debug.getinfo(1,"n") 返回带有当前函数名字信息的表（如果找的到名字的话）， 表达式 debug.getinfo(print) 返回关于 print 函数的 包含有所有能提供信息的表。

**5.6.4 debug.getlocal ([thread,] f, local)**

此函数返回在栈的 f 层处函数的索引为 local 的局部变量 的名字和值。 这个函数不仅用于访问显式定义的局部变量，也包括形参、临时变量等。

第一个形参或是定义的第一个局部变量的索引为 1 ， 然后遵循在代码中定义次序，以次类推。 其中只计算函数当前作用域的活动变量。 负索引指可变参数； -1 指第一个可变参数。 如果该索引处没有变量，函数返回 nil。 若指定的层次越界，抛出错误。 （你可以调用 debug.getinfo 来检查层次是否合法。）

以 '(' （开括号）打头的变量名表示没有名字的变量 （比如是循环控制用到的控制变量， 或是去除了调试信息的代码块）。

参数 f 也可以是一个函数。 这种情况下，getlocal 仅返回函数形参的名字。

**5.6.5 debug.getmetatable (value)**

返回给定 value 的元表。 若其没有元表则返回 nil 。

debug.getregistry ()

返回注册表。

**5.6.6 debug.getupvalue (f, up)**

此函数返回函数 f 的第 up 个上值的名字和值。 如果该函数没有那个上值，返回 nil 。

以 '(' （开括号）打头的变量名表示没有名字的变量 （去除了调试信息的代码块）。

**5.6.7 debug.getuservalue (u)**

返回关联在 u 上的 Lua 值。 如果 u 并非用户数据，返回 nil。

**5.6.8 debug.sethook ([thread,] hook, mask [, count])**

将一个函数作为钩子函数设入。 字符串 mask 以及数字 count 决定了钩子将在何时调用。 掩码是由下列字符组合成的字符串，每个字符有其含义：

'c': 每当 Lua 调用一个函数时，调用钩子；
'r': 每当 Lua 从一个函数内返回时，调用钩子；
'l': 每当 Lua 进入新的一行时，调用钩子。
此外， 传入一个不为零的 count ， 钩子将在每运行 count 条指令时调用。

如果不传入参数， debug.sethook 关闭钩子。

当钩子被调用时， 第一个参数是触发这次调用的事件： "call" （或 "tail call"）， "return"， "line"， "count"。 对于行事件， 钩子的第二个参数是新的行号。 在钩子内，你可以调用 getinfo ，指定第 2 层， 来获得正在运行的函数的详细信息 （0 层指 getinfo 函数， 1 层指钩子函数）。

**5.6.9 debug.setlocal ([thread,] level, local, value)**

这个函数将 value 赋给 栈上第 level 层函数的第 local 个局部变量。 如果没有那个变量，函数返回 nil 。 如果 level 越界，抛出一个错误。 （你可以调用 debug.getinfo 来检查层次是否合法。） 否则，它返回局部变量的名字。

关于变量索引和名字，参见 debug.getlocal。

**5.6.10 debug.setmetatable (value, table)**

将 value 的元表设为 table （可以是 nil）。 返回 value。

**5.6.11 debug.setupvalue (f, up, value)**

这个函数将 value 设为函数 f 的第 up 个上值。 如果函数没有那个上值，返回 nil 否则，返回该上值的名字。

**5.6.12 debug.setuservalue (udata, value)**

将 value 设为 udata 的关联值。 udata 必须是一个完全用户数据。

返回 udata。

**5.6.13 debug.traceback ([thread,] [message [, level]])**

如果 message 有，且不是字符串或 nil， 函数不做任何处理直接返回 message。 否则，它返回调用栈的栈回溯信息。 字符串可选项 message 被添加在栈回溯信息的开头。 数字可选项 level 指明从栈的哪一层开始回溯 （默认为 1 ，即调用 traceback 的那里）。

**5.6.14 debug.upvalueid (f, n)**

返回指定函数第 n 个上值的唯一标识符（一个轻量用户数据）。

这个唯一标识符可以让程序检查两个不同的闭包是否共享了上值。 若 Lua 闭包之间共享的是同一个上值 （即指向一个外部局部变量），会返回相同的标识符。

**5.6.15 debug.upvaluejoin (f1, n1, f2, n2)**

让 Lua 闭包 f1 的第 n1 个上值 引用 Lua 闭包 f2 的第 n2 个上值。

























**这个部分描述了 Lua 的 C API ， 也就是宿主程序跟 Lua 通讯用的一组 C 函数。 所有的 API 函数按相关的类型以及常量都声明在头文件 lua.h 中。**

虽然我们说的是“函数”， 但一部分简单的 API 是以宏的形式提供的。 除非另有说明， 所有的这些宏都只使用它们的参数一次 （除了第一个参数，那一定是 Lua 状态）， 因此你不需担心这些宏的展开会引起一些副作用。

C 库中所有的 Lua API 函数都不去检查参数是否相容及有效。 然而，你可以在编译 Lua 时加上打开一个宏开关 LUA_USE_APICHECK 来改变这个行为。

## 6.1 栈 ##
**Lua 使用一个 虚拟栈 来和 C 互传值。栈上的的每个元素都是一个 Lua 值 （nil，数字，字符串，等等）。**

无论何时 Lua 调用 C，被调用的函数都得到一个新的栈， 这个栈独立于 C 函数本身的栈，也独立于之前的 Lua 栈。 它里面包含了 Lua 传递给 C 函数的所有参数， 而 C 函数则把要返回的结果放入这个栈以返回给调用者 （参见 lua_CFunction）。

方便起见， 所有针对栈的 API 查询操作都不严格遵循栈的操作规则。 **而是可以用一个 索引 来指向栈上的任何元素： 正的索引指的是栈上的绝对位置（从1开始）； 负的索引则指从栈顶开始的偏移量。** 展开来说，如果堆栈有 n 个元素， 那么索引 1 表示第一个元素 （也就是最先被压栈的元素） 而索引 n 则指最后一个元素； 索引 -1 也是指最后一个元素 （即栈顶的元素）， 索引 -n 是指第一个元素。

## 6.2 栈大小 ##
当你使用 Lua API 时， 就有责任保证做恰当的调用。 特别需要注意的是， 你有责任控制不要堆栈溢出。 你可以**使用 lua_checkstack 这个函数来扩大可用堆栈的尺寸。**

无论何时 Lua 调用 C ， 它都只保证至少有 LUA_MINSTACK 这么多的堆栈空间可以使用。 **LUA_MINSTACK 一般被定义为 20 **， 因此，只要你不是不断的把数据压栈， 通常你不用关心堆栈大小。

**当你调用一个 Lua 函数却没有指定要接收多少个返回值时 （参见 lua_call），lua 可以保证栈一定有足够的空间来接收所有的返回值， 但不保证此外留有额外的空间。 因此，在做了一次这样的调用后，如果你需要继续压栈， 则需要使用 lua_checkstack。**

## 6.3 有效索引与可接受索引 ##
API 中的函数若需要传入栈索引，这个索引必须是 有效索引 或是 可接受索引。

**有效索引 指引用栈内真实位置的索引； 即在 1 到栈顶之间的位置 （1 ≤ abs(index) ≤ top）。** 通常，一个可能修改该位置的值的函数需要传入有效索引。

除非另有说明， 任何可以接受有效索引的函数同时也接受 伪索引。 伪索引指代一些可以被 C code 访问得到 Lua 值，而它们又不在栈内。 这用于访问注册表以及 C 函数的上值（参见 §6.4）。

对于那些只是需要栈中的值（例如查询函数） 而不需要指定一个栈位置的函数， 可以用一个可接受的索引去调用它们。 可接受索引 不仅可以是任何包括伪索引在内的有效索引， 还可以是任何超过栈顶但落在为栈分配出来的空间内的正索引。 （注意 0 永远都不是一个可接受索引。） 除非另有说明，API 里的函数都接受可接受索引。

允许可接受索引是为了避免对栈顶以外的查询时做额外的检查。 例如，C 函数可以直接查询传给它的第三个参数， 而不用先检查是不是有第三个参数， 即不需要检查 3 是不是一个有效索引。

对于那些以可接受索引调用的函数， 无效索引被看作包含了一个虚拟类型 LUA_TNONE 的值， 这个值的行为和 nil 一致。

## 6.4 C 闭包 ##

**当 C 函数被创建出来， 我们有可能会把一些值关联在一起， 也就是创建一个 C 闭包** （参见 lua_pushcclosure）； 这些被关联起来的值被叫做 上值 ， 它们可以在函数被调用的时候访问的到。

无论何时去调用 C 函数， 函数的上值都可以用伪索引定位。 我们可以用 lua_upvalueindex 这个宏来生成这些伪索引。 第一个关联到函数的值放在 lua_upvalueindex(1) 位置处，依此类推。 使用 lua_upvalueindex(n) 时， 若 n 大于当前函数的总上值个数 （但不可以大于 256）会产生一个可接受的但无效的索引。

## 6.5 lua C接口 ##
lua提供了一些系列C函数来和C语言交互(通过虚拟栈来交互)。下文列出了部分函数，更多函数和说明请参考[官方文档中文版](http://cloudwu.github.io/lua53doc/manual.html#4.8)。

**6.5.1 lua_checkstack**

[-0, +0, –]
int lua_checkstack (lua_State *L, int n);
确保堆栈上至少有 n 个额外空位。 如果不能把堆栈扩展到相应的尺寸，函数返回假。 失败的原因包括将把栈扩展到比固定最大尺寸还大 （至少是几千个元素）或分配内存失败。 这个函数永远不会缩小堆栈； 如果堆栈已经比需要的大了，那么就保持原样。

**6.5.2 lua_close**

[-0, +0, –]
void lua_close (lua_State *L);
销毁指定 Lua 状态机中的所有对象 （如果有垃圾收集相关的元方法的话，会调用它们）， 并且释放状态机中使用的所有动态内存。 在一些平台上，你可以不必调用这个函数， 因为当宿主程序结束的时候，所有的资源就自然被释放掉了。 另一方面，长期运行的程序，比如一个后台程序或是一个网站服务器， 会创建出多个 Lua 状态机。那么就应该在不需要时赶紧关闭它们。

**6.5.3 lua_getglobal**

[-0, +1, e]
int lua_getglobal (lua_State *L, const char *name);
把全局变量 name 里的值压栈，返回该值的类型。

**6.5.4 lua_isnumber**

[-0, +0, –]
int lua_isnumber (lua_State *L, int index);
当给定索引的值是一个数字，或是一个可转换为数字的字符串时，返回 1 ，否则返回 0 。

**6.5.5 lua_isstring**

[-0, +0, –]
int lua_isstring (lua_State *L, int index);
当给定索引的值是一个字符串或是一个数字 （数字总能转换成字符串）时，返回 1 ，否则返回 0 。

**6.5.6 lua_newstate**

[-0, +0, –]
lua_State *lua_newstate (lua_Alloc f, void *ud);
创建一个运行在新的独立的状态机中的线程。 如果无法创建线程或状态机（由于内存有限）则返回 NULL。 参数 f 是一个分配器函数； Lua 将通过这个函数做状态机内所有的内存分配操作。 第二个参数 ud ，这个指针将在每次调用分配器时被转入。

**6.5.7 lua_newtable**

[-0, +1, e]
void lua_newtable (lua_State *L);
创建一张空表，并将其压栈。 它等价于 lua_createtable(L, 0, 0) 。

## 6.6 辅助库 ##
**辅助库提供了一些便捷函数，方便在 C 中为 Lua 编程。 基础 API 提供了 C 和 Lua 交互用的主要函数， 而辅助库则为一些常见的任务提供了高阶函数。**

**所有辅助库中的函数和类型都定义在头文件 lauxlib.h 中， 它们均带有前缀 luaL_。**

辅助库中的所有函数都基于基础 API 实现。 故而它们并没有提供任何基础 API 实现不了的功能。 虽然如此，使用辅助库可以让你的代码**更为健壮**。

一些辅助库函数会在内部使用一些额外的栈空间。 当辅助库使用的栈空间少于五个时， 它们不去检查栈大小；而是简单的假设栈够用。

一些辅助库中的函数用于检查 C 函数的参数。 因为错误信息格式化为指代参数 （例如，"bad argument #1"）， 你就不要把这些函数用于参数之外的值了。

如果检查无法通过， luaL_check* 这些函数一定会抛出错误。

下文列出了部分辅助库函数，更多函数和说明请参考[官方文档中文版](http://cloudwu.github.io/lua53doc/manual.html#5)

**6.6.1 luaL_checknumber**

[-0, +0, v]
lua_Number luaL_checknumber (lua_State *L, int arg);
检查函数的第 arg 个参数是否是一个 数字，并返回这个数字。

**6.6.2 luaL_openlibs**

[-0, +0, e]
void luaL_openlibs (lua_State *L);
打开指定状态机中的所有 Lua 标准库。

**6.6.3 luaL_Reg**

typedef struct luaL_Reg {
  const char *name;
  lua_CFunction func;
} luaL_Reg;
用于 luaL_setfuncs 注册函数的数组类型。 name 指函数名，func 是函数指针。 任何 luaL_Reg 数组必须以一对 name 与 func 皆为 NULL 结束。

## 6.7 C调用lua的例子 ##
[原文](http://www.cnblogs.com/stephen-liu74/archive/2012/07/20/2460634.html)

conf.lua

	width = 200
	height = 300

parse_lua.c

	#include <stdio.h>
	#include <string.h>
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
	
	void load(lua_State* L, const char* fname, int* w, int* h) {
		if (luaL_loadfile(L,fname) || lua_pcall(L,0,0,0)) {
			printf("Error Msg is %s.\n",lua_tostring(L,-1));
			return;
		}
	
		lua_getglobal(L,"width");  // 先压入栈
		lua_getglobal(L,"height"); // 再压入栈
	
		if (!lua_isnumber(L,-2)) {
			printf("'width' should be a number\n" );
			return;
		}
	
		if (!lua_isnumber(L,-1)) {
			printf("'height' should be a number\n" );
			return;
		}
	
		*w = lua_tointeger(L,-2); // 从栈顶弹出数据
		*h = lua_tointeger(L,-1);
	}
	
	int main()
	{
		lua_State* L = luaL_newstate(); // 创建lua状态机
		int w,h;
	
		load(L,"conf.lua",&w,&h);
		printf("width = %d, height = %d\n",w,h);
	
		lua_close(L); // 销毁lua状态机
	
	 return 0;
	}

编译：

	gcc parse_lua.c -o parse_lua -llua -lm -ldl

运行：

	 ./parse_lua 
	width = 200, height = 300

这个简单的例子演示了在C语言中如何通过lua提供的虚拟栈来和lua交互。

## 6.8 lua调用C的例子##
[代码Github地址](https://github.com/peimin/lua_call_c_example)

lcal.c

	#include <lua.h>
	#include <lauxlib.h>
	#include <string.h>
	#include <stdlib.h>
	
	static int l_add(lua_State *L){
	    double d1 = luaL_checknumber(L, 1); // 这个1代表了从左到右lua调用这个函数时传入的第一个参数 lua只有double
		double d2 = luaL_checknumber(L, 2); 
	
	    lua_pushnumber(L, d1 + d2);			// 返回值压入lua栈
	    return 1;
	}
	  
	static int l_sub(lua_State *L){
	    double d1 = luaL_checknumber(L, 1);   
		double d2 = luaL_checknumber(L, 2); 
	
	    lua_pushnumber(L, d1 - d2);
	    return 1;
	} 
	 
	int luaopen_calc(lua_State *L){
		luaL_checkversion(L); // 防止链接多分lua库 
	
		luaL_Reg l[] = {
			{"add", l_add},		
			{"sub", l_sub},
			{NULL, NULL}
		};  
	 
		luaL_newlib(L, l); // Creates a new table and registers there the functions in list l. 
	    return 1;
	}

calc_test.lua

	-- 注意这里不能使用calc.lua来命名 这样会使得require在查找calc时出现死循环
	
	local calc = require "calc"
	
	print(calc.add(20, 40))
	print(calc.sub(20, 40))

Makefile

	calc.so : lcalc.c
		gcc -g -o $@ --shared $^  -I/usr/local/include -L/usr/local/bin -llua


运行结果：

	>make
	>lua calc_test.lua 
	60.0
	-20.0

在这是例子中我们用C语言编写了一个.so文件(动态库)，演示了lua使用虚拟栈来和C交互。

## 6.9 fflua ##

fflua用于C++嵌入lua或者是用C++扩展lua。

lua若要操作C++中的对象或函数，那么必须先把C++对应的接口注册都lua中。Lua C API提供了一系列的接口拥有完成此操作，lua通过虚拟栈来实现对C函数的调用。但是关于堆栈的操作总是会稍显头疼，fflua极大的简化了注册C++对象和接口的操作。

[fflua github地址](https://github.com/fanchy/fflua)官方的文档详细说明了怎么使用和使用中的一些问题等。

**[官方examples](https://github.com/fanchy/fflua/tree/master/example)**

我为main.cpp文件中添加了更加详细的注释，主要是说明了main()函数运行的时候是怎么一步一步打印出最后的结果的，可以根据注释一步一步了解lua和C++是怎么相互调用的。

main.cpp

	#include <iostream>
	#include <string>
	#include <assert.h>
	using namespace std;
	
	#include "lua/fflua.h"
	
	using namespace ff;
	
	class base_t
	{
	public:
	    base_t():v(789){}
		void dump()
		{
			printf("in %s a:%d\n", __FUNCTION__, v);
		}
		int v;
	};
	class foo_t: public base_t
	{
	public:
		foo_t(int b):a(b)
		{
			printf("in %s b:%d this=%p\n", __FUNCTION__, b, this);
		}
		~foo_t()
		{
			printf("in %s\n", __FUNCTION__);
		}
		void print(int64_t a, base_t* p) const
		{
			printf("in foo_t::print a:%ld p:%p\n", (long)a, p);
		}
	
		static void dumy()
		{
			printf("in %s\n", __FUNCTION__);
		}
		int a;
	};
	
	//! lua talbe 可以自动转换为stl 对象
	void dumy(map<string, string> ret, vector<int> a, list<string> b, set<int64_t> c)
	{
	    printf("in %s begin ------------\n", __FUNCTION__);
		for (map<string, string>::iterator it =  ret.begin(); it != ret.end(); ++it)
		{
			printf("map:%s, val:%s:\n", it->first.c_str(), it->second.c_str());
		}
		printf("in %s end ------------\n", __FUNCTION__);
	}
	
	class clazz{
	public:
	    static void static_func(){
	        printf("in clazz::%s end ------------\n", __FUNCTION__);
	    }
	};
	
	static void lua_reg(lua_State* ls)
	{
	    //! 注册基类函数, ctor() 为构造函数的类型
		fflua_register_t<base_t, ctor()>(ls, "base_t")  //! 注册构造函数
						.def(&base_t::dump, "dump")     //! 注册基类的函数
						.def(&base_t::v, "v");          //! 注册基类的属性
	
	    //! 注册子类，ctor(int) 为构造函数， foo_t为类型名称， base_t为继承的基类名称
		fflua_register_t<foo_t, ctor(int)>(ls, "foo_t", "base_t")
					.def(&foo_t::print, "print")        //! 子类的函数
					.def(&foo_t::a, "a");               //! 子类的字段
	
		fflua_register_t<>(ls)
					.def(&dumy, "dumy");                //! 注册静态函数
	
	    
	    fflua_register_t<clazz, ctor()>(ls, "clazz")
					.def(&clazz::static_func, "static_func"); 
	    
	}
	
	int main(int argc, char* argv[])
	{
	
		fflua_t fflua;
	    try 
	    {
	        //! 注册C++ 对象到lua中
	        fflua.reg(lua_reg);
	        
	        //! 载入lua文件
	        fflua.add_package_path("./");
	#ifdef _WIN32
	        fflua.load_file("../test.lua");
	#else
	        fflua.load_file("test.lua"); // lua文件里面调用了 clazz:static_func() 打印出 in clazz::static_func end ------------
	#endif
	        //! 获取全局变量
	        int var = 0;
	        assert(0 == fflua.get_global_variable("test_var", var));
	        //! 设置全局变量
	        assert(0 == fflua.set_global_variable("test_var", ++var));
	
	        //! 执行lua 语句 打印出 exe run_string!!
	        fflua.run_string("print(\"exe run_string!!\")");
	        
	        //! 调用lua test_func函数, 基本类型作为参数 打印出 in test_func:   1.0     2.0     3.0     4
			// lua test_func函数里面调用cpp的dumy函数打印出 
			// in dumy begin ------------
			// map:k, val:v:
			// in dumy end ------------
	        int32_t arg1 = 1;
	        float   arg2 = 2;
	        double  arg3 = 3;
	        string  arg4 = "4";
	        fflua.call<bool>("test_func", arg1, arg2, arg3,  arg4);
	        
	        //! 调用lua test_stl()函数，stl类型作为参数， 自动转换为lua talbe 打印出：
			// --------------dump_table begin ----------------
			// vec     1       100.0
			// lt      1       99.98999786377
			// st      1       OhNIce
			// mp      key     200.0
			// --------------dump_table end ----------------
	        vector<int> vec;        vec.push_back(100);
	        list<float> lt;         lt.push_back((float)99.99);
	        set<string> st;         st.insert("OhNIce");
	        map<string, int> mp;    mp["key"] = 200;
	        fflua.call<string>("test_stl", vec, lt, st,  mp);
	        
	        //! 调用lua 函数返回 talbe，自动转换为stl结构
	        vec = fflua.call<vector<int> >("test_return_stl_vector");
	        lt  = fflua.call<list<float> >("test_return_stl_list");
	        st  = fflua.call<set<string> >("test_return_stl_set");
	        mp  = fflua.call<map<string, int> >("test_return_stl_map");
	        
	        //! 调用lua函数，c++ 对象作为参数, foo_t 必须被注册过
			// new ffo_t(456) 打印出： in foo_t b:456 this=0x82cf468
			// 调用lua的test_object函数 打印出： base ptr:       137158880
			// test_object函数内调用 foo_obj:print 打印出： in foo_t::print a:12333 p:0x82ce0e0
			// test_object函数内调用 foo_obj:dump() 打印出：in dump a:789
			// test_object函数内 打印出： foo property    456.0
			// test_object函数内 打印出：	base property   789.0
	        foo_t* foo_ptr = new foo_t(456);
	        fflua.call<void>("test_object", foo_ptr);
	        
	        //! 调用lua函数，c++ 对象作为返回值, foo_t 必须被注册过 
	        assert(foo_ptr == fflua.call<foo_t*>("test_ret_object", foo_ptr));
	        //! 调用lua函数，c++ 对象作为返回值, 自动转换为基类
	        base_t* base_ptr = fflua.call<base_t*>("test_ret_base_object", foo_ptr);
	        assert(base_ptr == foo_ptr);
	 
	    }
	    catch (exception& e)
	    {
	        printf("exception:%s\n", e.what());
	
	
	    }
	#ifdef _WIN32
	    system("pause");
	#endif
	    return 0;
	}

test.lua 
	
	test_var = 99
	
	function dump_table(tb, str)
	    if nil == str then str = "" end
	    for k, v in pairs(tb)
	    do
	        print(str, k, v)
	    end
	end
	
	-- 测试调用lua
	function test_func(arg1, arg2, arg3, arg4)
	    print("in test_func:", arg1, arg2, arg3, arg4)
	    mp = {["k"] = "v"}
	    vc = {1,2,3}
	    lt = {4,5,6}
	    st = {7,8,9}
	    dumy(mp, vc, lt, st)
	end
	
	-- 接受stl参数
	function test_stl(vec, lt, st, mp)
	    print("--------------dump_table begin ----------------")
	    dump_table(vec, "vec")
	    dump_table(lt, "lt")
	    dump_table(st, "st")
	    dump_table(mp, "mp")
	    print("--------------dump_table end ----------------")
	    return "ok"
	end
	
	-- 返回stl 参数
	function test_return_stl_vector()
	    return {1,2,3,4}
	end
	function test_return_stl_list()
	    return {1,2,3,4}
	end
	function test_return_stl_set()
	    return {1,2,3,4}
	end
	function test_return_stl_map()
	    return {
	        ["key"] = 124
	    }
	end
	-- 测试接受C++对象
	function test_object(foo_obj)
	    --测试构造
	    base = base_t:new()
	    -- 每个对象都有一个get_pointer获取指针
	    print("base ptr:", base:get_pointer())
	    -- 测试C++对象函数
	    foo_obj:print(12333, base)
	    base:delete()
	    --基类的函数
	    foo_obj:dump()
	    -- 测试C++ 对象属性
	    print("foo property", foo_obj.a)
	    print("base property", foo_obj.v)
	end
	
	-- 测试返回C++对象
	function test_ret_object(foo_obj)
	    return foo_obj
	end
	
	-- 测试返回C++对象
	function test_ret_base_object(foo_obj)
	    return foo_obj
	end
	
	    
	clazz:static_func()

编译运行：

	> git clone https://github.com/fanchy/fflua
	> cd fflua/example/
	> make
	> ./app_lua 

运行结果：
	
	in clazz::static_func end ------------
	exe run_string!!
	in test_func:   1.0     2.0     3.0     4
	in dumy begin ------------
	map:k, val:v:
	in dumy end ------------
	--------------dump_table begin ----------------
	vec     1       100.0
	lt      1       99.98999786377
	st      1       OhNIce
	mp      key     200.0
	--------------dump_table end ----------------
	in foo_t b:456 this=0x82cf468
	base ptr:       137158880
	in foo_t::print a:12333 p:0x82ce0e0
	in dump a:789
	foo property    456.0
	base property   789.0


## 6.10 lua cjson库##

json官网列出来很多开源的[lua json库](http://lua-users.org/wiki/JsonModules)，用于lua table对象和json数据格式的相互转换。这里简单介绍cjson库的使用。

**获取代码并运行测试实例**
	
	> git clone https://github.com/peimin/lua_cjson
	> cd lua_cjson/
	> make
	> lua json_test.lua 

运行结果：

	usr_encode: {"name":"name5566boy","usr_xx":{"2":33,"3":44,"1":22},"age":72}
	name: name5566boy
	age : 72.0
	usr_xx...
	2       33.0
	3       44.0
	1       22.0

**json.lua**

对cjson接口的简单封装，以保护模式调用cjson提供的json解包 压包函数。

	local json = {}
	local cjson = require("cjson")
	
	function json.encode(var)
	    local status, result = pcall(cjson.encode, var)
	    if status then 
			return result 
		end
	end
	
	function json.decode(text)
	    local status, result = pcall(cjson.decode, text)
	    if status then 
			return result 
		end
	end
	
	return json

**json_test.lua**

	local json = require "json"
	
	local usr_xx ={}
	usr_xx["1"] = 22
	usr_xx["2"] = 33
	usr_xx["3"] = 44
	
	local usr = {}
	usr["name"]   = "name5566boy"
	usr["age"]    = 72
	usr["usr_xx"] = usr_xx
	
	local usr_encode = json.encode(usr)     -- 将lua table对象转换成json字符串
	if usr_encode then
		print("usr_encode: "..usr_encode)   -- {"usr_xx":{"2":33,"3":44,"1":22},"age":72,"name":"name5566boy"}
	end
	
	local usr_msg = json.decode(usr_encode) -- 将json字符串转换成lua table对象 
	if usr_msg then
		print("name: "..usr_msg["name"])
		print("age : "..usr_msg["age"])
		local usr_xx = usr_msg["usr_xx"]
	
		print("usr_xx...")
		for k, v in pairs (usr_xx) do
		  print (k, v)
		end 
	end

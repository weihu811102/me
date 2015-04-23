---------------------------------------------------------------------------------------------------------------
base_type=class()       -- 定义一个基类 base_type

function base_type:ctor(x)  -- 定义 base_type 的构造函数
    print("base_type ctor")
    self.x=x
end

function base_type:print_x()    -- 定义一个成员函数 base_type:print_x
    print(self.x)
end

function base_type:hello()  -- 定义另一个成员函数 base_type:hello
    print("hello base_type")
end

以上是基本的 class 定义的语法，完全兼容 lua 的编程习惯。我增加了一个叫做 ctor 的词，作为构造函数的名字。

 test=class(basetype) -- 定义一个类 test 继承于 basetype

function test:ctor()    -- 定义 test 的构造函数
    print("test ctor")
end

function test:hello()   -- 重载 base_type:hello 为 test:hello
    print("hello test")
end
 

a=test.new(1)   -- 输出两行，base_type ctor 和 test ctor 。这个对象被正确的构造了。
a:print_x() -- 输出 1 ，这个是基类 base_type 中的成员函数。
a:hello()   -- 输出 hello test ，这个函数被重载了。

Account = {balance = 0}
function Account.withDraw(self, v)
     self.balance = self.balance - v
end

--------------------------------------------------------------------------------------------------------------------------------
a = Account
Account = nil
a.withDraw(a, 100)
print(a.balance)
这样再调用，就不会出现错误了。

使用self参数是所有面向对象语言的一个核心。大多数面向对象语言都对程序员隐藏了self参数，从而使得程序员不必显示地声明这个参数。Lua也可以，当我们在定义函数时，使用了冒号，则能隐藏该参数，那么上述代码使用冒号来改下，就是下面这个样子了。

Account = {balance = 0}
function Account:withDraw(v) -- 注意这里的冒号":"
     self.balance = self.balance - v
end

a = Account
Account = nil
a:withDraw(100) -- 注意这里的调用时，也需要冒号":"
print(a.balance)
-------------------------------------------------------------------------------------------------------------------------

Lua支持goto语法, 但是有一定的局限性.
例如
1. 不能在block外面跳入block(因为block中的lable不可见), 
2. 不能跳出或者跳入一个函数. 
3. 不能跳入本地变量的作用域.


goto lable ::

首先,先来一段在lua创建一个类与对象的代码

 
Class = {}
Class.__index = Class
 
function Class:new(x,y)
    local temp = {}
    setmetatable(temp, Class)
    temp.x = x
    temp.y = y
    return temp
end
 
function Class:test()
    print(self.x,self.y)
end
 
 
object = Class.new(10,20)
 
object:test()
猜一下会输出什么结果呢？

输出：

>lua -e "io.stdout:setvbuf 'no'" "object.lua" 
20    nil
>Exit code: 0

我们的y值怎么没了?

这个原因很简单,因为我们创建一个对象的时候使用了一个 . 号

在lua程序设计第二版中,有提到当一项操作所作用的”接受者”,需要一个额外的参数来表示该接受者,这个参数通常称为self或this

然后我们在这段代码加上 self
 
Class = {}
Class.__index = Class
 
function Class:new(x,y)
    local temp = {}
    setmetatable(temp, Class)
    temp.x = x
    temp.y = y
    return temp
end
 
function Class:test()
    print(self.x,self.y)
end
 
 
object = Class.new(self,10,20)
 
object:test()
然后我们在看一下输出

>lua -e "io.stdout:setvbuf 'no'" "object.lua" 
10    20
>Exit code: 0

这下就正常了!!嗯,每次创建一个对象的时候都有写一个self,会不会感觉很麻烦呢?lua提供了用冒号的方式在一个方法定义中添加一个额外的参数,以及在一个方法调用中添加一个额外的实参

然后代码改成

  
Class = {}
Class.__index = Class
 
function Class:new(x,y)
    local temp = {}
    setmetatable(temp, Class)
    temp.x = x
    temp.y = y
    return temp
end
 
function Class:test()
    print(self.x,self.y)
end
 
 
object = Class:new(10,20)
 
object:test()
输出正常：

>lua -e "io.stdout:setvbuf 'no'" "object.lua" 
10    20
>Exit code: 0

如果，就这么完的话,本来是一件很欢乐的事情,但是,我尝试了一下以下代码

 
Class = {}
Class.__index = Class
 
function Class.new(x,y)
    local temp = {}
    setmetatable(temp, Class)
    temp.x = x
    temp.y = y
    return temp
end
 
function Class:test()
    print(self.x,self.y)
end
 
 
object = Class.new(10,20)
 
object:test()
出乎意料的是：

>lua -e "io.stdout:setvbuf 'no'" "object.lua" 
10    20
>Exit code: 0

代码正常运行….这个让人很费解,本来,点号对方法的操作是需要一个额外的接受者,第一段代码已经说明了这个问题,但是,现在程序有正常运行,令我真是有点费解…

然后,我接着尝试又发现
 
Class = {}
Class.__index = Class
 
function Class.new(x,y)
    local temp = {}
    setmetatable(temp, Class)
    temp.x = x
    temp.y = y
    return temp
end
 
function Class:test()
    print(self.x,self.y)
end
 
 
object = Class:new(10,20)
 
object:test()
输出结果：

>lua -e "io.stdout:setvbuf 'no'" "object.lua" 
table: 003CACA0    10
>Exit code: 0

这个只不过跟第一段代码点号和冒号的位置调换了一下,就出现了这样的结果…

如果,你仔细想想,这里和第一段代码的区别,可以发现,其实,这里就可以证明了冒号其实就是默认传了一个实参到方法中

为了证明冒号的作用,我改动了一下代码

 
Class = {}
Class.__index = Class
 
function Class:new(x,y)
    local temp = {}
    setmetatable(temp, Class)
    temp.x = x
    temp.y = y
    return temp
end
 
function Class.test()
    print(self.x,self.y)
end
 
 
object = Class:new(10,20)
 
object:test()
输出的结果是：

>lua -e "io.stdout:setvbuf 'no'" "object.lua" 
lua: object.lua:15: attempt to index global 'self' (a nil value)
stack traceback:
    object.lua:15: in function 'test'
    object.lua:21: in main chunk
    [C]: ?
>Exit code: 1

从这里的错误可以看出,没有self这个参数,竟然,方法用的是点号,那我们试一下把对象传进去看下能不能正常运行

 
Class = {}
Class.__index = Class
 
function Class:new(x,y)
    local temp = {}
    setmetatable(temp, Class)
    temp.x = x
    temp.y = y
    return temp
end
 
function Class.test()
    print(self.x,self.y)
end
 
 
object = Class:new(10,20)
 
object:test(object)
遗憾的是这样的改动是错误的,错误的结果也是一样的

>lua -e "io.stdout:setvbuf 'no'" "object.lua" 
lua: object.lua:15: attempt to index global 'self' (a nil value)
stack traceback:
    object.lua:15: in function 'test'
    object.lua:21: in main chunk
    [C]: ?
>Exit code: 1

那我们这次尝试下想刚才那样,把方法的点号搞成一致看下效果怎样

 
Class = {}
Class.__index = Class
 
function Class:new(x,y)
    local temp = {}
    setmetatable(temp, Class)
    temp.x = x
    temp.y = y
    return temp
end
 
function Class.test()
    print(self.x,self.y)
end
 
 
object = Class:new(10,20)
 
object.test()
遗憾的是跟之前不一样，还是不能运行

 
Class = {}
Class.__index = Class
 
function Class:new(x,y)
    local temp = {}
    setmetatable(temp, Class)
    temp.x = x
    temp.y = y
    return temp
end
 
function Class.test()
    print(self.x,self.y)
end
 
 
object = Class:new(10,20)
 
object.test()
1
  
1
回想一下，冒号的作用可以传递一个实参,对于方法的操作我们需要一个接受者,那么进行以下的改动
 
Class = {}
Class.__index = Class
 
function Class:new(x,y)
    local temp = {}
    setmetatable(temp, Class)
    temp.x = x
    temp.y = y
    return temp
end
 
function Class:test()
    print(self.x,self.y)
end
 
 
object = Class:new(10,20)
 
object.test(object)
这次输出就正常了

>lua -e "io.stdout:setvbuf 'no'" "object.lua" 
10    20
>Exit code: 0

这段代码告诉了我们，想要操作一个方法就一定需要一个额外参数来表示该值，对于点号，我们必须显示传递一个实参，才能使程序正常运行，而为了方便，我们可以直接使用冒号来简化操作．

结论:

罗嗦了半天其实,可以用一下一句话来避免这个问题:

用lua进行面向对象的编程,声明方法和调用方法统一用冒号,对于属性的调用全部用点号


## 模式匹配 ##
模式匹配是类似switch-case特性，但更加灵活；也类似if-else，但更加简约。

这个例子展示的使用用模式匹配实现斐波那契。 使用**case**来匹配参数，如果case _，则可以匹配任何参数。
```scala
试着将

case n: Int
修改为

case n: Int if (n > 1)
```

case后添加if语句判断,这样修改当输入负数时，就不会无限循环。

模式匹配也可以匹配类型，在**case _ **前加上
```scala
case n: String => fibonacci(n.toInt)
```
这样就可以匹配String类型

```scala
def fibonacci(in: Any): Int = in match {
    case 0 => 0
    case 1 => 1
    case n: Int if (n > 1) => fibonacci(n - 1) + fibonacci(n - 2)
    case n: String => fibonacci(n.toInt)
    case _ => 0
}

println(fibonacci(3))
println(fibonacci(-3))
println(fibonacci("3"))
```

## Case Class ##
case class 顾名思义就是为case语句专门设计的类， 在普通类的基础上添加了和类名一致的工厂方法， 还添加了hashcode,equals和toString等方法。

由于使用了require(n >= 0)来检验参数，如果使用负数计算，将会抛出异常。

```scala
abstract class Expr

case class FibonacciExpr(n: Int) extends Expr {
  require(n >= 0)
}

case class SumExpr(a: Expr, b: Expr) extends Expr

def value(in: Expr): Int = in match {
  case FibonacciExpr(0) => 0
  case FibonacciExpr(1) => 1
  case FibonacciExpr(n) => 
    value(SumExpr(FibonacciExpr(n - 1), FibonacciExpr(n - 2)))
  case SumExpr(a, b) => value(a) + value(b)
  case _ => 0
}
println(value(FibonacciExpr(3)))
```

## 函数式的威力 ##
这个例子是用指令式编程判断一个List中是否含有奇数。

试着将
```scala
containsOdd(list)
替换为

list.exists((x: Int) => x % 2 ==1)
通过将函数作为参数，可以使程序更简洁。还可以再简化为

list.exists(_ % 2 == 1)
可以用_替代参数
```

相比于Ruby等动态语言,这威力来自于科学而不是魔法

```scala
val list = List(1, 2, 3, 4)

def containsOdd(list: List[Int]): Boolean = {
  for (i <- list) {
    if (i % 2 == 1)
      return true;
  }
  return false;
}

println("list contains Odd ? " + containsOdd(list))
```

```scala
val list = List(1, 2, 3, 4)

println("list contains Odd ? " + list.exists((x: Int) => x % 2 ==1))
```

```scala
val list = List(1, 2, 3, 4)

println("list contains Odd ? " + list.exists(_ % 2 == 1))
```
## 函数式真正的威力 ##
函数式除了能简化代码外，更重要的是他关注的是Input和Output，函数本身没有副作用。

就是Unix pipeline一样，简单的命令组合在一起威力无穷。

如果你喜欢Unix pipeline的方式，你一定也会喜欢函数式编程。

这个例子是用函数式的代码模拟

cat file | grep 'warn' | grep '2013' | wc
List的filter方法接受一个过滤函数，返回一个新的List，作为下一个方法的输入。

```scala
val file = List("warn 2013 msg", "warn 2012 msg",
  "error 2013 msg", "warn 2013 msg")

println("cat file | grep 'warn' | grep '2013' | wc : " 
    + file.filter(_.contains("warn")).filter(_.contains("2013")).size)

cat file | grep 'warn' | grep '2013' | wc : 2
```

## Word Count ##
Word Count是一个MapReduce的一个经典示例。在函数式编程中，Word Count最直观的实现方法也是MapReduce。

这个例子介绍了List的两个重要的高阶方法map和reduceLeft。

map接受一个转换函数,返回转换结果。

reduceLeft接受一个合并函数，依次遍历合并。

使用高阶方法可以代替大部分需要循环的操作，使代码更清晰

```scala
val file = List("warn 2013 msg", "warn 2012 msg",
  "error 2013 msg", "warn 2013 msg")

def wordcount(str: String): Int = str.split(" ").count("msg" == _)
  
val num = file.map(wordcount).reduceLeft(_ + _)

println("wordcount:" + num)

wordcount:4
```

## 尾递归 ##
尾递归是递归的一种，特点在于会在函数的最末调用自身。尾递归是函数式编程的常见写法。

这个例子是foldLeft的尾递归实现。foldLeft和reduceLeft相比更常用，多一个初始参数。

当用List做match case时。可以使用 :: 来解构。返回第一个元素head和剩余元素tail。

注：尾递归会在编译期优化，因此不用担心递归造成的栈溢出问题。

```scala
val file = List("warn 2013 msg", "warn 2012 msg",
  "error 2013 msg", "warn 2013 msg")

def wordcount(str: String): Int = str.split(" ").count("msg" == _)

def foldLeft(list: List[Int])(init: Int)(f: (Int, Int) => Int): Int = {
  list match {
    case List() => init
    case head :: tail => foldLeft(tail)(f(init, head))(f)
  }
}

val num = foldLeft(file.map(wordcount))(0)(_ + _)

println("wordcount:" + num)

wordcount:4
```

## 更强大的For循环 ##
循环语句是指令式编程的常见语句，Scala对其加以改进，成为适应函数式风格的利器。

For循环也是有返回值的，返回的是一个List。在每一轮迭代中加入yield，yield后的值可以加入到List中。

这个例子是使用for循环代替map函数。

```scala
val file = List("warn 2013 msg", "warn 2012 msg",
  "error 2013 msg", "warn 2013 msg")

def wordcount(str: String): Int = str.split(" ").count("msg" == _)

val counts =
  for (line <- file)
    yield wordcount(line)

val num = counts.reduceLeft(_ + _)

println("wordcount:" + num)

// wordcount:4
```

## Option ##
Scala提供了Option机制来解决，代码中不断检查null的问题。

这个例子包装了getProperty方法，使其返回一个Option。 这样就可以不再漫无目的地null检查。只要Option类型的值即可。

使用pattern match来检查是常见做法。也可以使用getOrElse来提供当为None时的默认值。

给力的是Option还可以看作是最大长度为1的List，List的强大功能都可以使用。

再见 NullException
```scala
def getProperty(name: String): Option[String] = {
  val value = System.getProperty(name)
  if (value != null) Some(value) else None
}

val osName = getProperty("os.name")

osName match {
  case Some(value) => println(value)
  case _ => println("none")
}

println(osName.getOrElse("none"))

osName.foreach(print _)

// Linux
// Linux
// Linux
```

## Lazy初始化 ##
Lazy可以延迟初始化字段。加上lazy的字段会在第一次访问的时候初始化，而不是类初始化的时候初始化。

这个例子是从github获得Scala的版本号，由于访问网络需要较多时间。可以使用lazy来延迟获取。 防止可能的浪费。

Lazy非常适合于初始化非常耗时的场景

```scala
class ScalaCurrentVersion(val url: String) {
  lazy val source= {
    println("fetching from url...")
    scala.io.Source.fromURL(url).getLines().toList
  }
  lazy val majorVersion = source.find(_.contains("version.major"))
  lazy val minorVersion = source.find(_.contains("version.minor"))
}

val version = new ScalaCurrentVersion(
  "https://raw.github.com/scala/scala/master/build.number")
println("get scala version from " + version.url)
version.majorVersion.foreach(println _)
version.minorVersion.foreach(println _)

// get scala version from https://raw.github.com/scala/scala/master/build.number
// fetching from url...
// exit value is 143
```
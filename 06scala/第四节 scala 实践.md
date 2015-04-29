## 4.1 使用Java ##

Scala和Java可以非常方便的互操作，前面已经有大量Scala直接使用Java的例子。

同样Java也可以使用Scala。这个例子演示使用@BeanProperty注解来生成Java Style的Bean。

尝试将
```scala
var name: String
修改为

@BeanProperty var name: String
```

这样就给bean添加了getter/setter。 Apache BeanUtils就可以正常工作。

```scala

import org.apache.commons.beanutils.BeanUtils
import scala.beans.BeanProperty

class SimpleBean(var name: String) {
}

val bean = new SimpleBean("foo")

println(BeanUtils.describe(bean))

```

## 4.2 相等性 ##
在Scala中==等效于equals，这一点和Java不同。更自然一些。

写一个完全正确的equal函数并不容易，这个例子也有子类会不对称的Bug。

尝试将class修改为case class,并删除equals函数。

case类会自动生成正确的equals函数。

```scala

class Person(val name: String) {
  override def equals(other: Any) = other match {
    case that: Person => name.equals(that.name)
    case _ => false
  }
}

println(new Person("Black") == new Person("Black"))

```
## 4.3 抽取器 ##

抽取器可以帮助模式匹配进行解构。

这个例子是构建一个Email抽取器，只要实现unapply函数就可以了。

Scala的正则表达式会自带抽取器，可以抽取出一个List。List里的元素是匹配()里的表达式。

抽取器很有用，短短的例子里就有两处使用抽取器：

```scala

case user :: domain :: Nil
解构List

case Email(user, domain)
解构Email。

```

```scala

import scala.util.matching.Regex

object Email {
  def unapply(str: String) = new Regex("""(.*)@(.*)""")
    .unapplySeq(str).get match {
    case user :: domain :: Nil => Some(user, domain)
    case _ => None
  }
}

"user@domain.com" match {
  case Email(user, domain) => println(user + "@" + domain)
}

```

## 4.4 记忆模式 ##

记忆模式可以解决手动编写存取cache代码的麻烦。

这个例子中，memo可以将一个不含cache函数，包装成一个含有cache功能的。

还是斐波那契的例子，通过cache可以使性能提高。

尝试将
```scala
fibonacci_(n - 1) + fibonacci_(n - 2)
修改为

memo(fibonacci_)(n - 1) + memo(fibonacci_)(n - 2)
```

可以提高更多性能。

```scala

import scala.collection.mutable.WeakHashMap

val cache = new WeakHashMap[Int, Int]
def memo(f: Int => Int) = (x: Int) => cache.getOrElseUpdate(x, f(x))

def fibonacci_(in: Int): Int = in match {
  case 0 => 0;
  case 1 => 1;
  case n: Int => fibonacci_(n - 1) + fibonacci_(n - 2)
}
val fibonacci: Int => Int = memo(fibonacci_)

val t1 = System.currentTimeMillis()
println(fibonacci(40))
println("it takes " + (System.currentTimeMillis() - t1) + "ms")

val t2 = System.currentTimeMillis()
println(fibonacci(40))
println("it takes " + (System.currentTimeMillis() - t2) + "ms")

```

运行结果：

```scala

102334155
it takes 1198ms
102334155
it takes 0ms

```

## 4.5隐式转换 ##

implicit可以定义一个转换函数，可以在使用相应类型的时候自动转换。

这个例子可以将String自动转换为Date类型。隐式转换时实现DSL的重要工具。

```scala

import java.text.SimpleDateFormat

implicit def strToDate(str: String) = 
  new SimpleDateFormat("yyyy-MM-dd").parse(str)

println("2013-01-01 unix time: " + "2013-01-01".getTime()/1000l)

// 2013-01-01 unix time: 1356998400

```
## 4.6 DSL ##

DSL是Scala最强大武器，可以使一些描述性代码变得极为简单。

这个例子是使用DSL生成JSON。Scala很多看似是语言级的特性也是用DSL做到的。

自己编写DSL有点复杂，但使用起来非常方便。

```scala 

import org.json4s._
import org.json4s.JsonDSL._
import org.json4s.jackson.JsonMethods._
import java.util.Date

case class Twitter(id: Long, text: String, publishedAt: Option[java.util.Date])

var twitters = Twitter(1, "hello scala", Some(new Date())) ::
  Twitter(2, "I like scala tour", None) :: Nil

var json = ("twitters"
  -> twitters.map(
    t => ("id" -> t.id)
      ~ ("text" -> t.text)
      ~ ("published_at" -> t.publishedAt.toString())))

println(pretty(render(json)))

```
运行结果：

```scala

{
  "twitters" : [ {
    "id" : 1,
    "text" : "hello scala",
    "published_at" : "Some(Tue Apr 28 07:16:09 UTC 2015)"
  }, {
    "id" : 2,
    "text" : "I like scala tour",
    "published_at" : "None"
  } ]
}

```

## 4.7 测试 ##

Scala可以使用Spec2，ScalaTest来测试， DSL可以使测试更方便。

这个例子是测试一个阶乘函数。使用should/in来建立测试用例。

测试是默认并发执行的。

```Scala 

import org.specs2.mutable._

class FactorialSpec extends Specification {
  args.report(color = false)

  def factorial(n: Int) = (1 to n).reduce(_ * _)

  "The 'Hello world' string" should {
    "factorial 3 must be 6" in {
      factorial(3) mustEqual 6
    }
    "factorial 4 must greaterThan 6" in {
      factorial(4) must greaterThan(6)
    } 
  }
}
specs2.run(new FactorialSpec)

// exit value is 143

```








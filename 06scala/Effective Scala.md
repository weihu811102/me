Effective Scala
--
Marius Eriksen, Twitter Inc.
marius@twitter.com (@marius)

[translated by hongjiang(@hongjiang), tongqing(@tongqing)]

**Table of Contents**

- 序言
- 格式化: 空格, 命名, Imports, 花括号, 模式匹配, 注释
- 类型和泛型: 返回类型注解(annotation), 变型, 类型别名, 隐式转换
- 集合: 层级, 集合的使用, 风格, 性能, Java集合
- 并发: Future, 集合
- 控制结构: 递归, 返回(Return), for循环和for推导, 要求require和断言(assert)
- 函数式编程: Case类模拟代数数据类型, Options, 模式匹配, 偏函数, 解构绑定, 惰性赋值, 传名调用, flatMap
- 面向对象的编程: 依赖注入, Trait, 可见性, 结构类型
- 垃圾回收
- Java 兼容性
- Twitter标准库: Future, Offer/Broker


**序言**

Scala是Twitter使用的主要应用编程语言之一。很多我们的基础架构都是用scala写的，我们也有一些大的库支持我们使用。虽然非常有效, Scala也是一门大的语言,经验教会我们在实践中要非常小心。 它有什么陷阱？哪些特性我们应该拥抱，哪些应该避开？我们什么时候采用“纯函数式风格”，什么时候应该避免？换句话说：哪些是我们发现的，可以高效地使用这门语言的地方？本指南试图把我们的经验提炼成短文，提供一系列最佳实践。我们使用scala主要创建一些大容量分布式系统服务——我们的建议也偏向于此——但这里的大多建议也应该自然的适用其他系统。这不是定律，但不当的使用应该被调整。

Scala提供很多工具使表达式可以很简洁。敲的少读的就少，读的少就能更快的读，因此简洁增强了代码的清晰。然而简洁也是一把钝器(blunt tool)也可能起到相反的效果：在考虑正确性之后，也要为读者着想。

首先，用Scala编程，你不是在写Java，Haskell或Python；Scala程序不像这其中的任何一种。为了高效的使用语言，你必须用其术语表达你的问题。 强制把Java程序转成Scala程序是无用的，因为大多数情况下它会不如原来的。

这不是对Scala的一篇介绍，我们假定读者熟悉这门语言。这儿有些学习Scala的资源：

Scala School
Learning Scala
Learning Scala in Small Bites
这是一篇“活的”文档，我们会更新它,以反映我们当前的最佳实践，但核心的思想不太可能会变： 永远重视可读性；写泛化的代码但不要牺牲清晰度； 利用简单的语言特性的威力，但避免晦涩难懂（尤其是类型系统）。最重要的，总要意识到你所做的取舍。一门成熟的(sophisticated)语言需要复杂的实现，复杂性又产生了复杂性：之于推理，之于语义，之于特性之间的交互，以及与你合作者之间的理解。因此复杂性是为成熟所交的税——你必须确保效用超过它的成本。

玩的愉快。


1.格式化
--
代码格式化的规范并不重要，只要它们实用。它的定义形式没有先天的好与坏，几乎每个人都有自己的偏好。然而，对于一贯地采用同一格式化规则的总会增加可读性。已经熟悉某种特定风格的读者不必非要去掌握另一套当地习惯，或译解另一个角落里的语言语法。

这对Scala来说也特别重要，因为它的语法高度的重叠。一个例子是方法调用：方法调用可以用“.”后边跟圆括号，或不使用“.”，后边用空格加不带圆括号(针对空元或一元方法)方式调用。此外，不同风格的方法调用揭露了它们在语法上不同的分歧(ambiguities)。当然一致的应用慎重的选择一组格式化规则，对人和机器来说都会消除大量的歧义。

我们依着Scala style guide 增加了以下规则：


**空格**

用两个空格缩进。避免每行长度超过100列。在两个方法、类、对象定义之间使用一个空白行。


**命名**

- 对作用域较短的变量使用短名字：
- is, js 和 ks等可出现在循环中。
- 对作用域较长的变量使用长名字：
- 外部APIs应该用长的，不需加以说明便可理解的名字。例如：Future.collect 而非 Future.all
- 使用通用的缩写，避开隐秘难懂的缩写：
- 例如每个人都知道 ok,err, defn等缩写的意思，而sfri是不常用的。
- 不要在不同用途时重用同样的名字：
- 使用val(注：Scala中的不可变类型)
- 避免用 `声明保留字变量：
- 用typ替代 `type`
- 用主动语态(active)来命名有副作用的操作：
- user.activate()而非 user.setActive()
- 对有返回值的方法使用具有描述性的名字：
- src.isDefined 而非src.defined
- getters不采用前缀get：
- 用get是多余的: site.count而非site.getCount
- 不必重复已经被package或object封装过的名字：
- 使用：
```scala
object User {
  def get(id: Int): Option[User]
}
而非：
object User {
  def getUser(id: Int): Option[User]
}
```
相比 get 方法 getUser 方法中的User是多余的，并不能提供额外的信息。

**Imports**

- 对引入行按字母顺序排序：
- 这样机方便了视觉上的检查，也简化了自动操作。
- 当从一个包中引入多个时，用花括号：
- import com.twitter.concurrent.{Broker, Offer}
- 当引入超过6个时使用通配符：
- e.g.: import com.twitter.concurrent._ 
- 不要轻率的使用: 一些包导入了太多的名字
- 当引入集合的时候，通过用import scala.collections.immutable(不可变集合)或scala.collections.mutable(可变集合)来限定名称
- 可变和不可变集合有相同的名字。限定名称让读者很明确知道使用的是哪个变量(e.g. "immutable.Map")
- (译注，通常也会默认immutable，而在使用mutable时显示引入)
- 不要使用来自其它包的相对引用：
- 避免
- import com.twitter
- import concurrent
- 而应该用清晰的：
- import com.twitter.concurrent
- (译注，实际上上面的import不能编译通过，第二个import应该为：import twitter.concurrent 即import一个包实际是定义了这个包的别名。)
- 将import放在文件的头部：
- 读者可以在一个地方参考所有的引用。

**花括号**
- 
- 花括号用于创建复合表达式，复合表达式的返回值是最后一个表达式。避免对简单的表达式采用花括号；写成：
- 
-  def square(x: Int) = x*x
- 而不是：
- 
-  def square(x: Int) = {
-    x * x
-  }
- 尽管第二种方式用在区分方法体的语句构成很诱人。第一种选择更加简洁，易读。避免语句上的繁文缛节，除非需要阐明。


**模式匹配**

尽可能直接在函数定义的地方使用模式匹配。例如，下面的写法 match应该被折叠起来(collapse)

```scala
 list map { item =>
   item match {
     case Some(x) => x
     case None => default
   }
 }
用下面的写法替代：

 list map {
   case Some(x) => x
   case None => default
 }
```

它很清晰的表达了 list中的元素都被映射，间接的方式让人不容易明白。


注释
使用ScalaDoc提供API文档。用下面的风格：

 /**
  * ServiceBuilder builds services
  * ...
  */
而不是非标准的ScalaDoc风格：

 /** ServiceBuilder builds services
  * ...
  */
不要诉诸于ASCII码艺术或其他可视化修饰。用文档记录APIs但不要添加不必要的注释。如果你发现你自己添加注释解释你的代码行为，先问问自己是否可以调整代码结构，从而可以明显地可以看出它做了什么。相对于“it works, obviously” 更偏向于“obviously it works”


类型和泛型
类型系统的首要目的是检测程序错误。类型系统有效的提供了一个静态检测的有限形式，允许我们代码中明确某种类型的变量并且编译器可以验证。类型系统当然也提供了其他好处，但错误检测是他存在的理由(Raison d’Être)

我们使用类型系统应当反映这一目标，但我们必须考虑到读者（译注：读你代码的人）：明智地使用类型可以增加清晰度，而过份聪明只会迷乱。

Scala的强大类型系统是学术探索和实践共同来源(例如Type level programming in Scala) 。但这是一个迷人的学术话题，这些技术很少在应用和正式产品代码中使用。它们应该被避免。


返回类型注解(annotation)
尽管Scala允许返回类型是可以省略的，加上它们提供了很好的文档：这对public方法特别重要。而当一个方法不需要对外暴露，并且它的返回值类型是显而易见的时候，则可以直接省略。

在使用混入(mixin)实例化对象时这一点尤其重要，Scala编译器为这些对象创造了单类。例如：

 trait Service
 def make() = new Service {
   def getId = 123
 }
上面的make不需要定义返回类型为Service；编译器会创建一个加工过的类型: Object with Service{def getId:Int}(译注:with是Scala里的mixin的语法)。若用一个显式的注释：

 def make(): Service = new Service{}
现在作者则不必改变make方法的公开类型而随意的混入(mix in) 更多的特质(traits)，使向后兼容很容易实现。


变型
变型(Variance)发生在泛型与子类型化(subtyping)结合的时候。与容器类型的子类型化有关，它们定义了对所包含的类型如何子类型化。因为Scala有声明点变型(declaration site variance)注释（annotation），公共库的作者——特别是集合——必须有丰富的注释器。这些注释对共享代码的可用性很重要，但滥用也会很危险。

不可变(invariants)是Scala类型系统中高级部分，但也是必须的一面，因为它有助于子类型化的应用，应该广泛(并且正确)地使用。

不可变(Immutable)集合应该是协变的(covariant)。接受容器化类型得方法应该适当地降级(downgrade)集合：

 trait Collection[+T] {
   def add[U >: T](other: U): Collection[U]
 }
可变(mutable)集合应该是不可变的(invariant). 协变对于可变集合是典型无效的。考虑：

 trait HashSet[+T] {
   def add[U >: T](item: U)
 }
和下面的类型层级：

 trait Mammal
 trait Dog extends Mammal
 trait Cat extends Mammal
如果我现在有一个狗(dog)的 HashSet：

 val dogs: HashSet[Dog]
把它作为一个哺乳动物的Set，增加一只猫(cat)

 val mammals: HashSet[Mammal] = dogs
 mammals.add(new Cat{})
这将不再是一个只存储狗(dog)的HashSet！


类型别名
类型别名应当在其提供了便捷的命名或阐明意图时使用，但对于自解释（不言自明）的类型不要使用类型别名。比如

 () => Int
比下面定义的别名IntMarker更清晰

 type IntMaker = () => Int
 IntMaker
但，下面的别名:

 class ConcurrentPool[K, V] {
   type Queue = ConcurrentLinkedQueue[V]
   type Map   = ConcurrentHashMap[K, Queue]
   ...
 }
是有用的，因为它表达了目的并更加简短。

当使用类型别名的时候不要使用子类型化(subtyping)

 trait SocketFactory extends (SocketAddress => Socket)
SocketFactory 是一个生产Socket的方法。使用一个类型别名更好：

 type SocketFactory = SocketAddress => Socket
我们现在可以对 SocketFactory类型的值 提供函数字面量(function literals) ,也可以使用函数组合：

 val addrToInet: SocketAddress => Long
 val inetToSocket: Long => Socket

 val factory: SocketFactory = addrToInet andThen inetToSocket
类型别名通过用 package object 将名字绑定在顶层:

 package com.twitter
 package object net {
   type SocketFactory = (SocketAddress) => Socket
 }
注意类型别名不是新类型——他们等价于在语法上用别名代替了原类型。


隐式转换
隐式转换是类型系统里一个强大的功能，但应当谨慎地使用。它们有复杂的解决规则， 使得通过简单的词法检查领会实际发生了什么很困难。在下面的场景使用隐式转换是OK的：

扩展或增加一个Scala风格的集合
适配或扩展一个对象(pimp my library模式）（译注参见：http://www.artima.com/weblogs/viewpost.jsp?thread=179766)
通过提供约束证据来加强类型安全。
提供了类型的证据 (typeclassing，haskell中的概念，指定义一组函数，其实现因所给的数据类型不同而不同)
用于Manifests (注：Manifest[T]包含类型T的运行时信息)
如果你发现自己在用隐式转换，总要问问自己是否不使用这种方式也可以达到目的。

不要使用隐式转换对两个相似的数据类型做自动转换(例如，把list转换为stream);显示地做更好，因为不同类型有不同的语意，读者应该意识到这些含义。 译注： 1）一些单词的意义不同，但翻译为中文时可能用的相似的词语，比如mutable， Immutable 这两个翻译为可变和不可变，它们是指数据的可变与不可变。 variance, invariant 也翻译为 可变和不可变，（variance也翻译为“变型”），它们是指类型的可变与不可变。variance指支持协变或逆变的类型，invariant则相反。


集合
Scala有一个非常通用，丰富，强大，可组合的集合库；集合是高阶的(high level)并暴露了一大套操作方法。很多集合的处理和转换可以被表达的简洁又可读，但不审慎地用它们的功能也会导致相反的结果。每个Scala程序员应该阅读 集合设计文档；通过它可以很好地洞察集合库，并了解设计动机。

总使用最简单的集合来满足你的需求


层级
集合库很大：除了精心设计的层级(Hierarchy)——根是 Traversable[T] —— 大多数集合都有不可变(immutable)和可变(mutable)两种变体。无论其复杂性，下面的图表包含了可变和不可变集合层级的重要差异。



Iterable[T] 是所有可遍历的集合，它提供了迭代的方法(foreach)。Seq[T] 是有序集合，Set[T]是数学上的集合(无序且不重复)，Map[T]是关联数组，也是无序的。


集合的使用
优先使用不可变集合。不可变集合适用于大多数情况，让程序易于理解和推断，因为它们是引用透明的( referentially transparent )因此缺省也是线程安全的。

使用可变集合时，明确地引用可变集合的命名空间。不要用使用import scala.collection.mutable._ 然后引用 Set ，应该用下面的方式替代：

 import scala.collections.mutable
 val set = mutable.Set()
这样就很明确在使用一个可变集合。

使用集合类型缺省的构造函数。每当你需要一个有序的序列(不需要链表语义)，用 Seq() 等诸如此类的方法构造：

 val seq = Seq(1, 2, 3)
 val set = Set(1, 2, 3)
 val map = Map(1 -> "one", 2 -> "two", 3 -> "three")
这种风格从语意上分离了集合与它的实现，让集合库使用更适当的类型：你需要Map，而不是必须一个红黑树(Red-Black Tree，注：红黑树TreeMap是Map的实现者)

此外，默认的构造函数通常使用专有的表达式，例如：Map() 将使用有3个成员的对象(专用的Map3类)来映射3个keys。

上面的推论是：在你自己的方法和构造函数里，适当地接受最宽泛的集合类型。通常可以归结为Iterable, Seq, Set, 或 Map中的一个。如果你的方法需要一个 sequence，使用 Seq[T]，而不是List[T]


风格
函数式编程鼓励使用流水线转换将一个不可变的集合塑造为想要的结果。这常常会有非常简明的方案，但也容易迷糊读者——很难领悟作者的意图，或跟踪所有隐含的中间结果。例如，我们想要从一组语言中汇集不同的程序语言的投票，按照得票的顺序显示(语言，票数)：

 val votes = Seq(("scala", 1), ("java", 4), ("scala", 10), ("scala", 1), ("python", 10))
 val orderedVotes = votes
   .groupBy(_._1)
   .map { case (which, counts) =>
     (which, counts.foldLeft(0)(_ + _._2))
   }.toSeq
   .sortBy(_._2)
   .reverse
上面的代码简洁并且正确，但几乎每个读者都不能理解作者的原本意图。一个策略是声明中间结果和参数：

 val votesByLang = votes groupBy { case (lang, _) => lang }
 val sumByLang = votesByLang map { case (lang, counts) =>
   val countsOnly = counts map { case (_, count) => count }
   (lang, countsOnly.sum)
 }
 val orderedVotes = sumByLang.toSeq
   .sortBy { case (_, count) => count }
   .reverse
代码也同样简洁，但更清晰的表达了转换的发生(通过命名中间值)，和正在操作的数据的结构(通过命名参数)。如果你担心这种风格污染了命名空间，用大括号{}来将表达式分组:

 val orderedVotes = {
   val votesByLang = ...
   ...
 }
## 3.1 使用Actor ##

Actor是Scala的并发模型。在2.10之后的版本中，使用Akka作为其推荐Actor实现。

Actor是类似线程的实体，有一个邮箱。 Actor可以通过system.actorOf来创建,receive获取邮箱消息，！向邮箱发送消息。

这个例子是一个EchoServer，接受信息并打印。

```scala

import akka.actor.{ Actor, ActorSystem, Props }

val system = ActorSystem()

class EchoServer extends Actor {
  def receive = {
    case msg: String => println("echo " + msg)
  }
}

val echoServer = system.actorOf(Props[EchoServer])
echoServer ! "hi"

system.shutdown

```

## 3.2 Actor原理 ##

Actor比线程轻量。在Scala中可以创建数以百万级的Actor。奥秘在于Actor直接可以复用线程。

Actor和线程是不同的抽象，他们的对应关系是由Dispatcher决定的。

这个例子创建4个Actor，每次调用的时候打印自身线程名称。

可以发现Actor和线程之间没有一对一的对应关系。一个Actor可以使用多个线程，一个线程也会被多个Actor复用。

```scala

import akka.actor.{ Actor, Props, ActorSystem }
import akka.testkit.CallingThreadDispatcher

implicit val system = ActorSystem()

class EchoServer(name: String) extends Actor {
  def receive = {
    case msg => println("server" + name + " echo " + msg +
      " by " + Thread.currentThread().getName())
  }
}

val echoServers = (1 to 10).map(x =>
  system.actorOf(Props(new EchoServer(x.toString))
    .withDispatcher(CallingThreadDispatcher.Id)))
(1 to 10).foreach(msg =>
  echoServers(scala.util.Random.nextInt(10)) ! msg.toString)

system.shutdown

```

## 3.3 同步返回 ##

Actor非常适合于较耗时的操作。比如获取网络资源。

这个例子通过调用ask函数来获取一个Future。

在Actor内部通过 sender ! 传递结果。

Future像Option一样有很多高阶方法，可以使用foreach查看结果。

```scala

import akka.actor.ActorDSL._
import akka.pattern.ask

implicit val ec = scala.concurrent.ExecutionContext.Implicits.global
implicit val system = akka.actor.ActorSystem()

val versionUrl = "https://raw.github.com/scala/scala/master/starr.number"

val fromURL = actor(new Act {
  become {
    case url: String => sender ! scala.io.Source.fromURL(url)
      .getLines().mkString("\n")
  }
})

val version = fromURL.ask(versionUrl)(akka.util.Timeout(5 * 1000))
version.foreach(println _)
  
system.shutdown

```

## 3.4 异步返回 ##

异步操作可以最大发挥效能。Scala的Futrue很强大，可以异步返回。

可以实现Futrue的onComplete方法。当Futrue结束的时候就会回调。

在调用ask的时候，可以设定超时。

```scala

import akka.actor.ActorDSL._
import akka.pattern.ask

implicit val ec = scala.concurrent.ExecutionContext.Implicits.global
implicit val system = akka.actor.ActorSystem()

val versionUrl = "https://raw.github.com/scala/scala/master/starr.number"

val fromURL = actor(new Act {
  become {
    case url: String => sender ! scala.io.Source.fromURL(url)
      .getLines().mkString("\n")
  }
})

val version = fromURL.ask(versionUrl)(akka.util.Timeout(5 * 1000))
version onComplete {
  case msg => println(msg); system.shutdown
}

```

## 3.5 并行集合 ##

这个例子是访问若干URL，并记录时间。如果能并行访问，就可以大幅提高性能。

尝试将

```scala
urls.map
修改为

urls.par.map
```

这样每个map中的函数都可以并发执行。

当函数式和并发结合，就会这样让人兴奋。

```scala

val urls = List("http://scala-lang.org",
  "https://github.com/yankay/scala-tour")

def fromURL(url: String) = scala.io.Source.fromURL(url)
  .getLines().mkString("\n")

val t = System.currentTimeMillis()
urls.map(fromURL(_))
println("time: " + (System.currentTimeMillis - t) + "ms")

```

## 3.6 并行wordcount ##

这个例子是访问若干URL，并记录时间。

并行集合支持大部分集合的功能。

在前面有一个wordcount例子，也可以用并行集合加以实现。

不增加程序复杂性，却能大幅提高利用多核的能力。

```scala

val file = List("warn 2013 msg", "warn 2012 msg",
  "error 2013 msg", "warn 2013 msg")

def wordcount(str: String): Int = str.split(" ").count("msg" == _)

val num = file.par.map(wordcount).par.reduceLeft(_ + _)

println("wordcount:" + num)


```

## 3.7 远程Actor ##

Actor是并发模型，也使用于分布式。

这个例子创建一个Echo服务器，通过actorOf来注册自己。

然后再创建一个client，通过akka url来寻址。

除了是通过url创建的，其他使用的方法和普通Actor一样。

```scala

import akka.actor.{ Actor, ActorSystem, Props }
import com.typesafe.config.ConfigFactory

implicit val system = akka.actor.ActorSystem("RemoteSystem",
  ConfigFactory.load.getConfig("remote"))
class EchoServer extends Actor {
  def receive = {
    case msg: String => println("echo " + msg)
  }
}

val server = system.actorOf(Props[EchoServer], name = "echoServer")

val echoClient = system
  .actorFor("akka://RemoteSystem@127.0.0.1:2552/user/echoServer")
echoClient ! "Hi Remote"

system.shutdown


```




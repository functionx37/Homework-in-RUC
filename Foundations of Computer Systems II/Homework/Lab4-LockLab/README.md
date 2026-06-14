# Lock Lab：真实开源项目中的并发和锁

从社会学的角度看，互联网可能是一个消弭地域边界的数字地球；从经济学的角度看，它是一个颠覆传统生产关系的庞大市场；从大众消费者的角度看，它是一个触手可及的便利生活圈。但从技术的角度来看，互联网是一个充满高并发的世界：从每天清晨数千万人同时边看微信、刷抖音边搭乘地铁，到午间美团、饿了么系统在秒级内为全国骑手派发千万级的外卖订单；从淘宝、京东“双十一”零点瞬间爆发的亿级商品库存扣减，到支付宝和微信支付每秒数千万次的支付事务——在这些人们熟知的互联网服务背后，是无数由全球顶尖大厂深度使用、贡献代码甚至自主研发的硬核高并发与分布式开源项目在默默支撑：

从 Apache Hadoop、Spark 等在成千上万台服务器间调度海量数据的经典大数据基石，到基于 Google Spanner 论文演进出的 TiDB、OceanBase 以及作为现代存储底座的 Meta RocksDB 组成的分布式数据库生态；从 Meta 打造的 C++ 效率图腾 Folly 库、Google 深度主导的 Go 语言运行时（Go Runtime）调度器，到字节跳动在网络层追求极致吞吐的 CloudWeGo/Kitex 框架；从 Linux 内核到整个 KVM 虚拟化技术栈中 Google、红帽、阿里、腾讯持续重构的线程调度与内存管理系统；以及数不清的各种互联网服务的闭源业务代码——在所有这一切支撑起整个现代互联网世界的项目和代码中，并发，始终处于其技术设计中的核心位置。在看不见的幕后，数以亿计的请求在微秒级别内穿梭于分布式集群中，无数的线程、进程在争夺着有限的 CPU、内存和网络资源。在这个高并发的世界里，锁无处不在。

然而，这幅由无数天才工程师用智慧编织出的高并发长卷，并非完美无瑕。即便 Google 的顶尖架构师，也常常在复杂的线程交互、条件变量唤醒与锁的争夺中马失前蹄。那些隐藏极深的并发缺陷，往往在平时的本地测试中静若处子，却偏偏在生产环境遭遇流量洪峰的瞬间暴虐如兽，引发死锁卡死、数据竞争或性能雪崩。即便在今天这个 AI 时代，并发与锁依然是吞噬系统稳定性的头号杀手：AI 可以生成复杂的业务增删改查，但 AI 在编写高并发系统时，同样会继承人类对时间切片和竞态条件的盲区。更糟糕的是，AI 辅助下疯狂膨胀的代码量，反而给“幽灵 Bug”提供了更多隐藏的角落。

而本实验（Lock Lab），就是要带你借助 AI 的力量，正面围剿这些“幽灵”。 在过去，让一个本科生去读懂工业级源码，去调试连高级工程师都头疼的死锁，几乎是一件不可能完成的任务。但在今天，AI 成了最强大的外脑与倍增器。你将不再满足于教科书上那些玩具式的并发模型，而是深入到上述真实、复杂的工业级开源项目中。在这个过程中，你将以极高的效率真正体会并发编程的精妙与残酷，看到真实世界中锁的错误使用会导致哪些 Bug。

## 一、任务整体说明

本实验给你 3 个真实开源项目里的真实锁 Bug（zap、Lucene、Netty）。对每一个 Bug，你要借助 AI 完成三件事：

1. 定位：在该项目的 git 历史里找到修复这个 Bug 的 commit，以及它的父提交（`<fix>^`，即“还没修复”的那一版）。
2. 复现：用一个回归测试（regression test）在这两个 commit 上各跑一次，亲眼看到——父提交上 Bug 存在（测试失败 / 卡死 / 超时），修复提交上 Bug 消失（测试通过）。
3. 理解：借助 AI 阅读源码，搞懂这个 Bug 到底是怎么回事——哪把锁用错了、两个执行体如何互相卡住或错过信号、修复又是如何解决的。

**除三个用户态 Bug 外，本实验还包含一个 Linux 内核锁 Bug 自动检测机制的调研任务（见第四节）。**

## 二、背景知识：三个项目及其对应功能的详细介绍

你可能完全没听说过这三个项目。这一节先帮你建立背景——它们是什么、在工业界扮演什么角色、以及和 Bug 相关的那块功能是干嘛用的。

### 实验一 · uber-go/zap（Go）

- 仓库：<https://github.com/uber-go/zap>
- 工具链：Go

zap 是 Uber 开源的 Go 高性能结构化日志库，在 Go 后端和云原生生态里被极广泛地使用。“结构化日志”指日志以键值字段（而非一行纯文本）记录，便于机器检索；zap 以极低的内存分配开销著称。

zap 里有一个“带缓冲的写入器”——`BufferedWriteSyncer`（在 `zapcore` 包）：它先把日志攒在内存缓冲区，再由一个后台协程按固定时间间隔周期性地刷盘（flush）到真正的目的地（文件 / 网络），以减少系统调用、提升吞吐。整个写入器用一把互斥锁 `sync.Mutex` 来保护内部状态。

### 实验二 · Apache Lucene（Java 8 + Apache Ant）

- 仓库：<https://github.com/apache/lucene-solr>
- 工具链：Java 8 + Apache Ant。这个旧版本必须用 JDK 8（更高版本编译会失败）。

Lucene 是世界上使用最广的开源全文搜索引擎库（Java），著名的 Elasticsearch、Apache Solr 都建立在它之上。它把文档建成索引，从而能极快地回答搜索查询。（历史背景：这个 Bug 所在的年代，Lucene 和 Solr 还放在同一个仓库 `lucene-solr` 里，后来才拆分。）

`IndexWriter` 是 Lucene 里构建 / 更新索引的核心类，是完全线程安全的。两个相关动作：commit——把缓冲中的改动持久化下来；merge——把许多小的索引段合并成大段，从而保持搜索高效。

### 实验三 · Netty（Java 8+ 与 Apache Maven）

- 仓库：<https://github.com/netty/netty>
- 工具链：Java 8 及以上 + Apache Maven。

Netty 是 Java 生态里最重要的高性能异步网络框架，gRPC-Java、Elasticsearch、Cassandra、Spark 等大量系统的网络层都用它。它让你用非阻塞 I/O 高效地写 TCP / HTTP 等服务端与客户端。

因为 Netty 是异步的，一个操作不会立刻给你结果，而是返回一个 `Future` / `Promise` 对象（这里是 `DefaultPromise`），代表“一个稍后才会就绪的结果”。`DefaultPromise` 内部用 Java 对象监视器（等价于课程中学习的信号量）来协调“等待者”和“完成者”：等待结果的线程在 `await0` 例程里 `wait()`，而设置结果的线程在 `setValue0` 里写入结果后 `notifyAll()` 唤醒等待者。

### 实验四 · Linux 内核与并发检测（C 语言）

- 仓库：https://github.com/torvalds/linux
- 工具链：C 语言 + Make + GCC/Clang

Linux 内核是支撑当今绝大多数服务器、智能手机和云计算底座的操作系统核心。与用户态的普通应用不同，内核空间是一个环境极其恶劣的“高并发战场”：不仅有成千上万的用户态进程通过系统调用频繁涌入内核，还有随时随地触发的硬件中断、软中断（SoftIRQ）以及内核线程在多核 CPU 上无序交错执行。为了保护内存管理、网络协议栈、文件系统等全局数据结构，内核广泛使用了自旋锁（Spinlock）、互斥锁（Mutex）、读写锁以及 RCU 等复杂的同步原语。

在内核中写出并发 Bug 是致命的——用户态程序死锁最多卡死一个进程，而内核级死锁往往会导致整个操作系统直接冻结或崩溃（Kernel Panic），且极其难以通过常规断点调试。正因如此，Linux 社区的顶尖黑客们被迫在内核中原生实现了一整套“运行时锁正确性检测机制”（例如大名鼎鼎的 lockdep）。理解这些机制，不仅是在窥探顶级的系统级 C 语言工程实践，更是学习图论、死锁检测算法在工业界落地的绝佳素材。

## 三、三个 Bug 的相关代码节选

下面是父提交（buggy 版本）的真实代码节选，理解问题所需的上下文都在这里：理论上讲，一个足够优秀的工程师只需要阅读这些代码就可以理解三个 Bug。

> 重要：问题不会在这里被点破。 注释只描述代码在做什么，不会告诉你哪里错了。请你结合并发与锁的知识、借助 AI，自己把藏在这些代码交错里的 Bug 找出来。代码里出现的文件名、类名、方法名，就是你去 git 历史里定位“修复 commit”的线索。

### 3.1 实验一 · zap —— `zapcore/buffered_write_syncer.go`

```go
type BufferedWriteSyncer struct {
	WS WriteSyncer
	// ... 省略 Size / FlushInterval / Clock 等配置字段 ...

	mu          sync.Mutex    // 保护下面所有内部状态的互斥锁
	initialized bool
	stopped     bool
	writer      *bufio.Writer
	ticker      *time.Ticker
	stop        chan struct{} // 关闭它来通知 flushLoop 退出
	done        chan struct{} // flushLoop 退出后由它来关闭
}

// Sync 把缓冲区里的数据刷到下游。注意：它要先拿 s.mu。
func (s *BufferedWriteSyncer) Sync() error {
	s.mu.Lock()
	defer s.mu.Unlock()

	var err error
	if s.initialized {
		err = s.writer.Flush()
	}
	return multierr.Append(err, s.WS.Sync())
}

// flushLoop 是后台协程：按 ticker 周期性地调用 Sync()，直到收到 stop 信号。
func (s *BufferedWriteSyncer) flushLoop() {
	defer close(s.done)
	for {
		select {
		case <-s.ticker.C:
			_ = s.Sync()
		case <-s.stop:
			return
		}
	}
}

// Stop 关闭写入器：停掉后台协程，并刷出剩余数据。
func (s *BufferedWriteSyncer) Stop() (err error) {
	var stopped bool

	// Critical section.
	func() {
		s.mu.Lock()
		defer s.mu.Unlock()

		if !s.initialized {
			return
		}
		stopped = s.stopped
		if stopped {
			return
		}
		s.stopped = true

		s.ticker.Stop()
		close(s.stop) // 通知 flushLoop 退出
		<-s.done      // 等待 flushLoop 真正退出
	}()

	if !stopped {
		err = s.Sync()
	}
	return err
}
```

> 提示：盯住 `Stop()` 临界区里那一行 `<-s.done`，以及后台 `flushLoop` 在 `<-s.ticker.C` 这一支里调用的 `Sync()`——它们对同一把 `s.mu` 的需求关系。这个 Bug 是偶发的，复现时用 `-count` 配 `-timeout`。

### 3.2 实验二 · Lucene —— `IndexWriter.java` 与 `ConcurrentMergeScheduler.java`

```java
// ===== IndexWriter.java =====

// Used only by commit and prepareCommit, below; lock
// order is commitLock -> IW
private final Object commitLock = new Object();

// 提交路径：在持有 commitLock 期间会调用 maybeMerge(...)
private long prepareCommitInternal(MergePolicy mergePolicy) throws IOException {
    startCommitTime = System.nanoTime();
    synchronized (commitLock) {
        ensureOpen(false);
        // ... 准备本次提交的段、写盘 ...
        boolean success = false;
        try {
            if (anySegmentsFlushed) {
                maybeMerge(mergePolicy, MergeTrigger.FULL_FLUSH, UNBOUNDED_MAX_MERGE_SEGMENTS);
            }
            startCommit(toCommit);
            // ...
        }
        // ...
    }
}

// 回滚 / 关闭路径：同样要拿 commitLock
private void rollbackInternal() throws IOException {
    // Make sure no commit is running, else e.g. we can close while another thread is still fsync'ing:
    synchronized (commitLock) {
        rollbackInternalNoCommit();
    }
}

// 后台合并线程执行一次合并；一旦合并途中抛出致命异常，就走到 tragicEvent：
public void merge(MergePolicy.OneMerge merge) throws IOException {
    // ... 实际执行段合并 ...
    } catch (Throwable t) {
        // Important that tragicEvent is called after mergeFinish, else we hang
        // waiting for our merge thread to be removed from runningMerges:
        tragicEvent(t, "merge");   // tragicEvent 最终会触发 rollback → rollbackInternal
    }
    // ...
}
```

```java
// ===== ConcurrentMergeScheduler.java =====
// IndexWriter.maybeMerge(...) 最终会进到这里（整个方法 synchronized 在调度器对象上）
public synchronized void merge(IndexWriter writer, MergeTrigger trigger, boolean newMergesFound)
        throws IOException {
    // ...
}

// 当合并积压（pending 太多、合并线程已达上限）时，调用方线程会被“stall”住等待：
protected synchronized boolean maybeStall(IndexWriter writer) {
    while (writer.hasPendingMerges() && mergeThreadCount() >= maxMergeCount) {
        // ... 合并落后太多，暂停这个“生产者”线程，直到合并追上来 ...
        doStall();
    }
    return true;
}

/ Called from {@link #maybeStall} to pause the calling thread for a bit. */
protected synchronized void doStall() {
    try {
        wait(250); // 等待后台合并消化积压
    } catch (InterruptedException ie) {
        throw new ThreadInterruptedException(ie);
    }
}
```

> 提示：注意那行注释 `lock order is commitLock -> IW`——它在暗示一个约定的加锁顺序。再想一想：提交线程在持有 `commitLock` 时进入 `maybeMerge`，可能在 `doStall` 里等合并完成；而一个出错的合并线程要走 `rollbackInternal` 又得拿 `commitLock`。两者之间是否构成了环形等待？这个 Bug 由对应回归测试用线程协调确定性地触发，每次都会卡死；卡死后用 `jstack <pid>` 抓 dump，你会看到两个线程卡在同一把监视器锁上。构建用 JDK 8；旧版 Ant/Ivy 构建可能需要把依赖源指到 HTTPS 的 Maven 中央仓库（让 AI 帮你处理这个小问题）。

### 3.3 实验三 · Netty —— `common/.../util/concurrent/DefaultPromise.java`

```java
// 设置结果（成功 / 失败）时调用：先写入结果，再去唤醒等待者
private boolean setValue0(Object objResult) {
    if (RESULT_UPDATER.compareAndSet(this, null, objResult) ||
        RESULT_UPDATER.compareAndSet(this, UNCANCELLABLE, objResult)) {
        checkNotifyWaiters();
        return true;
    }
    return false;
}

private synchronized void checkNotifyWaiters() {
    if (waiters > 0) {
        notifyAll();
    }
}

private void incWaiters() {
    if (waiters == Short.MAX_VALUE) {
        throw new IllegalStateException("too many waiters: " + this);
    }
    ++waiters;
}

private void decWaiters() {
    --waiters;
}

// 带超时地等待这个 Promise 完成
private boolean await0(long timeoutNanos, boolean interruptable) throws InterruptedException {
    if (isDone()) {                 // (A) 先在锁外检查一次
        return true;
    }
    if (timeoutNanos <= 0) {
        return isDone();
    }
    if (interruptable && Thread.interrupted()) {
        throw new InterruptedException(toString());
    }
    checkDeadLock();

    long startTime = System.nanoTime();
    long waitTime = timeoutNanos;
    boolean interrupted = false;
    try {
        for (;;) {
            synchronized (this) {
                incWaiters();
                try {
                    wait(waitTime / 1000000, (int) (waitTime % 1000000));   // (B) 进入等待
                } catch (InterruptedException e) {
                    if (interruptable) {
                        throw e;
                    } else {
                        interrupted = true;
                    }
                } finally {
                    decWaiters();
                }
            }
            if (isDone()) {         // (C) wait 之后再检查
                return true;
            } else {
                waitTime = timeoutNanos - (System.nanoTime() - startTime);
                if (waitTime <= 0) {
                    return isDone();
                }
            }
        }
    } finally {
        if (interrupted) {
            Thread.currentThread().interrupt();
        }
    }
}
```

> 提示：对照条件变量的“黄金法则”——检查谓词（`isDone()`）和 `wait()` 必须在同一把锁内完成。数一数 `await0` 在 `synchronized (this)` 块内部、`wait()` 之前有没有再检查一次 `isDone()`？再设想：完成线程在 (A) 检查之后、等待线程真正 `wait()` 之前的那一瞬间调用了 `setValue0` → `checkNotifyWaiters`，此刻 `waiters` 是几？这个 Bug 不是死锁，而是“等待异常地久”（本该立刻返回，却睡满整个超时）。回归测试会创建大量（数千个）promise/线程对来制造这个竞争。构建时只编译 / 测试 Netty 的 `common` 子模块即可（快很多），命令让 AI 给你。

## 四、Linux 内核中几个与锁相关的“自动检测机制”

工业界对付并发 Bug，除了靠人去读栈，还造了一批能自动发现锁误用的工具。Linux 内核是这方面的集大成者——它内置了一整套运行时检测器，能在内核还没真正卡死之前，就把潜在的死锁、数据竞争等问题打印到 `dmesg` 里。

这一节只给你这些机制的名字。具体每个机制是怎么实现的、用到哪些数据结构、算法是什么——需要你自己 clone 内核源码，让 AI 对着真实代码向你讲解（内核仓库：<https://github.com/torvalds/linux>。

需要你研究并理解的机制（任选其中几个，至少覆盖 lockdep 和 KCSAN 这两个）：

- lockdep（配置项 `CONFIG_PROVE_LOCKING`）——运行时锁依赖关系校验器，能在不真正死锁的情况下检测出潜在的循环加锁依赖（ABBA 之类）。
- KCSAN（Kernel Concurrency Sanitizer，`CONFIG_KCSAN`）——内核数据竞争检测器。
- `CONFIG_DEBUG_ATOMIC_SLEEP`——检测“在原子 / 不可睡眠上下文里调用了可能睡眠的函数”（`might_sleep`）。
- `CONFIG_DEBUG_SPINLOCK` / `CONFIG_DEBUG_MUTEXES`——自旋锁 / 互斥锁的运行时正确性检查。
- `CONFIG_PROVE_RCU`——RCU 使用正确性校验。
- lockstat（`CONFIG_LOCK_STAT`）——锁竞争的统计与分析。
- （可选拔高）`CONFIG_DEBUG_LOCKING_API_SELFTESTS`——内核自带的加锁 API 自测，开机即在 `dmesg` 演示 lockdep 抓各类故意构造的死锁。

研究这些机制时，建议带着问题去问 AI：它用什么数据结构记录“谁在什么时候、按什么顺序拿了哪些锁”？它怎么建图、怎么判环？为什么它能在第一次观察到某种加锁顺序时就报警，而不需要真的发生死锁？

## 五、需要提交的内容

### 5.1 三个 Bug（zap / Lucene / Netty）

对每一个 Bug，提交以下三样：

1. 复现日志（文本）——两段终端输出：
   - 在父提交（buggy） 上运行回归测试 → 失败 / 卡死（超时）；
   - 在修复提交（fixed） 上运行同一个回归测试 → 通过。
   并写明：你用到的两个 commit 哈希、回归测试的名字、以及你跑的完整命令。
2. 截图——能证明是你本人完成的复现。截图里必须出现你的姓名和学号：可以另开一个记事本 / 编辑器窗口写上姓名学号，和复现窗口一起截进去。
3. 理解说明（可借助 AI，但要自己读懂）：这是哪一类锁问题？涉及哪把锁 / 哪个同步原语？代码层面出在哪个函数的哪几行？两个执行体怎样交错（interleaving）才触发？修复 commit 改了什么、为什么这样改就能解决？

> 助教没法逐一核实你是不是真的读懂了 AI 写给你的东西。但强烈建议你真正把这几个 Bug 搞懂——期末会考锁，而且这对将来工作和面试都很有帮助。这几个例子（自死锁、ABBA 环形等待、条件变量丢唤醒）几乎是并发面试的“必考母题”。

### 5.2 内核检测机制

提交一份对你所研究机制的讲解：贴出该机制涉及的具体数据结构和关键代码（来自你 clone 的内核源码），配上你自己的解释。标准是：让一个“懂锁、但完全不了解内核”的人读完，就能明白这个机制是怎么工作的——比如 lockdep 用什么结构记录锁的获取顺序、如何建图判环。

## 六、使用 AI 的建议

- 不要吊死在一棵树上。 AI 总是做不出来不是你的问题，是那个 AI 的问题——换一个 AI 试试。
- 一个对话里一直解决不了问题，就回滚修改或开一个新对话。 上下文越长，AI 越容易在错误的方向上越走越远。
- 需要执行时间较长的任务，可以让 AI 把命令给你，自己跑。 这样不容易焦虑，也方便中途中断和恢复。
- 多和同学交流。 AI 变化很快，助教个人也经常跟不上时代，要多向会用 AI 的同学请教和学习。

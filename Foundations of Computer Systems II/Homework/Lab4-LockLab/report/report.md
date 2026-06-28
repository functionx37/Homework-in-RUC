# LockLab Report

## 实验一：zap

### 复现日志

buggy 日志保存在 [zap/zap-exp1/out/buggy.log](/zap/zap-exp1/out/buggy.log)

fixed 日志保存在 [zap/zap-exp1/out/fixed.log](/zap/zap-exp1/out/fixed.log)

本实验定位到的两个版本如下：

- buggy 版本：`b15585bc7a2b383592004f75df35fa2088db5481`，即修复提交的父提交
- fixed 版本：`3ffa0c00594205c2c623aa9493847217982b558d`

修复提交信息：`Fix deadlock when Stop and flush race (#1430)`。

使用的回归测试是 `TestBufferWriter/stop_race_with_flush`，位于 `zapcore/buffered_write_syncer_test.go`。为了在两个版本上运行同一个回归测试，我将 fixed 版本中新加入的这个子测试同步放到了 `zap/zap-buggy` 中，再分别运行。

我使用的脚本命令为：

```bash
GO_BIN=/usr/local/go/bin/go ./zap/zap-exp1/run_compare.sh
```

复现结果如下：

- 在 buggy 版本上，测试在 `5s` 超时后失败，日志中可以看到 `Stop()` 卡在 `<-s.done`，而后台 `flushLoop()` 卡在 `Sync()` 里的 `s.mu.Lock()`，说明发生了死锁。
- 在 fixed 版本上，同一个测试正常通过，输出为 `ok   go.uber.org/zap/zapcore`。

### 理解说明

这个 Bug 属于典型的“持锁等待”导致的死锁问题。涉及的同步原语主要有一把互斥锁 `s.mu`，以及两个用于协程协调的 channel：`stop` 和 `done`。其中 `s.mu` 保护 `BufferedWriteSyncer` 的内部状态，`stop` 用来通知后台刷盘协程退出，`done` 用来表示后台协程已经真正结束。

出问题的关键代码在 buggy 版本的 [buffered_write_syncer.go](/zap/zap-buggy/zapcore/buffered_write_syncer.go)：

- `Sync()` 在第 158-167 行先执行 `s.mu.Lock()`，也就是说无论谁调用 `Sync()`，都必须先拿到这把锁。
- `flushLoop()` 在第 172-181 行收到 `ticker` 事件后会调用 `s.Sync()`。
- `Stop()` 在第 190-210 行持有 `s.mu` 的情况下执行 `close(s.stop)`，然后立刻执行 `<-s.done` 等待后台协程退出。

死锁的触发交错如下：

1. `BufferedWriteSyncer` 已经初始化完成，后台 `flushLoop` 正在运行。
2. 线程 A 调用 `Stop()`，进入临界区并拿到 `s.mu`。
3. 在线程 A 还持有 `s.mu` 时，`ticker` 刚好触发；后台协程线程 B 在 `flushLoop()` 中进入 `case <-s.ticker.C:` 分支，并调用 `s.Sync()`。
4. 线程 B 在 `Sync()` 中尝试执行 `s.mu.Lock()`，但是这把锁已经被线程 A 持有，因此线程 B 阻塞。
5. 与此同时，线程 A 在 `Stop()` 中执行 `close(s.stop)` 后，继续执行 `<-s.done`，等待后台 `flushLoop()` 退出。
6. 但是 `done` 只有在 `flushLoop()` 返回时才会被关闭，而 `flushLoop()` 此时正卡在 `s.Sync()` 的加锁处，根本无法返回。

这样就形成了环形等待：

- `Stop()` 持有 `s.mu`，等待 `done`
- `flushLoop()` 想关闭 `done`，但它必须先完成 `Sync()`，而 `Sync()` 又在等待 `s.mu`

因此两边永久互相等待，测试最终表现为超时。

修复提交 `3ffa0c00594205c2c623aa9493847217982b558d` 的核心改动是把等待 `flushLoop` 退出这件事移到了临界区外面。修复后的 `Stop()` 先在锁内只做状态检查、设置 `s.stopped = true`、停止 ticker 和 `close(s.stop)`，然后立即释放 `s.mu`。释放锁之后，`Stop()` 再去执行 `<-s.done`，最后再调用一次 `s.Sync()` 做收尾刷盘。

这样修改后，若后台 `flushLoop()` 此时正好因为 `ticker` 进入了 `Sync()`，它也能顺利拿到 `s.mu`、完成刷盘并退出，然后关闭 `done`；外层 `Stop()` 也就能继续执行，不再形成死锁。换句话说，修复的本质就是避免“拿着 `s.mu` 等待一个还需要 `s.mu` 才能完成的协程”。

## 实验二：Lucene

### 复现日志

buggy 日志保存在 [lucene/lucene-exp2/out/buggy.log](/lucene/lucene-exp2/out/buggy.log)

fixed 日志保存在 [lucene/lucene-exp2/out/fixed.log](/lucene/lucene-exp2/out/fixed.log)

本实验定位到的两个版本如下：

- buggy 版本：`72537fd2cd8caf1d85c0ec0ffac52b167f6982b2`，即修复提交的父提交
- fixed 版本：`ea3f8363319955c589eb3a7df59a031621852d3e`

修复提交信息：`LUCENE-7570: don't run merges while holding the commitLock to prevent deadlock when merges are stalled and a tragic merge exception strikes`。

使用的回归测试是 `TestTragicIndexWriterDeadlock.testDeadlockStalledMerges`，位于 `lucene/core/src/test/org/apache/lucene/index/TestTragicIndexWriterDeadlock.java`。为了在两个版本上运行同一个回归测试，我将 fixed 版本中新加入的这个测试方法同步放到了 buggy 版本中，再分别运行。

我使用的脚本命令为：

```bash
JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64 ANT_BIN=/usr/bin/ant TIME_LIMIT=300s ./lucene/lucene-exp2/run_compare.sh
```

复现结果如下：

- 在 buggy 版本上，测试实际进入了 `TestTragicIndexWriterDeadlock.testDeadlockStalledMerges`，并持续输出 heartbeat，显示该测试在同一个方法内长时间卡住；最终被外层 `timeout 300s` 杀掉，退出码为 `124`，说明发生了死锁/永久等待。
- 在 fixed 版本上，同一个测试在 `0.13s` 内通过，日志中出现 `OK      0.13s | TestTragicIndexWriterDeadlock.testDeadlockStalledMerges`、`BUILD SUCCESSFUL`，退出码为 `0`。

### 理解说明

这个 Bug 属于典型的“持锁调用可能阻塞的合并逻辑”导致的死锁问题。涉及的同步原语主要有：

- `commitLock`：专门保护 `commit()` / `prepareCommit()` / `rollback()` 这条提交与回滚路径的锁。
- `IndexWriter` 自身的内置监视器锁（源码注释写作 `IW`）。
- `ConcurrentMergeScheduler` 自身的监视器锁，用来保护 `merge()`、`maybeStall()` 和 `doStall()`。

出问题的关键代码在 buggy 版本的 [IndexWriter.java](/lucene/lucene-buggy/lucene/core/src/java/org/apache/lucene/index/IndexWriter.java) 和 [ConcurrentMergeScheduler.java](/lucene/lucene-buggy/lucene/core/src/java/org/apache/lucene/index/ConcurrentMergeScheduler.java)：

- `prepareCommitInternal(...)` 在持有 `commitLock` 的情况下，如果 `anySegmentsFlushed` 为真，就直接调用 `maybeMerge(...)`。
- `ConcurrentMergeScheduler.maybeStall(...)` 在检测到 pending merge 太多、合并线程数达到上限时，会进入 `doStall()`，也就是在调度器锁上 `wait()`，把当前调用线程暂停住。
- 后台 merge 线程如果在 `mergeSuccess(...)` 之后遭遇致命异常，会进入 `tragicEvent(...)`，后者最终调用 `rollbackInternal()`。
- `rollbackInternal()` 又必须先拿到 `commitLock`，这样才能保证没有其他提交正在执行。

死锁的触发交错如下：

1. `IndexWriter` 正在执行 `commit()`，进入 `prepareCommitInternal()`，并拿到了 `commitLock`。
2. 这次提交触发了 `anySegmentsFlushed`，于是提交线程在持有 `commitLock` 的情况下调用 `maybeMerge(...)`。
3. `maybeMerge(...)` 进入 `ConcurrentMergeScheduler.merge(...)`，由于已有 merge 在线程池里运行且积压过多，于是调用线程进入 `maybeStall()`，最终在 `doStall()` 里等待，也就是“拿着 `commitLock` 去等 merge 消化完积压”。
4. 与此同时，后台 merge 线程在一次 merge 完成后，`mergeSuccess(...)` 抛出 `OutOfMemoryError`，触发 `tragicEvent(...)`。
5. `tragicEvent(...)` 需要执行 `rollbackInternal()` 来关闭和回滚 writer，但 `rollbackInternal()` 必须先拿到 `commitLock`。
6. 此时 `commitLock` 还被第 3 步中那个已经 stall 住的提交线程持有，因此后台 merge 线程无法继续；而提交线程又在等待 merge 线程推进，形成环形等待。

这样就形成了死锁关系：

- 提交线程持有 `commitLock`，等待 merge 线程把积压 merge 处理掉；
- merge 线程想在 tragic rollback 里拿到 `commitLock`，但这把锁被提交线程一直占着。

因此两边永久互相等待，测试最终表现为 heartbeat 一直增长，直到外层超时终止。

修复提交 `ea3f8363319955c589eb3a7df59a031621852d3e` 的核心改动，是把 `maybeMerge(...)` 从 `commitLock` 的临界区里移了出去。修复后的逻辑不再在持有 `commitLock` 时直接跑 merge，而是在锁内只记录一个 `doMaybeMerge` 标记，等退出 `commitLock` 之后再调用 `maybeMerge(...)`。

这样修改后，即使 `maybeMerge()` 因 merge 积压而进入 `doStall()`，它也已经不再占着 `commitLock`；一旦后台 merge 线程触发 `tragicEvent()`，就还能顺利拿到 `commitLock` 进入 `rollbackInternal()`，不会再和提交线程互相卡住。换句话说，修复的本质就是避免“拿着 `commitLock` 等待一个在异常路径上也需要 `commitLock` 的线程”。

## 实验三：Netty

### 复现日志

buggy 日志保存在 [netty/netty-exp3/out/buggy.log](/netty/netty-exp3/out/buggy.log)

fixed 日志保存在 [netty/netty-exp3/out/fixed.log](/netty/netty-exp3/out/fixed.log)

本实验定位到的两个版本如下：

- buggy 版本：`b37a41a535e41b573c09a00226865561327fb01a`，即修复提交的父提交
- fixed 版本：`cfd6db79150fdb9ea91a3ff2eb6117c23299a12c`

修复提交信息：`Avoid missed signals on a default promise`。

使用的回归测试是 `DefaultPromiseTest.testSignalRace`，位于 `common/src/test/java/io/netty/util/concurrent/DefaultPromiseTest.java`。为了在两个版本上运行同一个回归测试，我将 fixed 版本中新加入的这个测试方法同步放到了 `netty/netty-buggy` 中，再分别运行。

我使用的脚本命令为：

```bash
JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64 MVN_BIN=/usr/bin/mvn ./netty/netty-exp3/run_compare.sh
```

复现结果如下：

- 在 buggy 版本上，`testSignalRace` 失败，日志中出现 `Expected: a value less than <10000000000L>`，但实际等待时间为 `10000258482` 纳秒，略高于 `10s` 超时阈值，说明等待线程错过了本应立刻收到的唤醒信号，异常地睡满了整个超时。
- 在 fixed 版本上，同一个测试在 `1.339 sec` 内通过，日志中出现 `Tests run: 1, Failures: 0, Errors: 0, Skipped: 0` 和 `BUILD SUCCESS`，退出码为 `0`。

### 理解说明

这个 Bug 属于典型的“条件检查与等待不在同一把锁内完成”导致的丢信号问题。它不是死锁，而是等待线程明明对应的 `Promise` 已经完成了，却因为错过 `notifyAll()` 而额外睡到超时。涉及的同步原语主要有：

- `DefaultPromise` 对象自己的监视器锁 `synchronized (this)`。
- `wait()` / `notifyAll()` 这一组条件等待机制。
- `waiters` 计数器，用来记录当前有多少线程正在等待这个 `Promise`。

出问题的关键代码在 buggy 版本的 [DefaultPromise.java](/netty/netty-buggy/common/src/main/java/io/netty/util/concurrent/DefaultPromise.java)：

- `setValue0(...)` 在第 546-552 行把结果写入 `result` 后，立刻调用 `checkNotifyWaiters()`。
- `checkNotifyWaiters()` 在第 555-559 行只有在 `waiters > 0` 时才会执行 `notifyAll()`。
- `await0(...)` 在第 581-629 行先在锁外检查一次 `isDone()`，然后进入循环；但在第 601-604 行进入 `synchronized (this)` 之后，并没有在 `wait()` 之前再次检查 `isDone()`，而是直接 `incWaiters()` 再 `wait(...)`。

问题的触发交错如下：

1. 线程 A 调用 `awaitUninterruptibly(...)`，进入 `await0(...)`。
2. 在线程 A 的第 582 行锁外检查 `isDone()` 时，`Promise` 还没有完成，因此它继续往下执行。
3. 在线程 A 真正进入 `synchronized (this)` 并调用 `wait()` 之前，线程 B 恰好调用 `setSuccess(...)`，最终进入 `setValue0(...)`。
4. 线程 B 在第 547-548 行成功把 `result` 从未完成状态更新为完成状态，然后调用 `checkNotifyWaiters()`。
5. 但这时线程 A 还没有执行 `incWaiters()`，所以 `waiters == 0`；第 556 行条件不成立，线程 B 不会调用 `notifyAll()`。
6. 随后线程 A 才进入同步块，执行 `incWaiters()` 并开始 `wait(10s)`。由于那次唯一需要的唤醒已经错过，它只能一直睡到超时或者等到别的事件偶然唤醒。

这样就形成了“丢信号”：

- 完成线程已经把 `Promise` 置为 done；
- 等待线程随后才真正进入 `wait()`；
- 但由于 `waiters` 计数发生得太晚，完成线程那次 `notifyAll()` 根本没有发出来。

因此测试中的等待线程会异常地久，最终表现为耗时接近整整 `10s`，从而触发 `assertThat(..., lessThan(wait))` 失败。

修复提交 `cfd6db79150fdb9ea91a3ff2eb6117c23299a12c` 的核心改动，是在 `await0(...)` 的 `synchronized (this)` 内、`incWaiters()` 和 `wait()` 之前，再补一次 `isDone()` 检查。也就是说，修复后逻辑变成：

1. 锁外先做一次快速检查，避免不必要加锁；
2. 若仍未完成，进入 `synchronized (this)`；
3. 在同一把锁内再次检查 `isDone()`，若此时已经完成就直接返回；
4. 只有在锁内确认“确实还没完成”时，才增加 `waiters` 并调用 `wait()`。

这样修改后，谓词检查和 `wait()` 就遵守了条件变量的基本规则：二者必须在同一把锁保护下完成。即使完成线程恰好发生在第一次锁外检查之后，只要它先拿到了锁并完成了结果写入，等待线程后续进入同步块时也会在第二次 `isDone()` 检查处直接返回，不会再进入 `wait()`，因此不会再错过唤醒信号。

## Linux 内核检测机制

本节我选取了 Linux 内核里两个最有代表性的并发检测机制来分析：`lockdep` 和 `KCSAN`。前者关注“锁的使用顺序是否可能导致死锁”，后者关注“两个并发执行体是否对同一内存位置发生了未同步的数据竞争”。二者一个偏向锁依赖图分析，一个偏向内存访问采样检测，正好对应并发错误里最常见的两大类问题。

我阅读的源码来自 Linux 内核仓库 `torvalds/linux` 的当前快照 `780d569`。下面只挑出理解机制所必需的关键数据结构和关键执行路径来讲。

### 1. lockdep：运行时锁依赖图校验器

`lockdep` 对应的配置项是 `CONFIG_PROVE_LOCKING`。它的核心目标不是“等系统真的死锁后再报错”，而是在运行过程中一旦观察到某种新的加锁顺序，就立刻把这种顺序纳入全局依赖图，并检查它会不会和过去见过的顺序组成环。如果会，就直接在 `dmesg` 里报警。也就是说，它检测的是“潜在死锁”。

#### 1.1 lockdep 记录的对象：不是具体某一把锁，而是 lock class

`lockdep` 的基本单位不是某个具体锁实例，而是“锁类（lock class）”。官方文档 `Documentation/locking/lockdep-design.rst` 开头就强调，语义上等价的一组锁会被当成同一个 class 对待，例如“每个 inode 里都有一把同类型的锁”，这些实例虽然地址不同，但在 lockdep 看来属于同一类规则对象。

对应的数据结构在 `include/linux/lockdep_types.h`：

- `struct lock_class`，见 `include/linux/lockdep_types.h:98`。这是锁类对象，最关键的两个字段是 `locks_after` 和 `locks_before`（第 111-116 行），它们共同表示一张有向图：
  - 如果曾经观察到“持有 L1 时再去获取 L2”，就会建立一条 `L1 -> L2` 的依赖边。
  - `locks_after` 记录从当前节点出发的后继边，`locks_before` 记录前驱边。
- `struct lockdep_map`，见 `include/linux/lockdep_types.h:186`。这是嵌在具体锁对象里的元数据，用来把“锁实例”映射到某个 `lock_class`。其中 `key` 是类标识，`class_cache` 是为了避免重复查表的缓存。
- `struct held_lock`，见 `include/linux/lockdep_types.h:206`。这是“当前线程已经持有的锁栈”中的单个元素。它记录当前拿着的是哪一类锁、在哪个调用点获取、前一段锁链的哈希值 `prev_chain_key` 等。

这里最重要的抽象是：**lockdep 并不关心某一时刻只有哪两个线程在竞争；它关心的是“整个系统历史上出现过哪些加锁偏序关系”**。因此它能把不同线程、不同时间观察到的顺序拼成一张全局图，再检查图上是否存在环。

#### 1.2 lock class 如何注册

当内核第一次看到某把锁被获取时，会把它映射到某个 `lock_class`。相关代码在 `kernel/locking/lockdep.c`：

- `look_up_lock_class()`，见 `kernel/locking/lockdep.c:887`。它根据 `lockdep_map` 里的 `key` 和 `subclass` 到哈希表里找已有的 `lock_class`。
- `register_lock_class()`，见 `kernel/locking/lockdep.c:1285`。如果查不到，就分配并初始化一个新的 `lock_class`，挂入全局哈希表。第 1339-1345 行会填充 `key`、`name`、`subclass`、`wait_type` 等基本信息。

这一步对应的设计思想很清晰：先把“这把锁属于哪一类规则对象”统一编号，后面所有依赖分析都只针对 class 做，而不是针对每个对象实例做。否则像 inode 这种成千上万个对象会让图爆炸。

#### 1.3 每次拿锁时 lockdep 做什么

所有 `mutex_lock*()`、`spin_lock*()` 之类的加锁操作，最终都会走到 `lock_acquire()` 和 `__lock_acquire()`：

- `lock_acquire()`，见 `kernel/locking/lockdep.c:5825`。它负责关中断、避免递归进入 lockdep，然后调用内部实现 `__lock_acquire()`。
- `__lock_acquire()`，见 `kernel/locking/lockdep.c:5077`。这是 lockdep 的核心入口。

`__lock_acquire()` 的关键流程可以概括成四步：

1. 找到或注册当前锁对应的 `lock_class`。
2. 构造一个新的 `held_lock`，准备把它压到当前任务的“持锁栈”里。
3. 计算新的锁链哈希 `chain_key`。相关代码在第 5194-5225 行。这里的目的不是检测死锁，而是给“当前持锁路径”做缓存键，避免每次都把整张图从头走一遍。
4. 调用 `validate_chain()` 检查“把这把新锁接到当前锁链末尾”是否合法；如果合法，才真正更新当前任务的持锁状态（第 5237 行之后）。

这说明 lockdep 的工作时机是“拿锁时在线检查”，而不是事后离线分析。

#### 1.4 lockdep 如何建图与判环

lockdep 的关键思想是：如果当前线程已经持有 `prev`，现在又要获取 `next`，那就意味着系统里出现了一条新的依赖边 `prev -> next`。但在真正把这条边加入图之前，它先反过来检查图中是否已经存在一条从 `next` 回到 `prev` 的路径；如果存在，那么新边一加上去就形成环，也就是潜在死锁。

这部分关键代码是：

- `check_prev_add()`，见 `kernel/locking/lockdep.c:3122`。注释第 3155-3165 行直接写明：它要证明新依赖 `prev -> next` 不会制造环。
- `check_noncircular()`，见 `kernel/locking/lockdep.c:2149`。它从 `src` 出发做图搜索，检查能否到达 `target`。如果能到达，就说明环存在，并打印死锁报告。

具体地说，`check_prev_add()` 做的是：

- 当前线程已持有若干把锁，lockdep 会把“新锁 `next`”与持锁栈中的每一把旧锁 `prev` 两两配对。
- 对每个配对，先调用 `check_noncircular(next, prev, ...)`。
- 这等价于在问：图里现在是否已经有一条 `next => ... => prev` 的路径？
- 如果答案是有，那么再新增 `prev -> next` 就闭环了。

这个算法本质上就是图论里的“加边前判环”。README 里提到的 ABBA 问题正好可以这样理解：

1. 线程 1 曾经拿过 `A -> B`，于是图中有边 `A -> B`。
2. 后来线程 2 又尝试拿 `B -> A`。
3. 在加 `B -> A` 之前，lockdep 检查到当前图里已经有从 `A` 到 `B` 的路径。
4. 因而它知道新边一加就成环，即使这次运行里线程 1 和线程 2 还没有真的互相卡死，也会立刻报警。

这就是 lockdep 最强的地方：**它并不要求死锁真的发生，只要求系统“观察到足以推出死锁可能性”的锁顺序证据**。

#### 1.5 为什么 lockdep 能在第一次观察到错误顺序时报警

原因就在于它维护的是“历史依赖图”，而不是“当前阻塞图”。

- 当前阻塞图只能告诉你“此刻谁在等谁”，适合发现已经发生的死锁。
- 历史依赖图记录的是“系统里曾经允许过哪些加锁顺序”，适合发现未来可能发生的死锁。

只要图里已经有 `A -> B`，再出现一次 `B -> A` 就足够证明系统的锁顺序约束不一致。此时即便这两次顺序发生在完全不同的时间、不同的线程、不同的对象实例上，只要属于同一个 lock class，lockdep 就认为这是一条真实的风险证据。

#### 1.6 lockdep 的优点与局限

优点：

- 能在死锁真正发生前提前报警。
- 能跨线程、跨时间汇总锁顺序信息。
- 除了环形等待，还会检查 IRQ/softirq 上下文中的锁使用规则，这也是 `usage_mask` 等字段存在的原因。

局限：

- 它主要验证“锁使用协议”是否一致，不负责检测普通数据竞争。
- 它依赖运行时实际观察到的加锁路径；没跑到的路径，就无法证明其正确性。
- 锁类数量和依赖图大小都有上限，因此属于工程上“有界但高价值”的动态分析。

总的来说，`lockdep` 可以理解为一个运行在内核里的“在线锁顺序图验证器”。

### 2. KCSAN：基于采样和软 watchpoint 的数据竞争检测器

`KCSAN` 对应配置项是 `CONFIG_KCSAN`。它解决的问题和 lockdep 完全不同：如果两个 CPU/线程没有通过锁、原子操作或内存屏障正确同步，却并发访问了同一内存位置，并且至少有一个是写，那么就可能产生 data race。KCSAN 的目标就是在运行时抓住这类竞争。

与用户态 ThreadSanitizer 那种“维护完整 happens-before 元数据”的重型方案不同，KCSAN 走的是更轻量的路线：**编译器插桩 + 采样 + 延迟 + 软件 watchpoint**。

#### 2.1 KCSAN 的关键数据结构

1. 访问上下文 `struct kcsan_ctx`

定义在 `include/linux/kcsan.h:22`。关键字段有：

- `disable_count`：当前上下文里是否临时关闭 KCSAN。
- `atomic_next`、`atomic_nest_count`、`in_flat_atomic`：表示哪些后续访问应被视为原子访问，不应该报 race。
- `scoped_accesses`：记录“作用域内持续检查”的访问。
- `reorder_access`：在 `CONFIG_KCSAN_WEAK_MEMORY` 打开时，用来模拟弱内存下的访问重排。

这说明 KCSAN 不是简单地“看到读写就报错”，它还要结合当前执行上下文判断：这次访问是不是原子的、是不是在特殊区域里、是不是需要建模内存重排。

2. 全局 watchpoint 数组

定义在 `kernel/kcsan/core.c:103`：

- `static atomic_long_t watchpoints[...]`

这里每个 watchpoint 都被编码进一个 `atomic_long_t`。源码注释（第 95-103 行）明确写到，这样做是为了避免在 fast path 上引入额外锁开销。换句话说，KCSAN 故意把自己的元数据也设计成接近无锁的形式，否则检测器本身就会显著扰动系统行为。

3. 访问类型标记

定义在 `include/linux/kcsan-checks.h` 开头：

- `KCSAN_ACCESS_WRITE`
- `KCSAN_ACCESS_COMPOUND`
- `KCSAN_ACCESS_ATOMIC`
- `KCSAN_ACCESS_ASSERT`
- `KCSAN_ACCESS_SCOPED`

这些标志让 KCSAN 区分“普通读写”“复合读改写”“原子访问”“断言式检查”等不同语义。

#### 2.2 KCSAN 的总体思路

KCSAN 的核心思想不是给每次访问都维护完整历史，而是只“抽样观察”一小部分访问：

1. 编译器给普通内存访问插桩，最终调用 `__kcsan_check_access()`，见 `kernel/kcsan/core.c:956`。
2. `__kcsan_check_access()` 再调用内部的 `check_access()`，见 `kernel/kcsan/core.c:719`。
3. `check_access()` 先看当前地址上是否已经存在别人设置的 watchpoint；如果有，且冲突成立，就说明抓到了并发访问。
4. 如果没有现成 watchpoint，则按采样策略决定这次访问要不要“被观察”。若决定观察，就给当前地址放一个 watchpoint，并故意延迟当前线程一小段时间，扩大被别的线程撞上的概率。
5. 延迟结束后，再检查这个 watchpoint 是否被命中，或者内存值是否发生变化，据此生成报告。

因此 KCSAN 的本质是：**我不可能盯住每一次访问，但我可以偶尔把一次访问故意“挂在那儿”等别人来撞，从而以较低开销抓到大量真实数据竞争。**

#### 2.3 快路径：先看有没有别人留下的 watchpoint

`check_access()` 的第一步是：

- 调用 `find_watchpoint()`，见 `kernel/kcsan/core.c:114` 以及 `check_access()` 中的 `737-739` 行。

`find_watchpoint()` 做的事很直接：

- 根据访问地址算一个 slot。
- 在附近几个槽位里查找是否已有匹配的 watchpoint。
- 若这次访问期待发现写冲突，还会过滤掉仅表示读的 watchpoint。
- 只要发现地址范围重叠，就认为“可能和另一个线程监视的访问冲突”。

如果找到了，`check_access()` 会走 `kcsan_found_watchpoint(...)` 路径（第 747-748 行），也就是“当前线程撞上了别人布下的监视点”。

#### 2.4 慢路径：决定是否设置 watchpoint

如果没找到已有 watchpoint，KCSAN 不会每次都主动设置新的监视点，因为那样成本太高。它会先走 `should_watch()`，见 `kernel/kcsan/core.c:268`。

`should_watch()` 主要做两件事：

- 如果这次访问被判定为原子访问，就不观察。
- 否则查看每 CPU 的 `kcsan_skip` 计数器；只有计数器耗尽时，才选中这次访问作为采样对象。

也就是说，KCSAN 是一种明确的采样型检测器。它接受“可能漏掉一些 race”，换取足够低的运行时开销。

#### 2.5 设置 watchpoint 并故意延迟

一旦当前访问被选中，`check_access()` 会进入 `kcsan_setup_watchpoint(...)`，其关键逻辑位于 `kernel/kcsan/core.c:591-699`：

- 第 591 行调用 `insert_watchpoint()` 把地址、大小、读写类型编码后放进 `watchpoints[]`。
- 第 605-609 行先读取一次当前内存值，保存为 `old`。
- 第 612-615 行调用 `delay_access(type)`，故意 `udelay(...)` 一小段时间。
- 第 617-645 行延迟结束后再次读取内存值，得到 `new`，比较差异 `diff`。

这里的延迟不是副作用，而是设计本身的一部分。因为数据竞争往往只发生在很窄的时间窗里，如果不把这个时间窗人为放大，两个线程很难恰好在检测器能观察到的时刻撞上。

#### 2.6 KCSAN 如何判定“发现了 race”

KCSAN 主要有两种报告来源。

第一种：已知另一方来源的竞争。

- 如果当前线程设置的 watchpoint 在延迟期间被其他线程命中，那么 `consume_watchpoint()` 会发现这个 watchpoint 已被别人消费。
- 这时走 `kcsan_report_known_origin(...)`，见 `kernel/kcsan/core.c:647-680`。
- 这种报告最好，因为 KCSAN 能同时拿到双方的访问类型、地址和栈信息。

第二种：未知来源的竞争。

- 如果没有明确抓到另一线程，但延迟前后的值发生了变化，说明这块内存很可能在这段时间里被别处改写过。
- 这时走 `kcsan_report_unknown_origin(...)`，见 `kernel/kcsan/core.c:681-690`。
- 这种情况常见于另一侧没有被插桩，或者修改来自 DMA/设备。

因此，KCSAN 既能抓“我明确看到另一个线程撞上来了”，也能抓“虽然没看到对方，但值已经变了，说明存在竞争痕迹”。

#### 2.7 为什么 KCSAN 不需要重型 happens-before 元数据

因为它根本不是从“完整证明所有同步顺序”出发设计的，而是从“尽量低成本地观察真实冲突”出发。

官方文档 `Documentation/dev-tools/kcsan.rst` 的 Implementation Details 里讲得很清楚：KCSAN 选择 soft watchpoint 而不是完整 shadow memory/happens-before 方案，换来的是：

- 内存占用很小，只需要少量 watchpoint 槽位。
- fast path 不需要拿大锁。
- 对内核这种拥有大量自定义同步原语的复杂环境更容易落地。

代价是：

- 它是采样式的，可能漏报，属于 false negative 可能存在的动态检测。
- 对内存模型的理解不是完整的，只覆盖一部分 LKMM 规则。

换句话说，KCSAN 追求的是“足够实用的高性价比”，而不是“理论上最完备的全量检测”。

#### 2.8 KCSAN 与 lockdep 的区别

两者虽然都属于“并发错误自动检测机制”，但关注对象完全不同：

- `lockdep` 观察的是“拿锁顺序”和“上下文中的锁语义”，核心数据结构是锁依赖图，核心算法是加边前判环。
- `KCSAN` 观察的是“共享内存访问是否并发冲突”，核心数据结构是访问上下文和 watchpoint 数组，核心方法是采样、延迟和冲突匹配。

如果把并发 bug 粗分成两类：

- 一类是“大家都不往前走了”，例如死锁、锁顺序反转，这类更适合 lockdep。
- 一类是“大家都在跑，但共享数据被悄悄改坏了”，例如 data race、缺失同步，这类更适合 KCSAN。

### 3. 总结

Linux 内核的这两套机制体现了非常典型的工程思路：不是试图用一个万能工具解决所有并发问题，而是针对不同错误模型分别设计检测器。

- `lockdep` 面向锁协议错误。它把“持锁再取锁”的行为抽象成图上的边，并在每次新增依赖时做判环，因此可以在真正死锁前就报出潜在环形等待。
- `KCSAN` 面向数据竞争。它通过编译器插桩、抽样设置 soft watchpoint、故意延迟线程和比较访问结果，在较低开销下高概率抓住真实冲突。

从课程角度看，这两者刚好对应了并发里的两种核心分析视角：

- 一个是“锁顺序是否一致”，本质上是图论问题；
- 一个是“共享内存访问是否被同步”，本质上是内存访问冲突检测问题。

这也是为什么 Linux 内核需要同时拥有 lockdep 和 KCSAN：前者抓不住无锁数据竞争，后者也不能替代锁依赖图分析。二者结合起来，才构成了内核中较完整的并发错误运行时防线。

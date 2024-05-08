# openEuler系统环境实验

**openEuler System Environment Experiments**



## I. 什么是系统环境实验

所谓**系统环境**，是指运行在一定硬件系统之上的操作系统所提供的软件运行环境，在本实验中特指运行于**鲲鹏处理器**之上的**openEuler操作系统 **向上层软件提供的运行环境。

窥探并研究系统环境的实验即为**系统环境实验**。从广泛的角度而言，内核编译、内核模块编程、系统编程（System Programming）都为系统环境实验提供了案例，但为初学者简单起见，本文探讨如下几个实验：

- 通过Linux shell命令查看系统信息
- 观察C语言char数据类型
- 体验aarch64架构精简指令集
- C代码中的汇编语句
- 纯汇编程序

💡 *<u>小知识</u>*

- **鲲鹏处理器**是华为基于ARMv8架构开发的通用处理器。作为一款现代处理器，在芯片内部架构中，鲲鹏处理器涉及到了体系结构中的SoC、Chip、DIE、Cluster以及Core等概念，从而组成了一个复杂的综合体。

- **openEuler**既是一个操作系统，又是一个开源、免费的Linux发行平台，其致力于打造中国原生开源、可自主演进操作系统根社区。作为操作系统，当前openEuler内核源于[Linux](https://www.kernel.org/)，支持鲲鹏及其他多种处理器。

🔗 *<u>学习资源</u>*

- [《openEuler操作系统》（第二版）](http://www.tup.tsinghua.edu.cn/booksCenter/book_09568001.html)“第2章 鲲鹏处理器”

- [openEuler门户网站](https://www.openeuler.org/zh/)
- [openEuler代码仓](https://gitee.com/openeuler/)
- [openEuler软件包仓](https://gitee.com/src-openeuler/)
- [高校子站](https://edu.hicomputing.huawei.com/)
- [鲲鹏生态官网](https://www.hikunpeng.com/)



## II. 实验设备与流程

```mermaid
flowchart TD
    Start([Start])
    Start --> A1
    Start --> A2
    Start --> A3
    A1[鲲鹏物理机] --> B{准备好
    以上实验环境
    之一？}
    A2[鲲鹏云ECS] --> B
    A3[鲲鹏开发板]:::ActiveClass --> B
    B -- Yes --> C[openEuler
    系统环境实验]:::ActiveClass
    C --> End([End])
    B -- No ----> End
    
    classDef ActiveClass fill:#f96
    
    
```

## 一、实验流程

如上图所示，实验流程如下：

1. 准备好一个“鲲鹏平台+openEuler操作系统”的实验环境
2. 进行实验

## 二、实验设备

在这里我们有3种鲲鹏平台可供选择：

- 鲲鹏物理机：包括基于鲲鹏处理器的PC机、服务器（如泰山2280物理服务器）等。
- 鲲鹏云ECS：即基于鲲鹏处理器的华为云ECS（弹性云服务器，Elastic Cloud Server）。
- 鲲鹏开发板：类似于树莓派但是以鲲鹏处理器为CPU的开发板。

本文以“**鲲鹏开发板**”为例进行讲解。

## 三、如何登录系统

可以有以下3种方式登录到系统：

- 通过本地终端登录系统，这里的本地终端包括键盘、鼠标（物理输入设备）和显示器（物理输出设备）
- 利用PC机中的Console命令行终端通过串口登录系统
- 利用PC机中的Console命令行终端通过以太网+TCP/IP+SSH登录系统（用`ssh`命令）



## III. openEuler系统环境实验

### 一、查看系统信息

本实验通过Linux shell命令查看系统信息以增进对鲲鹏处理器和openEuler操作系统的了解。

#### 1. 查看系统架构信息

🤚*<u>动手操作</u>*

```shell
uname -m # aarch64, i.e. arm64
uname -r
uname -a # ...... aarch64 GNU/Linux
```

以上一系列`uname`命令分别查看了系统的CPU架构、操作系统内核版本及其他信息。

⚛*<u>进一步研究</u>*

试用“`uname --help`”命令探索更多选项。

#### 2. 查看CPU信息

🤚*<u>动手操作</u>*

```shell
lscpu # ......
```

❓*<u>回答问题</u>*

试着说出上述命令执行结果所代表的含义。

⚛*<u>进一步研究</u>*

以下是在运行于泰山2280物理服务器（搭载鲲鹏920处理器）的openEuler操作系统中执行该命令的结果：

```
Architecture:                    aarch64
CPU op-mode(s):                  64-bit
Byte Order:                      Little Endian
CPU(s):                          128
On-line CPU(s) list:             0-127
Thread(s) per core:              1
Core(s) per socket:              64
Socket(s):                       2
NUMA node(s):                    4
Vendor ID:                       HiSilicon
Model:                           0
Model name:                      Kunpeng-920
Stepping:                        0x1
CPU max MHz:                     2600.0000
CPU min MHz:                     200.0000
BogoMIPS:                        200.00
L1d cache:                       8 MiB
L1i cache:                       8 MiB
L2 cache:                        64 MiB
L3 cache:                        256 MiB
NUMA node0 CPU(s):               0-31
NUMA node1 CPU(s):               32-63
NUMA node2 CPU(s):               64-95
NUMA node3 CPU(s):               96-127
Vulnerability Itlb multihit:     Not affected
Vulnerability L1tf:              Not affected
Vulnerability Mds:               Not affected
Vulnerability Meltdown:          Not affected
Vulnerability Spec store bypass: Vulnerable
Vulnerability Spectre v1:        Mitigation; __user pointer sanitization
Vulnerability Spectre v2:        Not affected
Vulnerability Tsx async abort:   Not affected
Flags:                           fp asimd evtstrm aes pmull sha1 sha2 crc32 atomics fphp asimdhp cpuid asimdrdm jscvt fcma dcpop asimddp asimdfhm
```

这里以L1、L2级缓存大小为例分析一下上面的数据。我们知道鲲鹏920处理器有L1、L2、L3三级cache，其中L1的指令cache（L1I）和数据cache（L1D）大小都是64KiB，L2 cache不区分指令或数据，大小为512KiB，L1和L2两级cache由各个CPU core独享。一颗鲲鹏920芯片包含64个core，故：
- L1d cache：64Kib x 64 = 4096Kib 即4MiB
- L1i cache：64Kib x 64 = 4096Kib 即4MiB
- L2 cache：512Kib x 128 = 32768Kib 即32MiB

在泰山2280 V2服务器有2个CPU socket，4个NUMA节点，总核数达到128个，故L1d/L1i cache大小分别是8MiB，L2 cache大小是64MiB。

#### 3. 查看操作系统版本

🤚*<u>动手操作</u>*

```shell
cat /etc/os-release # ......
```

❓*<u>回答问题</u>*

你能根据命令显示结果说出openEuler的版本号吗？

### 二、观察C语言char数据类型

C是一种“强类型、弱检查”的系统级通用编程语言，我们通过对比运行在aarch64和x64架构上C语言char默认数据类型的不同进一步了解硬/软件系统某一个小小环节的差异便可使上层应用产生截然不同的结果，从而从见微知著之处体会鲲鹏平台与其他平台的不同。

🔔*<u>说明</u>*

本节实验所用源代码在“`code`”目录中。

### 1. 检查GCC版本

🤚*<u>动手操作</u>*

```shell
gcc --version # ......
```

推荐使用gcc7.3.0及以上版本（不低于4.8.5）。

### 2. 准备C程序源代码

按以下内容准备C程序源代码：

`code/ch.c`

### 3. 编译并执行

在鲲鹏开发板上编译并执行：

🤚*<u>动手操作</u>*

```shell
gcc ch.c
./a.out
```

以下是在鲲鹏平台上的执行结果：

```
sizeof ch is 1, 1

         char ch = ff, +255, positive

  signed char ch = ff,   -1, negative
unsigned char ch = ff, +255, positive
```

可以看出：

- 由于计算机中的整数用补码表示的原因，`-1`在内存中的数值是`0xff`
- 在鲲鹏平台上`char`的默认数据类型与`unsigned char`同

### 4. 在x64平台上进行同样的实验

🤲*<u>与x64平台对比</u>*

以下是在x64平台上的执行结果：

```
sizeof ch is 1, 1

         char ch = ff,   -1, negative

  signed char ch = ff,   -1, negative
unsigned char ch = ff, +255, positive
```

可见，在x64平台上，`char`的默认数据类型与`signed char`同。

⚛*<u>进一步研究</u>*

其实，C语言标准并没有规定`char`应该是`unsigned char`还是`signed char`：

> “The C standards do say that "char" may either be a "signed char" or "unsigned char" and it is up to the compilers implementation or the platform which is followed.”

所以，在C语言中，`char`类型是不是就相当于`unsigned char`和`signed char`的泛型，仅仅代表一个8bit位数的集合，至于这个集合的具体含义（是`unsigned char`还是`signed char`），那是和C编译器与CPU指令集都有关的，可以看出：**信息就是上下文**。

❓*<u>回答问题</u>*

你可以举出另外一些“信息就是上下文”的例子吗？

### 5. 编译时指定char的类型

🤚*<u>动手操作</u>*

```shell
gcc -fsigned-char ch.c
./a.out
```

看看这一次的结果是怎样的。

❓*<u>回答问题</u>*

在软件行业有一个灵丹妙药，即“**再加一层**”，这里gcc编译时所带参数-f算不算是加了一层？

💡 *<u>小知识</u>*

> “All problems in computer science can be solved by another level of indirection, except for the problem of too many layers of indirection.”
> “计算机科学中的所有问题都可以通过另外一个分层来解决，除了太多间接分层的问题外。”
> ——Butler Lampson 1992年图灵奖获奖报告（其本人认为这句著名格言最早出自于David Wheeler）



### 三、体验aarch64架构精简指令集

由于鲲鹏处理器是基于aarch64架构的，故其采用的是精简指令集。

### 1. 准备源代码

按照以下文件准备源代码：

`code/abc.c`

### 2. 编译然后反汇编

编译源文件并进行反汇编：

🤚*<u>动手操作</u>*

```shell
gcc -g abc.c
objdump -S ./a.out
```

以下是在鲲鹏开发板（RISC）上的反汇编结果：

```

./a.out：     文件格式 elf64-littleaarch64


Disassembly of section .init:

00000000004004d0 <_init>:
  4004d0:       d503201f        nop
  4004d4:       a9bf7bfd        stp     x29, x30, [sp, #-16]!
  4004d8:       910003fd        mov     x29, sp
  4004dc:       9400003a        bl      4005c4 <call_weak_fn>
  4004e0:       a8c17bfd        ldp     x29, x30, [sp], #16
  4004e4:       d65f03c0        ret

Disassembly of section .plt:

00000000004004f0 <.plt>:
  4004f0:       a9bf7bf0        stp     x16, x30, [sp, #-16]!
  4004f4:       f00000f0        adrp    x16, 41f000 <__FRAME_END__+0x1e808>
  4004f8:       f947fe11        ldr     x17, [x16, #4088]
  4004fc:       913fe210        add     x16, x16, #0xff8
  400500:       d61f0220        br      x17
  400504:       d503201f        nop
  400508:       d503201f        nop
  40050c:       d503201f        nop

0000000000400510 <__libc_start_main@plt>:
  400510:       90000110        adrp    x16, 420000 <__libc_start_main@GLIBC_2.34>
  400514:       f9400211        ldr     x17, [x16]
  400518:       91000210        add     x16, x16, #0x0
  40051c:       d61f0220        br      x17

0000000000400520 <__gmon_start__@plt>:
  400520:       90000110        adrp    x16, 420000 <__libc_start_main@GLIBC_2.34>
  400524:       f9400611        ldr     x17, [x16, #8]
  400528:       91002210        add     x16, x16, #0x8
  40052c:       d61f0220        br      x17

0000000000400530 <abort@plt>:
  400530:       90000110        adrp    x16, 420000 <__libc_start_main@GLIBC_2.34>
  400534:       f9400a11        ldr     x17, [x16, #16]
  400538:       91004210        add     x16, x16, #0x10
  40053c:       d61f0220        br      x17

0000000000400540 <printf@plt>:
  400540:       90000110        adrp    x16, 420000 <__libc_start_main@GLIBC_2.34>
  400544:       f9400e11        ldr     x17, [x16, #24]
  400548:       91006210        add     x16, x16, #0x18
  40054c:       d61f0220        br      x17

Disassembly of section .text:

0000000000400580 <_start>:
  400580:       d503201f        nop
  400584:       d280001d        mov     x29, #0x0                       // #0
  400588:       d280001e        mov     x30, #0x0                       // #0
  40058c:       aa0003e5        mov     x5, x0
  400590:       f94003e1        ldr     x1, [sp]
  400594:       910023e2        add     x2, sp, #0x8
  400598:       910003e6        mov     x6, sp
  40059c:       90000000        adrp    x0, 400000 <__abi_tag-0x278>
  4005a0:       9116d000        add     x0, x0, #0x5b4
  4005a4:       d2800003        mov     x3, #0x0                        // #0
  4005a8:       d2800004        mov     x4, #0x0                        // #0
  4005ac:       97ffffd9        bl      400510 <__libc_start_main@plt>
  4005b0:       97ffffe0        bl      400530 <abort@plt>

00000000004005b4 <__wrap_main>:
  4005b4:       d503201f        nop
  4005b8:       14000033        b       400684 <main>
  4005bc:       d503201f        nop

00000000004005c0 <_dl_relocate_static_pie>:
  4005c0:       d65f03c0        ret

00000000004005c4 <call_weak_fn>:
  4005c4:       f00000e0        adrp    x0, 41f000 <__FRAME_END__+0x1e808>
  4005c8:       f947ec00        ldr     x0, [x0, #4056]
  4005cc:       b4000040        cbz     x0, 4005d4 <call_weak_fn+0x10>
  4005d0:       17ffffd4        b       400520 <__gmon_start__@plt>
  4005d4:       d65f03c0        ret
  4005d8:       d503201f        nop
  4005dc:       d503201f        nop

00000000004005e0 <deregister_tm_clones>:
  4005e0:       90000100        adrp    x0, 420000 <__libc_start_main@GLIBC_2.34>
  4005e4:       9100c000        add     x0, x0, #0x30
  4005e8:       90000101        adrp    x1, 420000 <__libc_start_main@GLIBC_2.34>
  4005ec:       9100c021        add     x1, x1, #0x30
  4005f0:       eb00003f        cmp     x1, x0
  4005f4:       540000c0        b.eq    40060c <deregister_tm_clones+0x2c>  // b.none
  4005f8:       f00000e1        adrp    x1, 41f000 <__FRAME_END__+0x1e808>
  4005fc:       f947e821        ldr     x1, [x1, #4048]
  400600:       b4000061        cbz     x1, 40060c <deregister_tm_clones+0x2c>
  400604:       aa0103f0        mov     x16, x1
  400608:       d61f0200        br      x16
  40060c:       d65f03c0        ret

0000000000400610 <register_tm_clones>:
  400610:       90000100        adrp    x0, 420000 <__libc_start_main@GLIBC_2.34>
  400614:       9100c000        add     x0, x0, #0x30
  400618:       90000101        adrp    x1, 420000 <__libc_start_main@GLIBC_2.34>
  40061c:       9100c021        add     x1, x1, #0x30
  400620:       cb000021        sub     x1, x1, x0
  400624:       d37ffc22        lsr     x2, x1, #63
  400628:       8b810c41        add     x1, x2, x1, asr #3
  40062c:       9341fc21        asr     x1, x1, #1
  400630:       b40000c1        cbz     x1, 400648 <register_tm_clones+0x38>
  400634:       f00000e2        adrp    x2, 41f000 <__FRAME_END__+0x1e808>
  400638:       f947f042        ldr     x2, [x2, #4064]
  40063c:       b4000062        cbz     x2, 400648 <register_tm_clones+0x38>
  400640:       aa0203f0        mov     x16, x2
  400644:       d61f0200        br      x16
  400648:       d65f03c0        ret
  40064c:       d503201f        nop

0000000000400650 <__do_global_dtors_aux>:
  400650:       a9be7bfd        stp     x29, x30, [sp, #-32]!
  400654:       910003fd        mov     x29, sp
  400658:       f9000bf3        str     x19, [sp, #16]
  40065c:       90000113        adrp    x19, 420000 <__libc_start_main@GLIBC_2.34>
  400660:       3940c260        ldrb    w0, [x19, #48]
  400664:       35000080        cbnz    w0, 400674 <__do_global_dtors_aux+0x24>
  400668:       97ffffde        bl      4005e0 <deregister_tm_clones>
  40066c:       52800020        mov     w0, #0x1                        // #1
  400670:       3900c260        strb    w0, [x19, #48]
  400674:       f9400bf3        ldr     x19, [sp, #16]
  400678:       a8c27bfd        ldp     x29, x30, [sp], #32
  40067c:       d65f03c0        ret

0000000000400680 <frame_dummy>:
  400680:       17ffffe4        b       400610 <register_tm_clones>

0000000000400684 <main>:
objdump：警告：source file /home/openEuler/code/abc.c is more recent than object file
// "gcc abc.c" to generates binary code
//

#include <stdio.h>

int main() {
  400684:       a9be7bfd        stp     x29, x30, [sp, #-32]!
  400688:       910003fd        mov     x29, sp
    int a = 1;
  40068c:       52800020        mov     w0, #0x1                        // #1
  400690:       b9001fe0        str     w0, [sp, #28]
    int b = 2;
  400694:       52800040        mov     w0, #0x2                        // #2
  400698:       b9001be0        str     w0, [sp, #24]
    int c = a + b;
  40069c:       b9401fe1        ldr     w1, [sp, #28]
  4006a0:       b9401be0        ldr     w0, [sp, #24]
  4006a4:       0b000020        add     w0, w1, w0
  4006a8:       b90017e0        str     w0, [sp, #20]
    printf("c = %d\n", c);
  4006ac:       b94017e1        ldr     w1, [sp, #20]
  4006b0:       90000000        adrp    x0, 400000 <__abi_tag-0x278>
  4006b4:       911ba000        add     x0, x0, #0x6e8
  4006b8:       97ffffa2        bl      400540 <printf@plt>

    return 0;
  4006bc:       52800000        mov     w0, #0x0                        // #0
  4006c0:       a8c27bfd        ldp     x29, x30, [sp], #32
  4006c4:       d65f03c0        ret

Disassembly of section .fini:

00000000004006c8 <_fini>:
  4006c8:       d503201f        nop
  4006cc:       a9bf7bfd        stp     x29, x30, [sp, #-16]!
  4006d0:       910003fd        mov     x29, sp
  4006d4:       a8c17bfd        ldp     x29, x30, [sp], #16
  4006d8:       d65f03c0        ret
```

### 3. 与CISC反汇编结果相比较

🤲*<u>与x64平台对比</u>*

以下是在x64平台上的反汇编结果：

```

./a.out:     file format elf64-x86-64


Disassembly of section .init:

0000000000401000 <_init>:
  401000:       f3 0f 1e fa             endbr64
  401004:       48 83 ec 08             sub    $0x8,%rsp
  401008:       48 8b 05 e1 2f 00 00    mov    0x2fe1(%rip),%rax        # 403ff0 <__gmon_start__@Base>
  40100f:       48 85 c0                test   %rax,%rax
  401012:       74 02                   je     401016 <_init+0x16>
  401014:       ff d0                   call   *%rax
  401016:       48 83 c4 08             add    $0x8,%rsp
  40101a:       c3                      ret

Disassembly of section .plt:

0000000000401020 <printf@plt-0x10>:
  401020:       ff 35 e2 2f 00 00       push   0x2fe2(%rip)        # 404008 <_GLOBAL_OFFSET_TABLE_+0x8>
  401026:       ff 25 e4 2f 00 00       jmp    *0x2fe4(%rip)        # 404010 <_GLOBAL_OFFSET_TABLE_+0x10>
  40102c:       0f 1f 40 00             nopl   0x0(%rax)

0000000000401030 <printf@plt>:
  401030:       ff 25 e2 2f 00 00       jmp    *0x2fe2(%rip)        # 404018 <printf@GLIBC_2.2.5>
  401036:       68 00 00 00 00          push   $0x0
  40103b:       e9 e0 ff ff ff          jmp    401020 <_init+0x20>

Disassembly of section .text:

0000000000401040 <_start>:
  401040:       f3 0f 1e fa             endbr64
  401044:       31 ed                   xor    %ebp,%ebp
  401046:       49 89 d1                mov    %rdx,%r9
  401049:       5e                      pop    %rsi
  40104a:       48 89 e2                mov    %rsp,%rdx
  40104d:       48 83 e4 f0             and    $0xfffffffffffffff0,%rsp
  401051:       50                      push   %rax
  401052:       54                      push   %rsp
  401053:       45 31 c0                xor    %r8d,%r8d
  401056:       31 c9                   xor    %ecx,%ecx
  401058:       48 c7 c7 22 11 40 00    mov    $0x401122,%rdi
  40105f:       ff 15 7b 2f 00 00       call   *0x2f7b(%rip)        # 403fe0 <__libc_start_main@GLIBC_2.34>
  401065:       f4                      hlt
  401066:       66 2e 0f 1f 84 00 00    cs nopw 0x0(%rax,%rax,1)
  40106d:       00 00 00

0000000000401070 <_dl_relocate_static_pie>:
  401070:       f3 0f 1e fa             endbr64
  401074:       c3                      ret
  401075:       66 2e 0f 1f 84 00 00    cs nopw 0x0(%rax,%rax,1)
  40107c:       00 00 00
  40107f:       90                      nop

0000000000401080 <deregister_tm_clones>:
  401080:       48 8d 3d a9 2f 00 00    lea    0x2fa9(%rip),%rdi        # 404030 <__TMC_END__>
  401087:       48 8d 05 a2 2f 00 00    lea    0x2fa2(%rip),%rax        # 404030 <__TMC_END__>
  40108e:       48 39 f8                cmp    %rdi,%rax
  401091:       74 15                   je     4010a8 <deregister_tm_clones+0x28>
  401093:       48 8b 05 4e 2f 00 00    mov    0x2f4e(%rip),%rax        # 403fe8 <_ITM_deregisterTMCloneTable@Base>
  40109a:       48 85 c0                test   %rax,%rax
  40109d:       74 09                   je     4010a8 <deregister_tm_clones+0x28>
  40109f:       ff e0                   jmp    *%rax
  4010a1:       0f 1f 80 00 00 00 00    nopl   0x0(%rax)
  4010a8:       c3                      ret
  4010a9:       0f 1f 80 00 00 00 00    nopl   0x0(%rax)

00000000004010b0 <register_tm_clones>:
  4010b0:       48 8d 3d 79 2f 00 00    lea    0x2f79(%rip),%rdi        # 404030 <__TMC_END__>
  4010b7:       48 8d 35 72 2f 00 00    lea    0x2f72(%rip),%rsi        # 404030 <__TMC_END__>
  4010be:       48 29 fe                sub    %rdi,%rsi
  4010c1:       48 89 f0                mov    %rsi,%rax
  4010c4:       48 c1 ee 3f             shr    $0x3f,%rsi
  4010c8:       48 c1 f8 03             sar    $0x3,%rax
  4010cc:       48 01 c6                add    %rax,%rsi
  4010cf:       48 d1 fe                sar    %rsi
  4010d2:       74 14                   je     4010e8 <register_tm_clones+0x38>
  4010d4:       48 8b 05 1d 2f 00 00    mov    0x2f1d(%rip),%rax        # 403ff8 <_ITM_registerTMCloneTable@Base>
  4010db:       48 85 c0                test   %rax,%rax
  4010de:       74 08                   je     4010e8 <register_tm_clones+0x38>
  4010e0:       ff e0                   jmp    *%rax
  4010e2:       66 0f 1f 44 00 00       nopw   0x0(%rax,%rax,1)
  4010e8:       c3                      ret
  4010e9:       0f 1f 80 00 00 00 00    nopl   0x0(%rax)

00000000004010f0 <__do_global_dtors_aux>:
  4010f0:       80 3d 39 2f 00 00 00    cmpb   $0x0,0x2f39(%rip)        # 404030 <__TMC_END__>
  4010f7:       75 17                   jne    401110 <__do_global_dtors_aux+0x20>
  4010f9:       55                      push   %rbp
  4010fa:       48 89 e5                mov    %rsp,%rbp
  4010fd:       e8 7e ff ff ff          call   401080 <deregister_tm_clones>
  401102:       c6 05 27 2f 00 00 01    movb   $0x1,0x2f27(%rip)        # 404030 <__TMC_END__>
  401109:       5d                      pop    %rbp
  40110a:       c3                      ret
  40110b:       0f 1f 44 00 00          nopl   0x0(%rax,%rax,1)
  401110:       c3                      ret
  401111:       66 66 2e 0f 1f 84 00    data16 cs nopw 0x0(%rax,%rax,1)
  401118:       00 00 00 00
  40111c:       0f 1f 40 00             nopl   0x0(%rax)

0000000000401120 <frame_dummy>:
  401120:       eb 8e                   jmp    4010b0 <register_tm_clones>

0000000000401122 <main>:
//

#include <stdio.h>

int main()
{
  401122:       55                      push   %rbp
  401123:       48 89 e5                mov    %rsp,%rbp
  401126:       48 83 ec 10             sub    $0x10,%rsp
    int a = 1;
  40112a:       c7 45 fc 01 00 00 00    movl   $0x1,-0x4(%rbp)
    int b = 2;
  401131:       c7 45 f8 02 00 00 00    movl   $0x2,-0x8(%rbp)
    int c = a + b;
  401138:       8b 55 fc                mov    -0x4(%rbp),%edx
  40113b:       8b 45 f8                mov    -0x8(%rbp),%eax
  40113e:       01 d0                   add    %edx,%eax
  401140:       89 45 f4                mov    %eax,-0xc(%rbp)
    printf("c = %d\n", c);
  401143:       8b 45 f4                mov    -0xc(%rbp),%eax
  401146:       89 c6                   mov    %eax,%esi
  401148:       bf 04 20 40 00          mov    $0x402004,%edi
  40114d:       b8 00 00 00 00          mov    $0x0,%eax
  401152:       e8 d9 fe ff ff          call   401030 <printf@plt>

    return 0;
  401157:       b8 00 00 00 00          mov    $0x0,%eax
  40115c:       c9                      leave
  40115d:       c3                      ret

Disassembly of section .fini:

0000000000401160 <_fini>:
  401160:       f3 0f 1e fa             endbr64
  401164:       48 83 ec 08             sub    $0x8,%rsp
  401168:       48 83 c4 08             add    $0x8,%rsp
  40116c:       c3                      ret
```

❓*<u>回答问题</u>*

观察两者的机器码，你能说出精简指令集和复杂指令集各自的特点吗？

## 四、C代码中的汇编语句

### 1. 准备源代码

按以下文件内容准备源代码：

`code/add.c`

### 2. 编译并执行

🤚*<u>动手操作</u>*

```shell
gcc -o add add.c
./add 2 3 # Desire 5
```

⚛*<u>进一步研究</u>*

试着将代码中32位的寄存器Wn换成64位的寄存器Xn再重新编译运行一遍。

## 五、纯汇编程序

### 1. 准备汇编代码

按以下文件内容准备汇编代码：

`code/hello.s`

### 2. 编译、链接

🤚*<u>动手操作</u>*

```shell
as -o hello.o hello.s
ld -o hello hello.o
```

### 3. 执行

🤚*<u>动手操作</u>*

```shell
./hello # Hello openEuler!
```

⚛*<u>进一步研究</u>*

试着弄懂上述程序中汇编代码的含义。



# IV. 术语表

本实验中所用术语列表如下：

- PC
  - Personal Computer，个人电脑
- ECS
  - Elastic Cloud Server，弹性云服务器
- SoC
  - System on Chip，片上系统
- Chip
  - 通常指一块物理芯片
- DIE
  - 芯片的最小物理单元
- Cluster
  - 族，若干个core的集合
- Core
  - 核，真正负责做计算的单元
- RISC
  - Reduced Instruction Set Computer，精简指令集计算机
- CISC
  - Complex InstrucTIon Set Computer，复杂指令集计算机
- NUMA
  - Non-Uniform Memory Access，非统一内存访问
- SSH
  - Secure Shell，安全外壳。一种网络安全协议，通过加密和认证机制实现安全的访问和文件传输等业务。



# openEuler内核模块编程

**openEuler Kernel Module Programming**

内核模块是一种可以扩展运行时内核功能的目标文件（Object File）。大多数类Unix及Windows系统均使用模块，这种机制使得允许内核在运行过程中动态地插入或者移除代码。

本实验通过在鲲鹏平台上编译、安装openEuler操作系统新内核以及实现简单的内核模块编程任务带领大家了解操作系统及其内核模块编程。

🏷 <u>*术语*</u>

鲲鹏云是“华为云鲲鹏云”的简称，本实验的鲲鹏平台是鲲鹏云ECS（也称鲲鹏弹性云服务器，Kunpeng Elastic Cloud Server，KECS）。

🔒🗝 <u>*何时使用root身份*</u>

为系统安全起见，进行应用编程实验时，我们一般以非root用户登录，但在进行内核编译、安装和内核模块编程实验时，为方便起见，我们以root身份登录。



## I 内核编译

### 一、内核编译与安装

#### 1. 准备环境

准备好编译内核的环境：

```shell
yum group install -y "Development Tools"
yum install -y bc
yum install -y openssl-devel
```
注意：如果在安装的过程中出现“Error: GPG check FAILED”错误则可在`yum install`命令后增加`--nogpgcheck`选项，例如：
```shell
yum group install -y "Development Tools" --nogpgcheck
```

#### 2. 备份

备份boot目录以防后续步骤更新内核失败：

```shell
tar czvf boot.origin.tgz /boot/
uname -r > uname_r.log # 保存当前内核版本信息
```

#### 3. 获取源代码

获取内核源代码并解压：

```shell
wget https://gitee.com/openeuler/kernel/repository/archive/openEuler-20.03-LTS-SP3.zip
unzip openEuler-20.03-LTS-SP3.zip # kernel-openEuler-20.03-LTS-SP3
mv kernel-openEuler-20.03-LTS-SP3 kernel
```

🔔 <u>*注意*</u>

您需要在[openEuler内核代码仓](https://gitee.com/openeuler/kernel)获取到正确的内核代码URL地址并更新其源代码包的具体文件名称。

解压后的内核源码文件夹名称可能和“*kernel-openEuler-20.03-LTS-SP3*”不一样，我们仅以此为例。

#### 4. 编译内核

先进入内核目录查看内核可以编译成为何种类型的文件：

```shell
cd kernel
make help | grep Image
```

这里输出结果为：

```
* Image.gz      - Compressed kernel image (arch/arm64/boot/Image.gz)
  Image         - Uncompressed kernel image (arch/arm64/boot/Image)
```

表示内核可编译为Image类型的文件。

配置内核：

```shell
make openeuler_defconfig
```

在这里，我们按源代码文件`kernel/arch/arm64/configs/openeuler_defconfig`配置了内核（会产生一个默认名称为`.config`的配置文件）。

```shell
echo $(nproc) # For instance: 4
make -j$(nproc) Image
make -j$(nproc) modules
make -j1 dtbs
```

上面的命令是以多线程编译内核和内核模块。为简单起见，我们将线程数设置成了核数。然后，又以单线程编译了dtbs。那么，这里的`make dtbs`是什么意思呢？下面是一个参考回答：

> 在Linux内核编译的上下文中，make dtbs 通常用于构建设备树源文件（Device Tree Source files）或设备树二进制文件（Device Tree Blobs, DTBs）。设备树（Device Tree）是一种数据结构，它描述了非平台特定的硬件细节，允许操作系统内核为给定硬件板卡上的设备提供合适的驱动程序和配置。
>
> 具体来说，make dtbs 命令会执行内核源代码树中定义的规则，这些规则用于从设备树源文件（.dts 文件）生成二进制设备树（.dtb 文件）。这些 .dtb 文件随后可以被引导加载程序（如U-Boot）加载到内核，以便内核在启动时可以知道哪些设备存在以及它们的配置。
>
> 在较新的内核版本中，你可能还会看到 make dtbs_install 这样的命令，它通常用于将生成的 .dtb 文件安装到某个特定的目录（如 /boot），以便在引导时可以被加载。
>
> 注意：不是所有的内核配置都需要设备树，这通常取决于你的硬件平台和内核配置。例如，传统的PC硬件通常不需要设备树，但许多嵌入式系统和ARM板卡确实需要。
>
> 如果你正在编译一个需要设备树的内核，并且你的内核源代码树中包含了相应的 .dts 文件，那么运行 make dtbs（或类似的命令）就是必要的步骤之一。

#### 5. 安装内核

安装内核模块：

```shell
make modules_install
```

会有类似以下的输出：

```
......
INSTALL sound/soundcore.ko
DEPMOD  4.19.90
```

安装内核：

```shell
make install
```

最后的输出类似这样：

```
/bin/sh ./arch/arm64/boot/install.sh 4.19.90 \
arch/arm64/boot/Image System.map "/boot"
dracut-install: Failed to find module 'xen-blkfront'
dracut: FAILED:  /usr/lib/dracut/dracut-install -D /var/tmp/dracut.tlIdPu/initramfs --kerneldir /lib/modules/4.19.90/ -m virtio_gpu xen-blkfront xen-netfront virtio_blk virtio_scsi virtio_net virtio_pci virtio_ring virtio
```

注意：这里出现的“dracut: FAILED”错误可以忽略之。

#### 6. 重启系统并以新的内核引导

通常，我们需要借助VNC（Virtual Network Computing）窗口在GRUB菜单显示时用上下光标键选择新编译出来的内核来启动系统。或者，对于本实验而言，也可以使用以下方法：

首先查看新编译出来的内核GRUB菜单：
```shell
cat /etc/grub2-efi.cfg
```

我们应该可以找到类似这样的菜单项：
```
menuentry 'openEuler (4.19.90) 20.03 (LTS)'
```
这就是本例中我们新编译出来的内核版本菜单项（请留意前述编译内核时的一些命令行输出信息）。这时我们可以对默认启动项进行设置：
```shell
grub2-set-default 'openEuler (4.19.90) 20.03 (LTS)'
```

确认：
```shell
grub2-editenv list
```

应该类似如下所示的输出：
```
saved_entry=openEuler (4.19.90) 20.03 (LTS)
boot_success=0
```

随后重启系统：
```shell
reboot
```

登录并验证：
```shell
uname -r # Desire 4.19.90
```
可以看出内核版本已更新。我们可以通过如下命令查看新内核的一些信息：
```shell
ls /lib/modules/`uname -r`
```

🔔 <u>*注意*</u>

您编译出来的新内核版本通常会与此不同。


### 二、验证：Hello, world!

#### 1. 准备源代码

正确编写满足功能的源文件，包括.c源文件和Makefile文件。在这里请参照示例源文件hello_world.c及其Makefile（您可以用scp命令将它们上传到ECS。）假设您的源文件在`hello-world`目录下：
```shell
cd hello-world
```

#### 2. 编译源文件

```shell
make
```

#### 3. 加载内核模块

加载编译完成的内核模块，并查看加载结果：

```shell
insmod hello_world.ko guy="Dinu" year=2013
lsmod | grep hello_world
# For instance: 
# hello_world            262144  0
```

#### 4. 卸载内核模块
卸载内核模块，并查看结果：

```shell
rmmod hello_world
dmesg | tail -n5
```

输出结果类似如下所示：

```
[   55.787283] hello_world: loading out-of-tree module taints kernel.
[   55.787798] hello_world: module verification failed: signature and/or required key missing - tainting kernel
[   55.788937] Init module.
[   55.789115] Hello, Dinu, 2013!
[   55.789283] Exit module.
```

如果您打开了VNC窗口，那么它相当于虚拟机的屏幕，因此您在VNC窗口中也会看到程序输出结果。

🔔 <u>*注意*</u>

- 第一次装在hello_world模块时，请忽略掉最开始安装模块时出现的两行错误提示信息。

- 以上步骤输出的进程号以及时间戳可能和您实际的情况不一致。

#### 5. 清理

```shell
make clean
```

#### 6. 关机

实验做完后可以关闭虚拟机：

```shell
poweroff
```

🔔 <u>*注意*</u>

如果是按需购买的ECS，实验做完后更要关机以节约云资源（这时仍有扣费，除非完全删除云资源）。

♾ <u>*从具体到一般*</u>

我们可以从这个例子中窥探到Linux内核模块的基本结构。

Linux的内核模块（Module）是一种具有独立功能的程序，它可以被单独编译，但是只能被链接到内核作为内核的一部分在内核空间中运行。模块本身不被编译入内核映像，从而避免使得内核过大，而其一旦被加载，其地位就和内核中的其它部分一样。

现在的内核模块一般具有如下形式的基本结构：

```C
#include <linux/module.h> //包含了对模块的结构定义

static __init my_module_init(void) {
	//加载模块
}

static __exit my_module_exit(void) {
	//卸载模块
}

module_init(my_module_init); //指定初始化函数
module_exit(my_module_exit); //指定卸载函数

MODULE_LICENSE("GPL"); //声明版权
```




## II 两个模块的内核编程

🔔 <u>*说明*</u>

这里简单编写了两个内核模块作为示例，更深入的内核模块编程请参考其他相关资料。

这两个模块分别实现以下功能：

- 模块1：支持数组参数，加载时读入并打印。
- 模块2：在/proc目录下创建只读文件。

需要注意以下问题：

- 模块可以写在一个C文件中，模块参数传递参考宏定义`module_param(name, type, perm)`，需要用到头文件linux/moduleparam.h。
- 编写Makefile文件将C源码编译成.ko模块。

🎵 <u>*准备代码*</u>

请参考所附源代码文件（module_1.c、module_2.c以及Makefile）准备您的文件。

🤚 <u>*动手操作*</u>

请参考命令行文件cmdline.sh进行实验。



## 附录A 在x64平台上编译内核

除了鲲鹏平台，openEuler操作系统也支持x86_64、RISC-V等架构。兹列出在x86_64平台上的openEuler编译内核的步骤以供参考。

### 一、内核编译与安装

#### 1. 查看系统信息

先查看当前的系统信息：

```shell
uname -m # Desire x86_64
uname -r # For instance: 4.19.90-2003.4.0.0036.oe1.x86_64
```

注意此例当前所用内核版本为4.19.XX-XXXX.X.X.XXXX.XXX.x86_64。

```shell
cat /etc/os-release
```

会有类似以下的输出信息：

```
NAME="openEuler"
VERSION="20.03 (LTS)"
ID="openEuler"
VERSION_ID="20.03"
PRETTY_NAME="openEuler 20.03 (LTS)"
ANSI_COLOR="0;31"
```

这表明当前是公元2020年3月发布的长期支持版openEuler操作系统。

🔔 <u>*注意*</u>

您的系统的版本号可能与此不同。

```shell
lscpu | grep '^CPU(s):'
```

输出如下表示本例所用CPU为4核（您的系统可能与此不同）：

```
CPU(s):                          4
```

也可以用以下命令查看CPU核数：

```shell
cat /proc/cpuinfo | grep "processor" | wc -l
grep processor /proc/cpuinfo | wc -l
echo $(nproc) # For instance: 4
```

#### 2. 准备环境

准备编译内核的软件环境：

```shell
yum group install -y "Development Tools"
yum install -y bc
yum install -y openssl-devel
yum install -y elfutils-libelf-devel
```

#### 3. 获取源码

获取内核源代码（假设把它放在Home目录下）：

```shell
cd ~
wget https://gitee.com/openeuler/kernel/repository/archive/kernel-4.19.zip
unzip kernel-4.19.zip
ls # For instance: kernel-4.19.zip  kernel-kernel-4.19
mv kernel-kernel-4.19/ kernel/
```

#### 4. 编译、安装内核

先进入内核目录查看内核可以编译成为何种类型的文件：

```shell
cd kernel
make help | grep Image
```

如果显示如下则表示内核可编译为bzImage类型的文件：

```
* bzImage      - Compressed kernel image (arch/x86/boot/bzImage)
```

产生配置文件：

```shell
make openeuler_defconfig
```

以下是一些可能的输出信息示例：

```
arch/x86/configs/openeuler_defconfig:2534:warning: override: reassigning to symbol HINIC
#
# configuration written to .config
#
```

编译内核：

```shell
make -j$(nproc) bzImage
```

这里以多线程编译内核，我们简单地将线程数设置成核数。编译完成通常会输出：

```
Kernel: arch/x86/boot/bzImage is ready  (#1)
```

编译内核模块：

```shell
make -j$(nproc) modules
```

安装内核模块：

```shell
make modules_install # For instance: DEPMOD  4.19.208
```

安装内核：

```shell
make install
```

在本例中有以下输出：

```
sh ./arch/x86/boot/install.sh 4.19.208 arch/x86/boot/bzImage \
        System.map "/boot"
dracut-install: ERROR: installing 'virtio_pci'
dracut: FAILED:  /usr/lib/dracut/dracut-install -D /var/tmp/dracut.R4CHaO/initramfs --kerneldir /lib/modules/4.19.208/ -m virtio_gpu xen-blkfront xen-netfront virtio_blk virtio_scsi virtio_net virtio_pci virtio_ring virtio
```

注意：这里的“dracut: FAILED”错误请忽略。

#### 5. 重启系统

```shell
reboot
```

重启并在GRUB菜单中选择以新的内核来引导操作系统，登录系统的shell然后检查内核版本：

```shell
uname -r # For instance: 4.19.208
```

这和原来的内核版本号不同，表明确实以新版本的内核启动了系统。

🔔 <u>*注意*</u>

您编译出来的新内核版本通常会与此不同。


### 二、验证

从略。



## 附录B 术语表

本实验中所用术语列表如下：

🏷<u>*术语表*<u>

- KMP
  - Kernel Module Programming，内核模块编程。
- KECS
  - Kunpeng Elastic Cloud Server，鲲鹏弹性云服务器
- VNC
  - Virtual Network Console，虚拟网络控制台
- GRUB
  - Grand Unified Boot Loader，大一统引导装载程序


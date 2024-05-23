# openEuler内核模块编程实验

**openEuler Kernel Module Programming Experiments**

内核模块是一种可以扩展运行时内核功能的目标文件（Object File）。大多数类Unix及Windows系统均使用模块，这种机制使得允许内核在运行过程中动态地插入或者移除代码。

本实验通过在鲲鹏平台上编译、安装openEuler操作系统新内核以及实现简单的内核模块编程任务带领大家了解操作系统及其内核模块编程。

🏷 <u>*术语*</u>

鲲鹏云是“华为云鲲鹏云”的简称，本实验的鲲鹏平台是鲲鹏云ECS（也称鲲鹏弹性云服务器，Kunpeng Elastic Cloud Server，KECS）。

🔒🗝 <u>*何时使用root身份*</u>

为系统安全起见，进行应用编程实验时，我们一般以非root用户登录，但在进行内核编译、安装和内核模块编程实验时，为方便起见，我们以root身份登录。



## I. 内核的编译与安装

#### 1. 准备环境

准备好编译内核的环境：

```shell
yum group install -y "Development Tools"
yum install -y bc
yum install -y openssl-devel
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
cd kernel
```

🔔 <u>*注意*</u>

您需要在openEuler代码仓（https://gitee.com/openeuler/kernel）获取到正确的内核代码URL地址并更新其源代码包的具体文件名称。

解压后的内核源码文件夹名称可能和“*kernel-openEuler-20.03-LTS-SP3*”不一样，我们仅以此为例。

#### 4. 编译内核

```shell
make openeuler_defconfig
```

在这里，我们按源代码文件`kernel/arch/arm64/configs/openeuler_defconfig`配置内核。

```shell
make help | grep Image
```

输出结果为：

```
* Image.gz      - Compressed kernel image (arch/arm64/boot/Image.gz)
  Image         - Uncompressed kernel image (arch/arm64/boot/Image)
```

这表示内核可编译为Image文件。

```shell
echo $(nproc) # For instance: 4
make -j$(nproc) Image modules dtbs
```

上面的命令是以多线程编译内核的Image、modules和dtbs。为简单起见，我们将线程数设置成了核数。

#### 5. 安装内核

安装内核模块：

```shell
make modules_install
```

会有类似以下的输出：

```
......
INSTALL sound/soundcore.ko
DEPMOD  4.19.154
```

安装内核：

```shell
make install
```

最后的输出类似这样：

```
/bin/sh ./arch/arm64/boot/install.sh 4.19.154 \
arch/arm64/boot/Image System.map "/boot"
dracut-install: Failed to find module 'xen-blkfront'
dracut: FAILED:  /usr/lib/dracut/dracut-install -D /var/tmp/dracut.tlIdPu/initramfs --kerneldir /lib/modules/4.19.154/ -m virtio_gpu xen-blkfront xen-netfront virtio_blk virtio_scsi virtio_net virtio_pci virtio_ring virtio
```

注意：这里出现的“dracut: FAILED”错误可以忽略之。

#### 6. 重启系统

```shell
reboot
```

然后我们需要借助VNC窗口在GRUB菜单显示时用上下光标键选择新编译出来的内核来启动系统。

我们以新的内核引导系统后，可以继续在VNC窗口中登录系统，也可以以ssh终端登录，然后在并在shell中输入如下命令查看内核版本：

```shell
uname -r # For instance: 4.19.154
```

可以看出内核版本已更新。

🔔 <u>*注意*</u>

您编译出来的新内核版本通常会与此不同。



## II. Hello, world!

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

MODULE_LICENSE("GPL"); //声明版权

static __init my_module_init(void) {
	//加载模块
}

static __exit my_module_exit(void) {
	//卸载模块
}

module_init(my_module_init); //指定初始化函数
module_exit(my_module_exit); //指定卸载函数
```



# III. 术语表

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



# IV. 附：在x64平台上编译内核

除了鲲鹏平台，openEuler操作系统也支持x86_64、RISC-V等架构。兹列出在x86_64平台上的openEuler编译内核的步骤以供参考。

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

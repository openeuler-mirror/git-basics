# openEuler内核模块编程实验

**openEuler Kernel Module Programming Experiments**

内核模块是一种可以扩展运行时内核功能的目标文件（Object File）。大多数类Unix及Windows系统均使用模块，这种机制使得允许内核在运行过程中动态地插入或者移除代码。

本实验通过在鲲鹏平台上编译、安装openEuler操作系统新内核以及实现简单的内核模块编程任务带领大家了解操作系统及其内核模块编程。

🏷 <u>*术语*</u>

鲲鹏云是“华为云鲲鹏云”的简称，本实验的鲲鹏平台是鲲鹏云ECS（也称鲲鹏弹性云服务器，Kunpeng Elastic Cloud Server，KECS）。

🔒🗝 <u>*何时使用root身份*</u>

为系统安全起见，进行应用编程实验时，我们一般以非root用户登录，但在进行内核编译、安装和内核模块编程实验时，为方便起见，我们以root身份登录。



## I. 内核的编译与安装

##### 1. 准备好编译内核的环境

```shell
yum group install -y "Development Tools"
yum install -y bc
yum install -y openssl-devel
```

##### 2. 备份boot目录以防后续步骤更新内核失败

```shell
tar czvf boot.origin.tgz /boot/
uname -r > uname_r.log # 保存当前内核版本信息
```

##### 3. 获取内核源代码并解压

```shell
wget https://gitee.com/openeuler/kernel/repository/archive/openEuler-20.03-LTS-SP3.zip
unzip openEuler-20.03-LTS-SP3.zip # kernel-openEuler-20.03-LTS-SP3
mv kernel-openEuler-20.03-LTS-SP3 kernel
cd kernel
```

🔔 <u>*注意*</u>

您可能需要在openEuler代码仓（https://gitee.com/openeuler/kernel）获取到正确的内核代码URL地址并更新其源代码包的具体文件名称。

解压后的内核源码文件夹名称可能和“*kernel-openEuler-20.03-LTS-SP3*”不一样，我们仅以此为例。

##### 4. 编译内核

```shell
make openeuler_defconfig
```

在这里，我们按源代码文件`kernel/arch/arm64/configs/openeuler_defconfig`的配置配置内核。

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

上面的命令是以多线程编译内核的Image、modules和dtbs。

##### 5. 安装内核

```shell
make modules_install
```

输出可能是这样的：

```
  ......
  INSTALL sound/soundcore.ko
  DEPMOD  4.19.154
```

```shell
make install
```

输出可能是这样的：

```
/bin/sh ./arch/arm64/boot/install.sh 4.19.154 \
arch/arm64/boot/Image System.map "/boot"
dracut-install: Failed to find module 'xen-blkfront'
dracut: FAILED:  /usr/lib/dracut/dracut-install -D /var/tmp/dracut.tlIdPu/initramfs --kerneldir /lib/modules/4.19.154/ -m virtio_gpu xen-blkfront xen-netfront virtio_blk virtio_scsi virtio_net virtio_pci virtio_ring virtio
```

🔔 <u>*注意*</u>

在这里最后一步`make install`时出现的错误在这里可以忽略。

##### 6. 以新的内核重启系统

```shell
reboot
```

在这里我们需要借助VNC窗口显示系统输出并在适当的时机（即GRUB菜单显示时）选用新编译出来的内核来启动系统。

我们以新的内核引导系统后，可以继续在VNC窗口中登录系统，也可以以ssh终端登录，然后在并在shell中输入如下命令查看内核版本：

```shell
uname -r
```

一个可能的输出是：`4.19.154`，可以看出内核版本已更新。

🔔 <u>*注意*</u>

您编译出来的新内核版本通常与此不同。



## II. Hello, world!

##### 1. 准备源代码

正确编写满足功能的源文件，包括.c源文件和Makefile文件。在这里请参照示例源文件hello_world.c及其Makefile（您可以用scp命令将它们上传到ECS。）假设您的源文件在`hello-world`目录下：
```shell
cd hello-world
```

##### 2. 编译源文件

```shell
make
```

##### 3. 加载内核模块

加载编译完成的内核模块，并查看加载结果：

```shell
insmod hello_world.ko guy="Dinu" year=2013
lsmod | grep hello_world
# For instance: 
# hello_world            262144  0
```

##### 4. 卸载内核模块
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

第一次装在hello_world模块时，请忽略掉最开始安装模块时出现的两行错误提示信息。

##### 5. 清理

```shell
make clean
```

##### 6. 关机

实验做完后可以关闭虚拟机（如果是按需购买的ECS，更要这么做以节约云资源）：

```shell
poweroff
```

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


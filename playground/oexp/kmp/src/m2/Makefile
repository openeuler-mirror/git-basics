# Build modules

obj-m := module_1.o module_2.o
KDIR := /lib/modules/$(shell uname -r)/build 
PWD := $(shell pwd)
all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	rm -f *.ko *.mod *.mod.? *.o *.order *.symvers

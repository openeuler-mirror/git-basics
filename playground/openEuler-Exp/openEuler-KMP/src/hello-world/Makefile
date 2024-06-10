# Build hello_world module

obj-m := hello_world.o
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	make -C $(KDIR) M=$(PWD) modules

.PHONY:clean
clean:
	rm -f *.ko *.mod.c *.o *.order *.symvers

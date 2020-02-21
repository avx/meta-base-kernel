obj-m := hello1.o

obj-m += hello2.o
hello2-objs = hello2-init.o hello2-main.o

SRC := $(shell pwd)
KERNEL_SRC ?= /usr/src/linux-headers-`uname -r`

all:
	$(MAKE) -C $(KERNEL_SRC) M=$(SRC)

modules_install:
	$(MAKE) -C $(KERNEL_SRC) M=$(SRC) modules_install

clean:
	rm -f *.o *~ core .depend .*.cmd *.ko *.mod.c
	rm -f Module.markers Module.symvers modules.order
	rm -rf .tmp_versions Modules.symvers

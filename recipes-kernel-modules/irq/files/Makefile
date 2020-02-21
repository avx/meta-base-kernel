obj-m += irq-isr-mod.o
obj-m += irq-tasklet-mod.o
obj-m += irq-workq-mod.o

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

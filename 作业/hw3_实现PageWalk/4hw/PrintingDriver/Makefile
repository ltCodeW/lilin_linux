ifneq ($(KERNELRELEASE),)
	obj-m := PrintModule.o
	PrintModule-objs := DriverMain.o DriverFileOperations.o ToolFunctions.o 
EXTRA_CFLAGS := -DNVM_DEBUG -Wno-declaration-after-statement
else
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	rm *.order *.symvers *.mod.c *.o .*.o.cmd
endif

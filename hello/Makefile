ifneq ($(KERNELRELEASE),)
	obj-m := hello.o
	hello-objs := main.o
else
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif

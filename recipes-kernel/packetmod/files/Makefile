obj-m += packetmod.o

packetmod-objs := \
    src/packetmod_main.o \
    src/filter_mac.o \
    src/filter_ipv4.o \

PWD := $(shell pwd)

ccflags-y += -I$(src)/include

all:
	$(MAKE) -C $(KERNEL_SRC) M=$(PWD) modules

modules_install:
	$(MAKE) -C $(KERNEL_SRC) M=$(PWD) modules_install

clean:
	$(MAKE) -C $(KERNEL_SRC) M=$(PWD) clean
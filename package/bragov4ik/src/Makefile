obj-m += stack_device.o
stack_device-objs += utils.o main.o
CONFIG_MODULE_SIG=n

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean


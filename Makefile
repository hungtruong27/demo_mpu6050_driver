obj-m += mpu6050_ioctl.o
KDIR = /lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR) M=$(shell pwd) modules
clean: 
	make -C $(KDIR) M=$(shell pwd) clean

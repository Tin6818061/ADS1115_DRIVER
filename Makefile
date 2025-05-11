obj-m += hello_driver.o #-m module, khi nào cần built-in driver thì thay bang -y
KDIR = /lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR) M=$(shell pwd) modules
clean: 
	make -C $(KDIR) M=$(shell pwd) clean

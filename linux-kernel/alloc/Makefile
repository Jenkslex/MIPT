obj-m+=allocator.o
 
driver:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
load:
	sudo insmod ./allocator.ko
unload:
	sudo rmmod ./allocator.ko
build:
	g++ main.c -o a.out
test:
	sudo ./a.out
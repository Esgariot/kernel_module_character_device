obj-m += bubu.o

.PHONY: clean

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	$(CC) bubu_reader.c -o bubu_reader

debug:
	$(CC) bubu_debug.c -o bubu_debug

install:
	sudo insmod bubu.ko

remove:
	sudo rmmod bubu.ko

test:
	sudo dmesg -C
	$(MAKE) install
	$(MAKE) remove
	dmesg

clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
	rm bubu_reader

obj-m += bubu.o

.PHONY: clean

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	$(CC) bubu_reader.c -o bubu_reader

install:
	sudo insmod bubu.ko

remove:
	sudo rmmod bubu.ko

test:
	sudo dmesg -C
	install
	remove

clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
	rm bubu_reader

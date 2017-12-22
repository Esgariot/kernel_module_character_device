obj-m += CRThreeModule.o
obj-m += GDTReaderModule.o

.PHONY: clean

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	$(CC) CRThreeReader.c -o CRThreeReader

install:
	sudo insmod CRThreeModule.ko

remove:
	sudo rmmod CRThreeModule.ko

test:
	sudo dmesg -C
	$(MAKE) install
	$(MAKE) remove
	sudo dmesg

test_gdt:
	sudo dmesg -C
	sudo insmod GDTReaderModule.ko
	sudo rmmod GDTReaderModule.ko
	sudo dmesg

clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
	rm CRThreeReader

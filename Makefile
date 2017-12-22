obj-m += CRThreeModule.o
obj-m += GDTReaderModule.o

.PHONY: clean

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	$(CC) CRThreeReader.c -o CRThreeReader

install:
	sudo insmod CRThree.ko

remove:
	sudo rmmod CRThree.ko

test:
	sudo dmesg -C
	$(MAKE) install
	$(MAKE) remove
	dmesg

clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
	rm CRThreeReader

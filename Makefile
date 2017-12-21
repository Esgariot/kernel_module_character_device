obj-m += bubu.o

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	$(CC) bubu_reader.c -o bubu_reader
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
	rm bubu_reader
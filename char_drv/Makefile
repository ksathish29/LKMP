obj-m += chdrv.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	$(CC) testing.c -o test
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm test

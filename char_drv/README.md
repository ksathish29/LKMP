Simple kernel and userspace code to implement character driver

Kernel file: chdrv.c
Userspace file: testing.c

How to use

1. Pull in the repository
2. Enter char_drv directory
3. $make
4. Ensure the kernel and userspace code is compiled (.ko, .o, test files created)
5. $insmod chdrv.c
6. Check the character device is listed as /dev/sathish_chdev
7. $./test
8. Enter string to be written into the character device
9. Press enter to read the string from character device in the userspace

Sample output

root@sathish-Satellite-M840:~/sathish/LKMP/char_drv# ./test
Starting to test character device driver
Type in the string to send to the character device :
This is my first character driver
Writing message to the character device : This is my first character driver
Press ENTER to read back from the character device

Reading from the device
Received message : [This is my first character driver(33 letters)]
Character device read and write tested

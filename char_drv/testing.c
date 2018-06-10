/*
 * Userspace program to test the character device kernel module
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_LEN 256
static char rcv[BUFFER_LEN];

int main()
{
	int ret, fd;
	char str_send[BUFFER_LEN];
	printf("Starting to test character device driver\n");
	fd = open("/dev/sathish_chdev", O_RDWR);
	if (fd < 0) {
		printf("Failed to open the character device\n");
		return errno;
	}
	printf("Type in the string to send to the character device :\n");
	scanf("%[^\n]%*c", str_send);
	printf("Writing message to the character device : %s\n", str_send);
	ret = write(fd, str_send, strlen(str_send));
	if (ret < 0) {
		perror("Failed to write the message to character device");
		return ret;
	}
	printf("Press ENTER to read back from the character device\n");
	getchar();

	printf("Reading from the device\n");
	ret = read(fd, rcv, BUFFER_LEN);
	if (ret < 0) {
		perror("Failed to read message from the character device\n");
		return errno;
	}
	printf("Received message : [%s]\n", rcv);
	printf("Character device read and write tested\n");
	return 0;
}

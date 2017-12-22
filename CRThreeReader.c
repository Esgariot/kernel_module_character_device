#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main (int argc, char *argv[]) {

	int fileDevice;
	int readValue;
	int writeValue;
	char readBuffer[256];
	memset(readBuffer,0,sizeof(readBuffer));
	if(argc==0)
		return 1;
	fileDevice = open("/dev/CRThree_char", O_RDWR);
	writeValue = write(fileDevice, argv[1], strlen(argv[1]));
	if(writeValue<0) return 2;
	readValue = read(fileDevice, readBuffer, sizeof(readBuffer)/sizeof(readBuffer[0]));
	printf("%s\n", readBuffer);
	close(fileDevice);
	return 0;
}

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main (int argc, char *argv[]) {

	int fileDevice;
	int processToLookup;
	int readValue;
	int writeValue;
	char readBuffer[256];
	
	if(argc==0)
		return 1;
	
	fileDevice = open("/dev/bubu_char", O_RDWR);
	writeValue = write(fileDevice, argv[1], strlen(argv[1]));
	readValue = read(fileDevice, readBuffer, sizeof(readBuffer)/sizeof(readBuffer[0]));
	printf("%s\n", readBuffer);
	return 0;
}

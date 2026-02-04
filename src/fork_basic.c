#include "stdio.h"
#include <stdlib.h>
#include <unistd.h>



int main(void) {
	
	pid_t cpid;

	cpid = fork();

	if (cpid == 0) {
		printf("Currently in child process\n");
		sleep(16);
		exit(0);

	}
	else {
		printf("Currently in parent process\n");
		sleep(8);
	}
	
	printf("Child PID: %d\n", cpid);
	printf("Parent PID: %d\n", getpid());

	return 0;
}

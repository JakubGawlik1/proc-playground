#include "stdio.h"
#include <linux/prctl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/prctl.h>

int main(void) {
	
	pid_t child1, child2, child3, w;
	int wstatus;
	
	if (prctl(PR_SET_CHILD_SUBREAPER, 1) == -1) { // setting Parent #1 as child subreaper
		perror("prctl error!\n");
		exit(1);
	}

	if ((child1 = fork()) == 0) {
		printf("Child #1 id: %d, Parent id: %d\n", getpid(),getppid());
		
		if ((child2 = fork()) == 0) {
			printf("Child #2 id: %d, Parent id: %d\n", getpid(), getppid());
			sleep(11);
			printf("Child #2 parent id before exit: %d\n", getppid());
			sleep(4);
			exit(3);
		}
		else {
			if ((child3 = fork()) == 0) {
				printf("Child #3 id: %d, Parent id: %d\n", getpid(), getppid());
				sleep(5);
				exit(42);
			}
			else {
				sleep(10); //Child #3 with zombie status for 5s because parent is not calling wait
				w = waitpid(child3, &wstatus, 0);
				printf("Child with id: %d exited with exit code %d\n", w, WEXITSTATUS(wstatus));
				exit(0); //Parent exit before child #2 so we look for new subreaper for child #2


			}
		}
	}
	else {
		printf("Parent #1 id: %d\n", getpid());
		w = wait(&wstatus); //waiting for any child to quit which in our case removes the zombie process
		if (w == -1) {
			perror("wait error!\n");
			exit(1);
		}
		else {
			printf("Child with id: %d exited with status: %d\n", w, WEXITSTATUS(wstatus));
			sleep(5); //now we can see that Parent #1 became a parent to child #2
		}
	}


	return 0;
}

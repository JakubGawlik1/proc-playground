/*/
 * mini_init - simple primite init like manager, creates childs, manages them, acts like a subreaper for orphans
/*/


#include "stdio.h"
#include <linux/prctl.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <unistd.h>

volatile sig_atomic_t sigchld_pending = 0;

void sigchld_handler(int sig) {
	(void) sig; // to suppress unused warnings
	sigchld_pending = 1;
}

void forking(const char *s) {
	
	if (fork() == 0) {
		if (fork() == 0) {
			execlp("sleep", "sleep", "10", NULL);
			exit(127);
		}

		execlp("sleep", "sleep", s, NULL);
		exit(127);
	}
}

int main(void) {

	struct sigaction sa = {0};


	sa.sa_handler = &sigchld_handler;
	sa.sa_flags = SA_NOCLDSTOP;

	sigemptyset(&sa.sa_mask);
	sigaction(SIGCHLD, &sa, NULL);
	
	prctl(PR_SET_CHILD_SUBREAPER, 1); //setting init process as subreaper

	int wstatus;
	pid_t pid;
	
	while (1) {

		sleep(5); //After receiving a SIGCHLD, handler is stopping this sleep syscall and the rest of the code is executed so no zombie processes are made
/*/		sleep(5);
		sleep(4); // Here handler is stopping one syscall but there is another before a waitpid so zombie processes might occur
		sleep(1);/*/
		forking("5");
		
		if (sigchld_pending) {
			sigchld_pending = 0;

			while((pid = waitpid(-1, &wstatus, WNOHANG)) > 0) {
			
				if (WIFSIGNALED(wstatus)) printf("Process with id: %d was terminated by a signal: %d\n", pid,WTERMSIG(wstatus));
				else printf("Process with id: %d exited with status: %d\n", pid, WEXITSTATUS(wstatus));
			}
		}

	}

}

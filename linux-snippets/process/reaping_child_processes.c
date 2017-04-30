#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    const int N = 10;

    pid_t pid;
    
    for (int i = 0; i < N; ++i)
    {
	pid = fork();
	if (pid == 0)
	{
	    exit(100 + i);
	}
	else
	{
	    perror("fork");	    
	}
    }

    int status;
    while ((pid = waitpid(-1, &status, 0)) > 0)
    {
	if (WIFEXITED(status))
	{
	    printf("child %d terininated normally with exit status=%d\n",
		   pid,  WEXITSTATUS(status));
	}
	else
	{
	    printf("child %d terminated abnormally\n", pid);
	}
    }

    if (errno != ECHILD)
    {
	perror("waitpid");
    }
    
    return 0;
}




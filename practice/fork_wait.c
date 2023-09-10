#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
	pid_t pid = getpid();
	pid_t child_pid;
	int i, status;
	char *argv[] = {"/usr/bin/ls", "-l", "/tmp", NULL};

	for (i = 1; i < 6; i++)
	{
		child_pid = fork();
		if (child_pid == -1)
		{
			printf("Error creating child %d\n", i);
			return (1);
		}
		else if (child_pid == 0)
		{
			if (execve(argv[0], argv, NULL) == -1)
			{
				perror("Error executing command\n");
				return (1);
			}
			return (0);
		}
		else
		{
			wait(&status);
			printf("Child %d done.\n", i);
		}
	}
	return (0);
}
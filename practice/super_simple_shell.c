#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int prompt(char **, size_t *);

int main(void)
{
	char **lineptr;
	size_t n, len;
	char *argv[2];
	pid_t pid;
	int status;
	
	while(1)
	{
		len = prompt(lineptr, &n);
		if (len == -1)
			continue;
		if (strcmp(*lineptr, "quit\n") == 0 || strcmp(*lineptr, "exit\n") == 0)
		{
			printf("Bye Bye\n");
			break;
		}
		//if ((argv[0] = malloc(len)) == NULL)
		//	continue;
		printf("Value: %c\n", *lineptr[2]);
		*lineptr[len-1] = '\0';
		argv[0] = *lineptr;
		argv[1] = NULL;
		pid = fork();
		if (pid == -1)
		{
			printf("There was an error in creating a child process");
			return (1);
		}
		else if (pid == 0 )
		{
			if (execve(argv[0], argv, NULL) == -1)
			{
				perror("Error in executing command\n");
				return (1);
			}
			return (0);
		}
		else
		{
			wait(&status);
		}
	}
	return (0);
}

int prompt(char **lineptr, size_t *n)
{
	*lineptr = NULL;
	*n = 0;

	printf("#cisfun$ ");
	return (getline(lineptr, n, stdin));
}

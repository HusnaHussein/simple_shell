#include "main.h"

void execute(bundle *);

/**
 * build_args - build argument vector
 * @b: bundle
*/
void build_args(bundle *b)
{
	char *cmd;
	uint64_t i;

	cmd = malloc(strlen(b->cmd) + 1);
	if (cmd)
	{
		strcpy(cmd, b->cmd);
		b->args = 0;
		while (strtok(b->args ? NULL : cmd, " "))
			b->args++;
		free(cmd);
		if (b->args)
		{
			i = 0;
			while ((b->argv[i] = strtok(i ? NULL : b->cmd, " ")))
			{
				bzero(b->conv_buffer, CONV_BUFFER_SIZE);
				if (strcmp(b->argv[i], "$?") == 0)
					b->argv[i] = base_conv(b->status, 10, b->conv_buffer);
				else if (strcmp(b->argv[i], "$$") == 0)
					b->argv[i] = base_conv(getpid(), 10, b->conv_buffer);
				else if (*b->argv[i] == '$')
				{
					if (getenv(b->argv[i] + 1))
					{
						b->argv[i] = getenv(b->argv[i] + 1);
					}
				}
				i++;
			}
		}
	}
}

/**
 * execute - execute programs
 * @b: bundle
*/
void execute(bundle *b)
{
	if (find_path(b))
	{
		b->argv[0] = b->path;
		b->pid = fork();
		if (b->pid == -1)
		{
			printf("There was an error in creating a child process");
		}
		else if (b->pid == 0)
		{
			if (execve(b->argv[0], b->argv, environ) == -1)
			{
				fprintf(stderr, "%s: 1: %s: not found\n", *b->shell.argv, *b->argv);
				exit(127);
			}

		}
		else
		{
			wait(&b->status);
		}
	}
	else
	{
		fprintf(stderr, "%s: 1: %s: not found\n", *b->shell.argv, *b->argv);
		exit(127);
	}
}

/**
 * find_path - path finder
 * @b: bundle
 *
 * Return: 1 on success otherwise 0
*/
short find_path(bundle *b)
{
	char *path, *token;
	short ret_val = 0;

	bzero(b->path, MAX_PATH);
	strcpy(b->path, b->argv[0]);
	if (access(b->argv[0], X_OK) == 0)
		return (1);
	path = getenv("PATH");
	if (!path)
		return (0);
	path = malloc(strlen(path) + 1);
	if (path)
	{
		strcpy(path, getenv("PATH"));
		token = strtok(path, ":");
		while (token)
		{
			bzero(b->path, MAX_PATH);
			sprintf(b->path, "%s%s%s", token, "/", b->argv[0]);
			if (access(b->path, X_OK) == 0)
			{
				ret_val = 1;
				break;
			}
			token = strtok(NULL, ":");
		}
		free(path);
	}
	return (ret_val);
}

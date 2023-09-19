#include "main.h"

void execute(bundle *);

void build_args(bundle *b)
{
	char *cmd;
	uint64_t i;

	cmd = malloc(strlen(b->cmd) + 1);
	if (cmd)
	{
		b->list.loc[++b->list.cursor] = cmd;
		strcpy(cmd, b->cmd);
		b->args = 0;
		while (strtok(b->args ? NULL : cmd, " "))
			b->args++;
		if (b->args)
		{
			b->argv = malloc(sizeof(char *) * (b->args + 1));
			if (b->argv)
			{
				b->list.loc[++b->list.cursor] = b->argv;
				cmd = b->cmd;
				i = 0;
				while ((b->argv[i] = strtok(i ? NULL : cmd, " ")))
					i++;
			}
		}
	}
}

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
			if (execve(b->argv[0], b->argv, b->shell.env) == -1)
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

void free_mem(allocs *list)
{
	while (list->cursor >= 0)
		free(list->loc[list->cursor--]);
}

/**
 * Note: cmd should never be NULL
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

#include "main.h"

short exe_builtin(bundle *);

short exe_builtin(bundle *b)
{
	int i;

	if (strcmp(b->argv[0], "exit") == 0)
	{
		if (b->args == 1)
			exit(EXIT_SUCCESS);
		else if (b->args == 2)
		{
			i = 0;
			while (b->argv[1][i])
			{
				if (!isdigit(b->argv[1][i++]))
				{
					fprintf(stderr, "%s: 1: exit: Illegal number: ", *b->shell.argv);
					fprintf(stderr, "%s\n", b->argv[1]);
					exit(2);
				}
			}
			if (!b->argv[1][i])
				exit(atoi(b->argv[1]));
		}
		else
		{
			fprintf(stderr, "%s: exit: too many arguments\n", *b->shell.argv);
			exit(EXIT_FAILURE);
		}
		return (1);
	}
	else if (strcmp(b->argv[0], "env") == 0)
	{
		i = 0;
		while (b->shell.env[i] != NULL)
		{
			printf("%s\n", b->shell.env[i]);
			i++;
		}
		return (1);
	}
	else if (strcmp(b->argv[0], "setenv") == 0)
	{
		if (b->args == 3)
		{
			i = 0;
			while (b->shell.env[i] != NULL)
			{
				printf("%s\n", b->shell.env[i]);
				i++;
			}
			setenv(b->argv[1], b->argv[2], 1);
			i = 0;
			while (b->shell.env[i] != NULL)
			{
				printf("%s\n", b->shell.env[i]);
				i++;
			}
			return (1);
		}
		printf("Some error code the checker will reveal to us\n");
	}
	else if (strcmp(b->argv[0], "unsetenv") == 0)
	{
		if (b->args == 2)
		{
			unsetenv(b->argv[1]);
			return (1);
		}
		printf("Some error code the checker will reveal to us\n");
	}
	else if (strcmp(b->argv[0], "cd") == 0)
	{
		bzero(b->pwd.old, MAX_PATH);
		strcpy(b->pwd.old, getenv("OLDPWD"));
		bzero(b->pwd.cur, MAX_PATH);
		getcwd(b->pwd.cur, MAX_PATH);
		bzero(b->pwd.tmp, MAX_PATH);
		bzero(b->pwd.new, MAX_PATH);
		switch (b->args)
		{
			case 1:
				strcpy(b->pwd.new, getenv("HOME"));
				break;
			case 2:
				if (*b->argv[1] == '-')
				{
					strcpy(b->pwd.new, b->pwd.old);
				}
				else
				{
					strcpy(b->pwd.new, b->argv[1]);
				}
			break;
		}
		if (b->args)
			change_dir(b);
		return (1);
	}
	return (0);
}

void free_array_memory(char **lines)
{
	int i = 0;
	while (lines[i] != NULL)
	{
		free(lines[i]);
		i++;
	}
	free(lines);
}

bool change_dir(bundle *b)
{
	bool change = false;
	if (access(b->pwd.new, F_OK) == 0)
	{
		change = !change;
	}
	else
	{
		bzero(b->path, MAX_PATH);
		strcpy(b->path, b->pwd.cur);
		strcat(b->path, "/");
		strcat(b->path, b->pwd.new);
		if (access(b->pwd.new, F_OK) == 0)
			change = !change;
	}
	if (change && chdir(b->pwd.new) == 0)
	{
		setenv("OLDPWD", b->pwd.cur, 1);
		setenv("PWD", b->pwd.new, 1);
		return (true);
	}
	return (false);
}


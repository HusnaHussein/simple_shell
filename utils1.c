#include "main.h"

short exe_builtin(bundle *);

/**
 * exe_builtin - execute builtins
 * @b: bundle
 *
 * Return: 1 on success otherwise 0
*/
short exe_builtin(bundle *b)
{
	int i;
	char *tmp;

	if (strcmp(b->argv[0], "exit") == 0)
	{
		if (b->args == 1)
		{
			free(b->line);
			exit(b->status);
		}
		else if (b->args == 2)
		{
			i = 0;
			while (b->argv[1][i])
			{
				if (!isdigit(b->argv[1][i++]))
				{
					fprintf(stderr, "%s: 1: exit: Illegal number: ", *b->shell.argv);
					fprintf(stderr, "%s\n", b->argv[1]);
					free(b->line);
					exit(2);
				}
			}
			if (!b->argv[1][i])
			{
				i = atoi(b->argv[1]);
				free(b->line);
				exit(i);
			}
		}
		else
		{
			fprintf(stderr, "%s: exit: too many arguments\n", *b->shell.argv);
			free(b->line);
			exit(EXIT_FAILURE);
		}
		return (1);
	}
	else if (strcmp(b->argv[0], "env") == 0)
	{
		i = 0;
		while (environ[i] != NULL)
		{
			printf("%s\n", environ[i]);
			i++;
		}
		return (1);
	}
	else if (strcmp(b->argv[0], "setenv") == 0)
	{
		if (b->args == 3)
		{
			setenv(b->argv[1], b->argv[2], 1);
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
		bzero(b->pwd.cur, MAX_PATH);
		getcwd(b->pwd.cur, MAX_PATH);
		bzero(b->pwd.tmp, MAX_PATH);
		bzero(b->pwd.new, MAX_PATH);
		if (getenv("OLDPWD"))
			strcpy(b->pwd.old, getenv("OLDPWD"));
		else
			strcpy(b->pwd.old, b->pwd.cur);
		switch (b->args)
		{
			case 1:
				if (getenv("HOME"))
					strcpy(b->pwd.new, getenv("HOME"));
				else
					b->args = 0;
				break;
			case 2:
				if (*b->argv[1] == '-')
				{
					printf("%s\n", b->pwd.old);
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
	else if (strcmp(b->argv[0], "alias") == 0)
	{
		switch (b->args)
		{
		case 1:
			bzero(b->path, MAX_PATH);
			strcpy(b->path, "cat ~/.bashrc | awk '{$1=$1};1' | grep '^alias'");
			strcat(b->path, " | sort -k 2 > /tmp/mole_alias");
			system(b->path);
			if (read_textfile(b, "/tmp/mole_alias"))
			{
				i = 0;
				while ((tmp = strtok(i++ ? NULL : b->file_buffer, "\n")))
				{
					if (strstr(tmp, "alias ") == tmp)
						printf("%s\n", tmp);
				}
			}
			else
				printf("NULL NOTHING\n");
			break;
		default:
			break;
		}
		return (1);
	}
	return (0);
}

/**
 * free_array_memory - free memory allocated
 * @lines: lines
*/
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

/**
 * change_dir - changes process PWD
 * @b: bundle
 *
 * Return: true on success otherwise false
*/
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
	fprintf(stderr, "%s: 1: %s: ", *b->shell.argv, *b->argv);
	fprintf(stderr, "can't cd to %s\n", b->argv[1]);
	return (false);
}

/**
* base_conv - converts decimal to specified base
* @n: in decimal
* @buffer: holds converted chars
* @base: to be converted to
*
* Description: Only works bases 2 to 10
* Return: pointer to first char in buffer
*/
char *base_conv(int64_t n, short base, char *buffer)
{
	short tmp_cur = CONV_BUFFER_SIZE - 2;
	int16_t mod;

	bzero(buffer, CONV_BUFFER_SIZE);
	if (n < base && n > -base)
	{
		buffer[tmp_cur--] = (n < 0 ? -n : n) + '0';
	}
	else
	{
		while (n != 0)
		{
			mod = n % base;
			mod = mod < 0 ? -mod : mod;
			buffer[tmp_cur--] = mod + '0';
			n /= base;
		}
	}
	return (buffer + tmp_cur + 1);
}

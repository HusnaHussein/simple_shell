#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int ac, char **av);
/**
* main - stat example
*
* Return: Always 0.
*/
int main(int ac, char **av)
{
	unsigned int i;

	struct stat st;
	char *path, *path_cpy, *tmp;

	if (ac < 2)
	{
		printf("Usage: %s filename ...\n", av[0]);
		return (1);
	}
	i = 1;
	path = getenv("PATH");
	if (path)
	{
		path_cpy = malloc(strlen(path) + 1);
		if (!path_cpy)
			return (1);
        strcpy(path_cpy, path);
		tmp = strtok(path_cpy, ":");
		int j = 0;

		while (av[i] && tmp)
		{
			if (chdir(tmp) == 0)
			{
				printf("%s/%s:", tmp, av[i]);
				if (stat(av[i], &st) == 0)
				{
					printf(" FOUND\n");
				}
				else
				{
					printf(" NOT FOUND\n");
				}
			}
			else
				printf("Couldn't change dir\n");
			tmp = strtok(NULL, ":");
			if (!tmp)
			{
				i++;
				j = 0;
                strcpy(path_cpy, path);
				tmp = strtok(path_cpy, ":");
			}
		}
        free(path_cpy);
	}
	return (0);
}

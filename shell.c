#include "main.h"
int main(int, char **, char **);

/**
 * main - Entry point
 * @argc: command line args count
 * @argv: command line args array
 * @env: process environment
 *
 * Return: integer
*/
int main(int argc, char **argv, char **env)
{
	bundle b = {0};
	short i;
	size_t n = 0;
	char *tmp, *and_pos, *or_pos, *semi_pos, *min, *comment;

	b.shell.argc = argc;
	b.shell.argv = argv;
	b.shell.env = env;
		while (true)
		{
			b.line = NULL;
			if (isatty(STDIN_FILENO))
				printf("$ ");
			if (getline(&b.line, &n, stdin) == -1)
			{
				free(b.line);
				break;
			}
			i = 0;
			tmp = b.line;
			comment = strchr(tmp, '#');
			if (comment && (comment == tmp || isspace(*(comment - 1))))
				*comment = '\0';
			bzero(b.sep_list, sizeof(serpator) * MAX_PATH);
			while (true)
			{
				and_pos = strstr(tmp, "&&");
				or_pos = strstr(tmp, "||");
				semi_pos = strstr(tmp, ";");
				min = and_pos;
				min = !min || (or_pos < min && min && or_pos) ? or_pos : min;
				min = !min || (semi_pos < min && semi_pos && semi_pos) ? semi_pos : min;
				if (!min)
					break;
				switch (*min)
				{
				case '&':
					b.sep_list[i] = and;
					*min = ';';
					*(min + 1) = ' ';
					tmp = min + 2;
					break;
				case '|':
					b.sep_list[i] = or;
					*min = ';';
					*(min + 1) = ' ';
					tmp = min + 2;
					break;
				case ';':
					b.sep_list[i] = semicolon;
					tmp = min + 1;
					break;
				default:
					break;
				}
				i++;
			}
			i = 0;
			bzero(b.cmd_list, sizeof(char *) * MAX_PATH);
			while ((b.cmd_list[i] = strtok(i ? NULL : b.line, ";\n")))
				i++;
			i = 0;
			while ((b.cmd = b.cmd_list[i++]))
			{
				bzero(b.argv, sizeof(char *) * MAX_PATH);
				b.list.cursor = -1;
				build_args(&b);
				if (b.argv[0])
				{
					if (!exe_builtin(&b))
						execute(&b);
					if (b.status == 0 && b.sep_list[i - 1] == or)
						break;
					if (!isatty(STDIN_FILENO) && b.status == 512 &&
					(!b.cmd_list[i] || b.sep_list[i - 1] == and))
					{
						free(b.line);
						exit(2);
					}
				}
			}
			free(b.line);
		}
	return (0);
}

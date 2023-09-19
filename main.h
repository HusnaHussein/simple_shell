#ifndef GroupSimpleShell
#define GroupSimpleShell

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include <limits.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAX_PATH 4096
#define ALLOC_SIZE 32

/**
* struct shell - struct for shell
* @argc: count
* @argv: args
* @env: environment
*/
typedef struct shell
{
	int argc;
	char **argv;
	char **env;
} shell;

/**
* struct allocs - struct for allocs
* @loc: loc
* @cursor: cursor
*/
typedef struct allocs
{
	void *loc[ALLOC_SIZE];
	short cursor;
} allocs;

/**
* struct pwd - struct for pwd
* @cur: cur
* @old: old
* @new: new
* @tmp: tmp
*/
typedef struct pwd
{
	char cur[MAX_PATH];
	char old[MAX_PATH];
	char new[MAX_PATH];
	char tmp[MAX_PATH];
} pwd;

/**
* enum serpator - enum for serpators
* @and: and
* @or: or
* @semicolon: semicolon
*/
typedef enum serpator
{
	and = 1, or, semicolon
} serpator;

/**
* struct bundle - struct for bundle
* @argv: count
* @cmd_list: args
* @path: environment
* @status: count
* @args: args
* @pid: environment
* @line: count
* @list: args
* @shell: environment
* @pwd: count
* @cmd: args
* @sep_list: environment
*/
typedef struct bundle
{
	char **argv;
	char *cmd_list[MAX_PATH];
	char path[MAX_PATH];
	int status;
	uint64_t args;
	pid_t pid;
	char *line;
	allocs list;
	shell shell;
	pwd pwd;
	char *cmd;
	serpator sep_list[MAX_PATH];
} bundle;

void execute(bundle *);
void free_mem(allocs *);
short find_path(bundle *);
short exe_builtin(bundle *);
void free_array_memory(char **);
bool change_dir(bundle *);
void build_args(bundle *);

#endif

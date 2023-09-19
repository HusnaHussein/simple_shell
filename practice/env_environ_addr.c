#include <stdio.h>

extern char **environ;
/**
 * main - prints the environment
 *
 * Return: Always 0.
 */
int main(int ac, char **av, char **env)
{
    printf("env     address: %p\n", env);
    printf("environ address: %p\n", environ);
    return (0);
}
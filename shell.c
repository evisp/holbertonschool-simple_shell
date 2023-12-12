#include "shell.h"

char *name;

int main(int __attribute__ ((unused))argc, char *argv[])
{
    char   *line = NULL;
    size_t  buf_size = 0;
    ssize_t characters = 0;

    name = argv[0];

    while (1)
    {
        if (isatty(STDIN_FILENO) == 1)
            write(1, "$ ", 2);

        characters = getline(&line, &buf_size, stdin);

        if (characters == -1)
        {
            if (isatty(STDIN_FILENO) == 1)
                write(1, "\n", 1);
            break;
        }

        if (line[characters - 1] == '\n')
            line[characters - 1] = '\0';

        if (*line == '\0')
            continue;

        if (command_read(line) == 2)
            break;
    }

    free(line);
    line = NULL;

    return 0;
}

int command_read(char *s)
{
    char *token = NULL;
    char *cmd_arr[100];
    int i;

    if (strcmp(s, "exit") == 0)
        return 2;
    if (strcmp(s, "env") == 0)
        return _printenv();

    token = strtok(s, " "), i = 0;
    while (token && i < 99) 
    {
        cmd_arr[i++] = token;
        token = strtok(NULL, " ");
    }
    cmd_arr[i] = NULL;

    return execute(cmd_arr);
}

int execute(char *cmd_arr[])
{
    pid_t pid;
    int status;
    char *exe_path = command_path(cmd_arr[0]);

    if (exe_path == NULL)
    {
        fprintf(stderr, "%s: %s: not found\n", name, cmd_arr[0]);
        return 3;
    }

    pid = fork();
    if (pid < 0)
    {
        perror("Error:");
        return -1;
    }

    if (pid > 0)
    {
        wait(&status);
    }
    else
    {
        if (environ)
        {
            execve(exe_path, cmd_arr, environ);
            perror("Error:");
            exit(1);
        }
        else
        {
            execve(exe_path, cmd_arr, NULL);
        }
    }

    free(exe_path);
    return 0;
}

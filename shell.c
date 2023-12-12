#include "shell.h"

/* global variable name */
char *name;

int main(int __attribute__ ((unused))argc, char *argv[])
{
    char   *line = NULL;
    size_t  buf_size = 0;
    ssize_t characters = 0;

    name = argv[0];

    while (1)
    {
        /* check if an open file descriptor is associated with the terminal device */
        if (isatty(STDIN_FILENO) == 1)
            /* write to standard output */
            write(1, "$ ", 2);

        /* store the number of characters read */
        characters = getline(&line, &buf_size, stdin);

        /* in case of failure or reaching EOF, write a new line if the file is open */
        if (characters == -1)
        {
            if (isatty(STDIN_FILENO) == 1)
                write(1, "\n", 1);
            break;
        }

        /* when encountering a newline, write a null terminating byte */
        if (line[characters - 1] == '\n')
            line[characters - 1] = '\0';

        if (*line == '\0')
            continue;

        /* in case of exit */
        if (command_read(line) == 2)
            break;
    }

    /* free resources */
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
    while (token && i < 99) // Ensure the array doesn't overflow
    {
        cmd_arr[i++] = token;
        token = strtok(NULL, " ");
    }
    cmd_arr[i] = NULL;

    /* Return status code */
    return execute(cmd_arr);
}

int execute(char *cmd_arr[])
{
    char *exe_path = command_path(cmd_arr[0]);
    if (exe_path == NULL)
    {
        fprintf(stderr, "%s: %s: not found\n", name, cmd_arr[0]);
        return 3;
    }

    pid_t pid;
    int status;

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

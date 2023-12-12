#include "shell.h"

char *command_path(char *cmd)
{
    char *path = _getenv("PATH");
    if (path == NULL)
    {
        fprintf(stderr, "PATH variable not found in the environment.\n");
        return NULL;
    }

    char *token, *path_copy, *full_path;
    struct stat buf;

    /* Duplicate the PATH variable to avoid modifying the original */
    path_copy = strdup(path);
    if (path_copy == NULL)
    {
        perror("Error duplicating PATH");
        return NULL;
    }

    /* Tokenize the PATH variable */
    token = strtok(path_copy, ":");
    while (token != NULL)
    {
        /* Construct the full path by concatenating the directory and the command */
        full_path = malloc(strlen(token) + strlen(cmd) + 2); // +2 for '/' and '\0'
        if (full_path == NULL)
        {
            perror("Error allocating memory for full path");
            free(path_copy);
            return NULL;
        }

        strcpy(full_path, token);
        strcat(full_path, "/");
        strcat(full_path, cmd);

        /* Check if the full path is a valid executable */
        if (stat(full_path, &buf) == 0)
        {
            free(path_copy);
            return full_path;
        }

        free(full_path);
        token = strtok(NULL, ":");
    }

    /* Check if the command is in the current directory */
    if (stat(cmd, &buf) == 0)
    {
        free(path_copy);
        return strdup(cmd);
    }

    free(path_copy);
    return NULL;
}

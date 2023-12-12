#include "shell.h"

char *command_path(char *cmd)
{
    char *token, *path_copy, *path, *full_path;
    struct stat buf;
    
    path = _getenv("PATH");

    if (path == NULL)
    {
        fprintf(stderr, "PATH variable not found in the environment.\n");
        return NULL;
    }

    path_copy = strdup(path);
    if (path_copy == NULL)
    {
        perror("Error duplicating PATH");
        return NULL;
    }

    token = strtok(path_copy, ":");
    while (token != NULL)
    {
        full_path = malloc(strlen(token) + strlen(cmd) + 2); 
        if (full_path == NULL)
        {
            perror("Error allocating memory for full path");
            free(path_copy);
            return NULL;
        }

        strcpy(full_path, token);
        strcat(full_path, "/");
        strcat(full_path, cmd);

        if (stat(full_path, &buf) == 0)
        {
            free(path_copy);
            return full_path;
        }

        free(full_path);
        token = strtok(NULL, ":");
    }

    if (stat(cmd, &buf) == 0)
    {
        free(path_copy);
        return strdup(cmd);
    }

    free(path_copy);
    return NULL;
}

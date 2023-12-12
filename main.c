#include "shell.h"

int main(void)
{
	/* Test _printenv */
	printf("Printing environment variables:\n");
	_printenv();

	/* Test _getenv */
	char *path_var = _getenv("PATH");
	if (path_var != NULL)
		//printf("\nValue of PATH: %s\n", path_var);
        printf("Path found\n");
	else
		printf("\nPATH not found in environment.\n");

    /* Test command_path */
    char *cmd = "pwd";
    char *path = command_path(cmd);

    if (path != NULL)
    {
        printf("Full path for %s: %s\n", cmd, path);
        free(path); 
    } else {
        printf("Command %s not found.\n", cmd);
    }

	return 0;
}

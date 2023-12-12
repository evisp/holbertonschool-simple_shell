#include "shell.h"

int main(void)
{
    char *cmd, *path, *path_var; 

	printf("Printing environment variables:\n");
	_printenv();

	path_var = _getenv("PATH");
	if (path_var != NULL)
		printf("\nValue of PATH: %s\n", path_var);
    else
		printf("\nPATH not found in environment.\n");

    cmd = "pwd";
    path = command_path(cmd);

    if (path != NULL)
    {
        printf("Full path for %s: %s\n", cmd, path);
        free(path); 
    } else {
        printf("Command %s not found.\n", cmd);
    }

	return 0;
}

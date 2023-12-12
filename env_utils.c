#include "shell.h"

int _printenv()
{
    int i, file_descr = 1;

	if (environ == NULL) {
		fprintf(stderr, "Environment variable is NULL.\n");
		return (-1);
	}

	i = 0;
	while (environ[i] != NULL)
	{
		write(file_descr, environ[i], strlen(environ[i]));
		write(file_descr, "\n", 1);
		++i;
	}

	return 0;
}

char *_getenv(char *name)
{
	if (environ == NULL) {
		fprintf(stderr, "Environment variable is NULL.\n");
		return NULL;
	}

	int len_name = strlen(name);

	for (int i = 0; environ[i] != NULL; i++)
	{
		if (strncmp(environ[i], name, len_name) == 0)
			return &environ[i][len_name];
	}

	return NULL;
}

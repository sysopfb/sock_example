#include "base.h"

void die(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

void remove_newline(char *str)
{
	char *pos = strchr(str, '\n');
	if(pos != NULL)
		*pos = '\0';
}


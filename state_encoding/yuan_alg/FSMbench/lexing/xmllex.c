#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "xmllex.h"

int token(char buff_in)
{
	int buff_out;

	if(buff_in == '<')
		buff_out = 0;
	else if (buff_in == '>')
		buff_out = 1;
	else if (buff_in == '/')
		buff_out = 2;
	else 					/* others */
	{
		buff_out = 3;
	}

	return buff_out;

}

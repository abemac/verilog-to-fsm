#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "divisibility.h"

int token(char buff_in)
{
	int buff_out;

	if(buff_in == '0')
		buff_out = 0;
	else if (buff_in == '1')
		buff_out = 1;
	else if (buff_in == EOF)
		;
	else
	{
			printf("unknow character!\n");
	}

	return buff_out;

}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "clarissa.h"

int token(char buff_in)
{
	int buff_out;

	if(buff_in == 'y')
		buff_out = 0;
	else if (buff_in == 'o')
		buff_out = 1;
	else if (buff_in == 'u')
		buff_out = 2;
	else if (buff_in == 'l')
		buff_out = 3;
	else if (buff_in == 'i')
		buff_out = 4;
	else if (buff_in == 'k')
		buff_out = 5;
	else if (buff_in == 'e')
		buff_out = 6;
	else if (buff_in == 'a')
		buff_out = 7;
	else if (buff_in == 'p')
		buff_out = 8;
	else if (buff_in == ',')
		buff_out = 9;
	else if (buff_in == '.')
		buff_out = 10;
	else /* x i.e. others */
	{
		buff_out = 11;
	}

	return buff_out;

}

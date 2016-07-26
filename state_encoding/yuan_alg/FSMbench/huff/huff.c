#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "huff.h"

int token(char buff_in)
{
	int buff_out;
	if( buff_in == '0' )
		buff_out = 0;
	else if ( buff_in == '1' )
		buff_out = 1;
	else if (buff_in == EOF)
		buff_out = 1;
	else
	{
        buff_out = 1;
        printf("unknow character!\n");
	}

	return buff_out;

}

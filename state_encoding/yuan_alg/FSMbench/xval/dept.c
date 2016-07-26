//
//  dept.c
//  SpecDFA
//
//  Created by Zhijia Zhao on 12/23/13.
//  Copyright (c) 2013 Zhijia Zhao. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dept.h"

int token(char buff_in)
{
    int buff_out;
    if( buff_in == ' ')
        buff_out = 0;
    else if( buff_in == '-')
        buff_out = 1;
    else if( buff_in == '.')
        buff_out = 2;
    else if( buff_in == '/')
        buff_out = 3;
    else if( buff_in >= '0' && buff_in <= '9')
        buff_out = 4;
    else if( buff_in == ':')
        buff_out = 5;
    else if( buff_in == '@')
        buff_out = 6;
    else if( buff_in >= 'A' && buff_in <= 'Z')
        buff_out = 7;
    else if( buff_in == '<')
        buff_out = 8;
    else if( buff_in == '>')
        buff_out = 9;
    else if( buff_in == 'a')
        buff_out = 10;
    else if( buff_in == 'b')
        buff_out = 11 ;
    else if( buff_in == 'c')
        buff_out = 12 ;
    else if( buff_in == 'd')
        buff_out = 13 ;
    else if( buff_in == 'e')
        buff_out = 14 ;
    else if( buff_in == 'f')
        buff_out = 15 ;
    else if( buff_in == 'g')
        buff_out = 16 ;
    else if( buff_in == 'h')
        buff_out = 17 ;
    else if( buff_in == 'i')
        buff_out = 18 ;
    else if( buff_in >= 'j' && buff_in <= 'k')
        buff_out = 19 ;
    else if( buff_in == 'l')
        buff_out = 20 ;
    else if( buff_in == 'm')
        buff_out = 21 ;
    else if( buff_in == 'n')
        buff_out = 22 ;
    else if( buff_in == 'o')
        buff_out = 23 ;
    else if( buff_in == 'p')
        buff_out = 24 ;
    else if( buff_in == 'q')
        buff_out = 25 ;
    else if( buff_in == 'r')
        buff_out = 26 ;
    else if( buff_in == 's')
        buff_out = 27 ;
    else if( buff_in == 't')
        buff_out = 28 ;
    else if( buff_in == 'u')
        buff_out = 29 ;
    else if( buff_in >= 'v' && buff_in <= 'x')
        buff_out = 30 ;
    else if( buff_in == 'y')
        buff_out = 31 ;
    else if( buff_in == 'z')
        buff_out = 32 ;
    else if( buff_in == '~')
        buff_out = 33 ;
    else if( buff_in == EOF)
        buff_out = 10 ;
    else if( buff_in == 10) /* new line */
        buff_out = 10 ;
    else
    {
        buff_out = 10; /* a */
        printf("unknow character:%x(hex format) changed to a\n", buff_in);
    }
    
    return buff_out;
    
}

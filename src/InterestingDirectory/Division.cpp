#include <iostream>
#include "Division.h"

double Division(int a, int b)
{
    if(b == 0)
    {
        return 0;
    } // Une division par zero doit retourner 0
    else
    {
        return ((double) a) / ((double) b);
    }
}
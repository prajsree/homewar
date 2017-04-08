#include <stdio.h>
#include "demon_user_api.h"

uint8_t get_user_option (void) 
{
    static int t = -1;
    int r;
    r = ((t+1)%3);
    t++;
    return r;
}

void set_other_user_option (uint8_t opt)
{
    return;
}

void set_result (int8_t opt)
{
    return;
}

int main (void)
{
    return 0;
}

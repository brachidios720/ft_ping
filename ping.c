#include "includes/ping.h"

int main(int ac, char **av)
{

    
    if(parse_args(ac, av) == 1)
        return 1;
    return 0;
}
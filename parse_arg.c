#include "includes/ping.h"


int parse_args(int ac, char **av){

    if(ac < 2 && ac > 4)
        return(1);

    if(pars_args_to_ping_url(ac, av) == 1){
        return(1);
    // if(ft_strncmp(av[1], "ping", 4) != 0){
    //     print("Fomat error in ping word");
    //     return(1);
    }
    return(0);
}
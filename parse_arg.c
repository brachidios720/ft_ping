#include "includes/ping.h"


int parse_args(int ac, char **av){

    if(ac < 2 || ac > 4)
        return(1);

    if(check_option(av) == 1){
        print_help();
        return 1;
    }
    
    if(check_args_to_ping(av) == 0){
        if(is_valid_ipv4(av[1]) == 1){
            printf("ERROR: IP Format Error\n");
            return 1;
        }
    }
    else{
        if(pars_args_to_ping_url(ac, av) == 1){
            printf("ERROR: URL Format Error\n");
            return 1;
        }
    }
    return(0);
}
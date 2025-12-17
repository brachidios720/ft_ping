#include "includes/ping.h"


int parse_args(int ac, char **av){

    if(ac < 2 || ac > 4)
        return(1);

    //check du -v (verbose)
    for(int i = 1; i < ac; i++){
        if(strcmp(av[i], "-v") == 0)
        {
            g_verbose = 1;
            //on remet l'argument IP/URL dans av[1] pour la suite du parse
            if(i + 1 < ac)
                av[1] = av[i + 1];
            break ;
        }
        else if(strcmp(av[i], "-?") == 0)
        {
            print_help();
            return 1;
        }
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
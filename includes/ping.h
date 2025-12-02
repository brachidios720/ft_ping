#ifndef PING_H

#define PING_H


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

# define EXTENSION  ".fr", ".com", ".org" , NULL



int parse_args(int ac, char **av);
int check_args_to_ping(char **av);
int pars_args_to_ping_url(int ac, char **av);
char *extract_extension(char **av);
int is_valid_ipv4(const char *ip);

#endif
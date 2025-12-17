#ifndef PING_H

#define PING_H


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <netinet/ip_icmp.h>
#include <string.h> 

# define EXTENSION  ".fr", ".com", ".org" , NULL

extern int g_verbose;




int parse_args(int ac, char **av);
int check_args_to_ping(char **av);
int pars_args_to_ping_url(int ac, char **av);
char *extract_extension(char **av);
int is_valid_ipv4(const char *ip);
int check_option(char **av);
void print_help(void);

void build_icmp_packet(struct icmphdr *icmp, int seq);
unsigned short compute_icmp_checksum(void *buf, int length);

#endif
#include "includes/ping.h"

unsigned short compute_icmp_checksum(void *buf, int length)
{
    unsigned short *data = buf;
    unsigned int sum = 0;

    //somme 16 bits
    while(length > 1)
    {
        sum += *data++;
        length -= 2;
    }

    //si length % 2 = 1, ajout d'un octet
    if(length == 1)
    {
        unsigned short last_byte = 0;
        *((unsigned char *)&last_byte) = *((unsigned char *)data);
        sum += last_byte;
    }

    //ajout des retenues
    while(sum >> 16)
    {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }

    //complement a un
    return ((unsigned short)(~sum));
}

void build_icmp_packet(struct icmphdr *icmp, int seq)
{
    icmp->type = ICMP_ECHO;
    icmp->code = 0;
    icmp->un.echo.id = getpid();
    icmp->un.echo.sequence = seq;
    icmp->checksum = 0;
}  


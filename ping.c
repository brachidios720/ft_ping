#include "includes/ping.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>

int main(int ac, char **av)
{

    
    if(parse_args(ac, av) == 1)
        return 1;

    
    //creation du socket
    int sock;

    //creation de la structure qui sert a build le paquet ICMP
    struct icmphdr icmp;
    int seq = 1;

    build_icmp_packet(&icmp, seq);

    //calcul du checksum
    icmp.checksum = compute_icmp_checksum(&icmp, sizeof(struct icmphdr));

    printf("ICMP Packet:\n");
    printf(" type = %d\n", icmp.type);
    printf(" code = %d\n", icmp.code);
    printf(" id = %d\n", icmp.un.echo.id);
    printf(" seq = %d\n", icmp.un.echo.sequence);
    printf(" checksum = %d\n", icmp.checksum);


    sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if(sock < 0)
        return(perror("socket"), 1);

    printf("[OK] SOCKET CREE AVEC SUCCES\n");

    //import de la structure timeval
    struct timeval tv;
    tv.tv_sec = 1 ;
    tv.tv_usec = 0;
    //on set les options du socket 
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0)
    {
        perror("socketopt");
        close(sock);
        return 1;
    }

    //on check l'attente d'un paquet dans notre socket 
    char buffer[1024];
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    printf("[ICMP] Attente d'un paquet pendant 1 seconde ... \n");

    ssize_t ret = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&addr, &addr_len);

    if ( ret < 0 )
    {
        perror("[ICMP] recvfrom");
        printf("Timeout OK (aucun paquet recu)\n");   
    }
    else 
        printf("Paquet ICMP recu (%ld octets) \n", ret);

    

    close(sock);
    return 0;
}
#include "includes/ping.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <sys/time.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <netdb.h> 


typedef struct s_stats {
    int transmitted;
    int received;
    double min;
    double max;
    double sum;
} t_stats;

struct addrinfo hints;
struct addrinfo *res;


static t_stats stats;

void sigint_handler(int sig)
{
    (void)sig;

    double avg = stats.received ? stats.sum / stats.received : 0;

    printf("\n--- %s ping statistics ---\n", "target");
    printf("%d packets transmitted, %d received, %.0f%% packet loss\n",
        stats.transmitted,
        stats.received,
        stats.transmitted ? ((stats.transmitted - stats.received) * 100.0) / stats.transmitted : 0);

    printf("rtt min/avg/max = %.2f/%.2f/%.2f ms\n",
        stats.min == 1e9 ? 0 : stats.min,
        avg,
        stats.max);

    close(0);
    exit(0);
}

int main(int ac, char **av)
{
    if(parse_args(ac, av) == 1)
        return 1;

    signal(SIGINT, sigint_handler);

    int sock;
    struct icmphdr icmp;
    int seq = 1;

    stats.transmitted = 0;
    stats.received = 0;
    stats.min = 1e9;
    stats.max = 0;
    stats.sum = 0;

    sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if(sock < 0)
        return(perror("socket"), 1);

    printf("[OK] SOCKET CREE AVEC SUCCES\n");

    struct timeval tv;
    tv.tv_sec = 1 ;
    tv.tv_usec = 0;

    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0)
    {
        perror("socketopt");
        close(sock);
        return 1;
    }

    // permet de pourvoir donner une addresse ipv4 si nom de domaine rentrer
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;        // Obligatoire : ping 42 = IPv4
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = IPPROTO_ICMP;

    int ret = getaddrinfo(av[1], NULL, &hints, &res);
    if (ret != 0)
    {
        fprintf(stderr, "ft_ping: %s: %s\n", av[1], gai_strerror(ret));
        exit(1);
    }

    // Résultat IPv4
    struct sockaddr_in *dest = (struct sockaddr_in *)res->ai_addr;

    // Pour afficher l’IP
    char ipstr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &dest->sin_addr, ipstr, sizeof(ipstr));

    printf("PING %s (%s):\n", av[1], ipstr);


    char buffer[1024];
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    printf("PING %s:\n", av[1]);

    while (1)
    {
        build_icmp_packet(&icmp, seq);
        icmp.checksum = compute_icmp_checksum(&icmp, sizeof(struct icmphdr));

        struct timeval start, end;
        gettimeofday(&start, NULL);

        ssize_t sent = sendto(sock, &icmp, sizeof(icmp), 0,
            (struct sockaddr *)dest, sizeof(struct sockaddr_in));

        stats.transmitted++;

        if (sent < 0)
        {
            perror("sendto");
            continue;
        }

        ssize_t ret = recvfrom(sock, buffer, sizeof(buffer), 0,
            (struct sockaddr *)&addr, &addr_len);

        gettimeofday(&end, NULL);

        if (ret < 0)
        {
            if (errno == EAGAIN)
                printf("Request timeout for icmp_seq %d\n", seq);
        }
        else
        {
            double time_ms = (end.tv_sec - start.tv_sec) * 1000.0 +
                (end.tv_usec - start.tv_usec) / 1000.0;

            struct iphdr *ip = (struct iphdr *)buffer;
            int ip_header_len = ip->ihl * 4;
            struct icmphdr *reply = (struct icmphdr *)(buffer + ip_header_len);

            stats.received++;
            stats.sum += time_ms;
            if (time_ms < stats.min) stats.min = time_ms;
            if (time_ms > stats.max) stats.max = time_ms;

            printf("64 bytes from %s: icmp_seq=%d ttl=%d time=%.2f ms\n",
                av[1],
                reply->un.echo.sequence,
                ip->ttl,
                time_ms);
        }

        seq++;
        sleep(1);
    }

    close(sock);
    return 0;
}

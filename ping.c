#include "includes/ping.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <sys/time.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

typedef struct s_stats {
    int transmitted;
    int received;
    double min;
    double max;
    double sum;
} t_stats;

static t_stats stats;
int g_verbose = 0;

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
    if (parse_args(ac, av) == 1)
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
        return (perror("socket"), 1);

    printf("[OK] SOCKET CREE AVEC SUCCES\n");

    // Timeout de 1 seconde
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    // TTL = 64 (comme ping Linux)
    int ttl = 64;
    setsockopt(sock, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));

    char buffer[1024];
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    struct sockaddr_in dest;
    dest.sin_family = AF_INET;
    dest.sin_addr.s_addr = inet_addr(av[1]);

    printf("PING %s:\n", av[1]);

    while (1)
    {
        build_icmp_packet(&icmp, seq);
        icmp.checksum = compute_icmp_checksum(&icmp, sizeof(struct icmphdr));

        struct timeval start, end;
        gettimeofday(&start, NULL);

        ssize_t sent = sendto(sock, &icmp, sizeof(icmp), 0,
            (struct sockaddr *)&dest, sizeof(dest));

        stats.transmitted++;

        if (sent < 0)
        {
            perror("sendto");
            continue;
        }

        int error_shown = 0;
        int got_reply = 0;

        while (1)
        {
            ssize_t ret = recvfrom(sock, buffer, sizeof(buffer), 0,
                (struct sockaddr *)&addr, &addr_len);

            gettimeofday(&end, NULL);

            if (ret < 0)
            {
                if (errno == EAGAIN)
                {
                    if (!got_reply && !error_shown)
                        printf("Request timeout for icmp_seq %d\n", seq);
                }
                break;
            }

            struct iphdr *ip = (struct iphdr *)buffer;
            int ip_header_len = ip->ihl * 4;
            struct icmphdr *reply = (struct icmphdr *)(buffer + ip_header_len);

            if (reply->type != ICMP_ECHOREPLY)
            {
                if (g_verbose && !error_shown)
                {
                    printf("From %s: icmp_seq=%d ICMP error: type %d code=%d\n",
                        inet_ntoa(addr.sin_addr),
                        seq,
                        reply->type,
                        reply->code);
                    error_shown = 1;
                }
                continue;
            }

            // Echo reply
            double time_ms = (end.tv_sec - start.tv_sec) * 1000.0 +
                             (end.tv_usec - start.tv_usec) / 1000.0;

            printf("64 bytes from %s: icmp_seq=%d ttl=%d time=%.2f ms\n",
                av[1], seq, ip->ttl, time_ms);

            stats.received++;
            stats.sum += time_ms;
            if (time_ms < stats.min) stats.min = time_ms;
            if (time_ms > stats.max) stats.max = time_ms;

            got_reply = 1;
        }

        seq++;
        sleep(1);
    }

    close(sock);
    return 0;
}

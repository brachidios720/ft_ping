#include "includes/ping.h"


int pars_args_to_ping_url(int ac, char **av)
{
    // permet de determiner si l extension est valide ou non
    (void)ac;
    char *str;
    char *ext;
    size_t flag = 1;
    size_t check = 0;
    size_t here = 0;

    str = av[1];
    if(str == NULL)
        return 1;

    while((str[check] != '.') && str[check])
        check++;

    if(str[check] == '.')
        here += 1;
    else
        return 1;
    
    size_t len = strlen(str);
    
    while(str[len] !=  '.')
    len --;
    len++;
    ext = extract_extension(av);
    if(ext == NULL)
        return 1;
    const char *tab[] = { EXTENSION };
    size_t i = 0;
    while(tab[i]){

        if(strcmp(tab[i], ext) == 0)
            flag = 0;
        i++;
    }
    free(ext);
    if(flag == 0)
        return 0;
    else 
        return 1;
}


char *extract_extension(char **av){

    // ret et l'extension utiliser dans l'arguments rentrer
    size_t i = 0;
    size_t start = 0;
    while(av[1][i] != '.')
        i++;
    start = i;
    while(av[1][i])
        i++;
    if((i - start) > 4 )
        return NULL;
    size_t x = 0;
    char *ret = malloc(sizeof(char) * 4);
    if(!ret)
        return NULL;
    while(start < i){
        ret[x] = av[1][start];
        start++;
        x++;
    }
    ret[x] = '\0';
    return ret;
}


int check_args_to_ping(char **av){

    char *s = av[1];
    size_t i = 0;
    while(s[i]){
        if((s[i] >= '0' && s[i] <= '9') || s[i] == '.')
            i++;
        else 
            return(1);
    }
    return 0;
}

int is_valid_ipv4(const char *ip){

    int seg = 0;
    int start = 0;
    if(!ip || !*ip)
        return 1;
    
    for(int i = 0; ; i++){

        char c = ip[i];
        if(c == '.' || c == '\0'){

            if(i == start)
                return 1;

            for(int j = start; j < i; j++){
                if(ip[j] < '0' || ip[j] > '9')
                    return 1;
            }
            int  num = atoi(ip + start);
            if(num < 0 || num > 255)
                return 1;

            seg++;
            if(c =='\0')
                break;
            if(seg > 3)
                return 1;

            start = i + 1;
        }
        else{
            if(c < '0' || c > '9')
                return 1;
        }
    }
    return seg == 4 ? 0 : 1;
}


void print_help(void){

    printf("Usage: ft_ping [options] <destination>\n");
    printf("Options:\n");
    printf("  -v    verbose output\n");
    printf("  -?    display this help\n");
}

int check_option(char **av){


    if(strcmp(av[1], "-?") == 0)
        return 1;
    return 0;
}
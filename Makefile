# Nom des exécutables
PING = ft_ping

# Fichiers sources
PING_SRC = parsing_utils.c \
			ping.c \
			parse_arg.c 
			
# Compilateurs et flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g3

OBJ = $(PING_SRC:.c=.o)
# Règle par défaut
all: $(PING) 

$(PING): $(PING_SRC)
	$(CC) $(CFLAGS) -o $(PING) $(PING_SRC)

# Nettoyage
clean:
	rm -f $(OBJ)

# Nettoyage complet
fclean: clean
	rm -f $(PING)

# Recompilation complète
re: fclean all

# Cibles "non fichiers"
.PHONY: all clean fclean re
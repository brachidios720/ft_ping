ft_ping — README.md

# ft_ping

Implémentation en C d’un équivalent simplifié de `ping`, réalisée dans le cadre du cursus 42.

## À propos

`ft_ping` envoie des paquets ICMP Echo Request à une cible réseau, mesure le temps de réponse et affiche des statistiques similaires à l’outil système `ping`.

Ce projet m’a permis de travailler sur :
- les sockets bas niveau,
- le protocole ICMP,
- la gestion des erreurs réseau,
- le parsing d’arguments,
- la mesure du temps et l’affichage de statistiques.

## Fonctionnalités

- Envoi de paquets ICMP Echo Request
- Réception et analyse des Echo Reply
- Résolution d’hôte
- Affichage du TTL, du temps de réponse et de la taille des paquets
- Résumé statistique en fin d’exécution
- Gestion d’arguments en ligne de commande

## Structure du projet

- `ping.c` : logique principale
- `ICMP_packet.c` : construction et traitement des paquets ICMP
- `parse_arg.c` / `parsing_utils.c` : parsing des arguments
- `includes/` : headers du projet
- `Makefile` : compilation

## Compilation

```bash
make
Utilisation
./ft_ping <hostname>
Exemple :
./ft_ping google.com
Objectifs pédagogiques
* Comprendre le fonctionnement d’ICMP
* Manipuler les sockets raw
* Reproduire le comportement d’un outil système connu
* Écrire un programme robuste face aux erreurs réseau
Remarques
Selon le système, l’exécution peut nécessiter des privilèges élevés pour l’utilisation de sockets raw.

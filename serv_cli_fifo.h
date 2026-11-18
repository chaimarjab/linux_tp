#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define NMAX  10
#define FIFO1 "fifo1"   /* clients -> serveur : questions */
#define FIFO2 "fifo2"   /* serveur -> clients : réponses  */

typedef struct {
    pid_t pid_client;   /* pour que le serveur puisse réveiller le client */
    int   n;            /* nombre de valeurs demandées (1..NMAX) */
} Question;

typedef struct {
    pid_t pid_serveur;  /* pour que le client puisse acquitter */
    int   n;
    int   tab[NMAX];    /* les n nombres tirés au sort */
} Reponse;


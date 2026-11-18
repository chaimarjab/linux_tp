#include "serv_cli_fifo.h"
#include "Handlers_Cli.h"

int main()
{
    /* Déclarations */
    int fd1, fd2, i;
    Question q;
    Reponse  r;

    /* Ouverture des tubes nommés */
    fd1 = open(FIFO1, O_WRONLY);
    fd2 = open(FIFO2, O_RDONLY);
    if (fd1 == -1 || fd2 == -1) {
        perror("open (le serveur est-il lancé ?)");
        exit(1);
    }

    /* Installation des handlers (avant d'envoyer la question) */
    signal(SIGUSR1, hand_reveil);

    /* Construction et envoi d'une question */
    srand(getpid());
    q.pid_client = getpid();
    q.n = rand() % NMAX + 1;            /* entre 1 et NMAX */
    printf("Client %d : je demande %d nombre(s)\n", getpid(), q.n);
    write(fd1, &q, sizeof(Question));

    /* Attente de la réponse */
    while (!reveil)
        pause();

    /* Lecture de la réponse */
    read(fd2, &r, sizeof(Reponse));

    /* Envoi du signal SIGUSR1 au serveur */
    kill(r.pid_serveur, SIGUSR1);

    /* Traitement local de la réponse */
    printf("Client %d a reçu :", getpid());
    for (i = 0; i < r.n; i++)
        printf(" %d", r.tab[i]);
    printf("\n");

    close(fd1);
    close(fd2);
    return 0;
}

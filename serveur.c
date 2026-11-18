#include "serv_cli_fifo.h"
#include "Handlers_Serv.h"

int main()
{
    /* Déclarations */
    int fd1, fd2, i;
    Question q;
    Reponse  r;

    /* Création des tubes nommés */
    unlink(FIFO1);              /* supprime d'éventuels restes */
    unlink(FIFO2);
    mkfifo(FIFO1, 0666);
    mkfifo(FIFO2, 0666);

    /* Initialisation du générateur de nombres aléatoires */
    srand(getpid());

    /* Ouverture des tubes nommés */
    fd1 = open(FIFO1, O_RDWR);
    fd2 = open(FIFO2, O_RDWR);

    /* Installation des handlers */
    signal(SIGUSR1, hand_reveil);
    signal(SIGINT,  fin_serveur);
    signal(SIGTERM, fin_serveur);

    printf("Serveur prêt (pid %d)\n", getpid());

    while (1) {
        /* Lecture d'une question */
        if (read(fd1, &q, sizeof(Question)) != sizeof(Question))
            continue;
        if (q.n < 1 || q.n > NMAX)
            continue;

        /* Construction de la réponse */
        r.pid_serveur = getpid();
        r.n = q.n;
        for (i = 0; i < q.n; i++)
            r.tab[i] = rand() % 1000;

        /* Envoi de la réponse */
        write(fd2, &r, sizeof(Reponse));

        /* Envoi du signal SIGUSR1 au client concerné */
        reveil = 0;
        kill(q.pid_client, SIGUSR1);

        /* Attente de l'acquittement du client */
        while (!reveil)
            pause();
    }
}

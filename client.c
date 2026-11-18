#include "serv_cli_fifo.h"
#include "Handlers_Cli.h"

int main()
{
    /* Déclarations */
    int fd1, fd2, i, somme = 0;
    Question q;
    Reponse r;

    sigset_t masque_usr1, ancien_masque, masque_attente;

    /* Ouverture des tubes nommés */
    fd1 = open(FIFO1, O_WRONLY);
    fd2 = open(FIFO2, O_RDONLY);

    if (fd1 == -1 || fd2 == -1) {
        perror("open (le serveur est-il lancé ?)");
        exit(1);
    }

    /* Installation du handler SIGUSR1 */
    signal(SIGUSR1, hand_reveil);

    /* Préparation du masque pour bloquer SIGUSR1 */
    sigemptyset(&masque_usr1);
    sigaddset(&masque_usr1, SIGUSR1);

    /* Bloquer SIGUSR1 et sauvegarder l'ancien masque */
    sigprocmask(SIG_BLOCK, &masque_usr1, &ancien_masque);

    /* Construction de la question */
    srand(getpid());
    q.pid_client = getpid();
    q.n = rand() % NMAX + 1;

    printf("Client %d : je demande %d nombres\n", getpid(), q.n);

    /* Initialisation de reveil */
    reveil = 0;

    /* Envoi de la question au serveur */
    write(fd1, &q, sizeof(Question));

    /* Préparation du masque utilisé pendant l'attente */
    masque_attente = ancien_masque;
    sigdelset(&masque_attente, SIGUSR1);

    /* Attente de la réponse */
    while (!reveil)
        sigsuspend(&masque_attente);

    /* Restauration de l'ancien masque */
    sigprocmask(SIG_SETMASK, &ancien_masque, NULL);

    /* Lecture de la réponse */
    read(fd2, &r, sizeof(Reponse));

    /* Envoi du signal SIGUSR1 au serveur */
    kill(r.pid_serveur, SIGUSR1);

    /* Traitement local de la réponse */
    printf("Client %d a reçu :", getpid());

    for (i = 0; i < r.n; i++) {
        printf(" %d", r.tab[i]);
        somme += r.tab[i];
    }

    printf("\nSomme = %d\n", somme);

    close(fd1);
    close(fd2);

    return 0;
}

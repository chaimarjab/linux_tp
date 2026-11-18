volatile sig_atomic_t reveil = 0;   /* passe à 1 quand le client a acquitté */

/* Réveil du serveur : SIGUSR1 reçu du client */
void hand_reveil(int sig)
{
    reveil = 1;
    signal(SIGUSR1, hand_reveil);
}

/* Fin du serveur : signal quelconque (SIGINT, SIGTERM...) */
void fin_serveur(int sig)
{
    printf("\nFin du serveur (signal %d)\n", sig);
    unlink(FIFO1);
    unlink(FIFO2);
    exit(0);
}

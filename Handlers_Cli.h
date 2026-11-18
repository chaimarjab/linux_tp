volatile sig_atomic_t reveil = 0;   /* passe à 1 quand la réponse est prête */

/* Réveil du client : SIGUSR1 reçu du serveur */
void hand_reveil(int sig)
{
    reveil = 1;
    signal(SIGUSR1, hand_reveil);
}

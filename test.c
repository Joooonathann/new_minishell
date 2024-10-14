# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <linux/limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdarg.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

volatile sig_atomic_t g_signal = 0;

void handler(int signal)
{
    if (signal == SIGINT)
    {
        g_signal = 1;
        // Si readline est bloqué, le programme s'exécute à ce niveau
        // Utilisez les fonctions readline pour gérer cela
        write(1, "\n", 1); // Écrit une nouvelle ligne
    }
}

int main(void)
{
    signal(SIGINT, handler); // Configure le gestionnaire de signaux

    while (1)
    {
        char *input = readline("myfuckingbash@42$ ");
        
        if (g_signal)
        {
            g_signal = 0; // Réinitialise le signal
           	exit(1);
        }

        if (!input)
        {
            printf("exit\n");
            break; // Sortie si readline retourne NULL
        }

        // Traitez l'entrée ici
        add_history(input); // Ajoute l'entrée à l'historique
        free(input); // Libère l'espace mémoire alloué par readline
    }
    return 0;
}

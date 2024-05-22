#include <iostream>
#include <poll.h>
#include <unistd.h>
#include <vector>

/*

POLLIN
    Il y a des données en attente de lecture.
POLLPRI
    Il y a des données urgentes en attente de lecture (par exemple, des données « hors bande » sur une socket TCP ; un pseudo-terminal maître en mode paquet a vu l'esclave changer d'état).
POLLOUT
    Une écriture ne bloquera pas.
POLLRDHUP (depuis Linux 2.6.17)
    Le correspondant sur une socket en mode flux a fermé la connexion, ou bien a été mis hors service lors de l'écriture sur la connexion. La macro de test de fonctionnalité _GNU_SOURCE doit être définie pour obtenir cette définition.
POLLERR
    Condition d'erreur (uniquement en sortie).
POLLHUP
    Déconnexion (uniquement en sortie).
POLLNVAL
    Requête invalide : fd n'est pas ouvert (uniquement en sortie).

*/

typedef struct pollfd  pollfd;

int main(void)
{
    char    name[256];
    int     counter = 0;
    pollfd  fd;

    memset(&fd, 0, sizeof(fd));
    fd.fd = 0;                 // On surveille l'entrée standard (donc le fd 0)
    // POLLIN : Il y a des données en attente de lecture.
    fd.events = POLLIN;
    counter = 1;
    std::cout << "Your name : " << std::endl;
    while (1)
    {
        int status = poll(&fd, 1, 100); // -1 pour bloquer indefiniment
        // std::cout << "poll() = " << status << std::endl; // pour verif que ce n'est pas 0
        if (status == 1)
        {
            read(0, name, sizeof(name));
            std::cout << "My name is " << name << std::endl;
            break;
        }
        else
        {
            counter++;
        }
    }
    std::cout << "Counter : " << counter << std::endl;
    return (0);
}

// DESOLE JE DOIS ALLER CHERCHER MON FRERE A LECOLE  JARRIVE !!!!!!
// je vais en profiter pour aller manger alors okokokokokok
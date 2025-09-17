
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstring>

using namespace std;

#define MAX_COMMANDES 1000

struct Commande {
    int numero;
    string commande;
    int pid;
};

vector<Commande> historique;
int compteur = 0;

void afficherHistorique() {
    cout << "Dernières commandes :\n";
    ofstream out("historique.txt", ios::trunc);
    for (int i = historique.size() - 1; i >= 0 && i > int(historique.size()) - 6; i--) {
        cout << historique[i].numero << "\t" << historique[i].commande << "\t" << historique[i].pid << "\n";
        out << historique[i].numero << "\t" << historique[i].commande << "\t" << historique[i].pid << "\n";
    }
    out.close();
}

int main() {
    string input;
    char* argv[4]; // commande, option, NULL, (possiblement & à gérer)

    while (true) {
        cout << "KABAKARAMO";
        getline(cin, input);

        if (input == "stop") break;

        // Vérifie si on veut afficher l'historique
        if (input == "historique") {
            afficherHistorique();
            continue;
        }

        // Découpage de la commande
        istringstream iss(input);
        string part;
        vector<string> parts;

        while (iss >> part) parts.push_back(part);

        // Gestion du & (background)
        bool background = false;
        if (!parts.empty() && parts.back() == "&") {
            background = true;
            parts.pop_back();  // On retire le &
        }

        // Préparation des arguments pour execvp
        for (size_t i = 0; i < parts.size(); ++i)
            argv[i] = strdup(parts[i].c_str());
        argv[parts.size()] = NULL;

        pid_t pid = fork();
        if (pid == 0) {
            // Processus enfant
            execvp(argv[0], argv);
            perror("Erreur execvp");
            exit(1);
        } else if (pid > 0) {
            // Processus parent
            compteur++;
            historique.push_back({compteur, input, pid});
            if (!background)
                waitpid(pid, NULL, 0);
        } else {
            perror("Erreur fork");
        }

        // Libération des strdup
        for (size_t i = 0; i < parts.size(); ++i)
            free(argv[i]);
    }

    cout << "Terminal fermé.\n";
    return 0;
}
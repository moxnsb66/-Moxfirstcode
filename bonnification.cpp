#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <sys/wait.h>
#include <sstream>
#include <cstring>

using namespace std;

vector<string> commandes = {
    "ls", "ls -a", "ls -l",
    "pwd",
    "mkdir testdir",
    "rmdir testdir",
    "cat terminal.cpp"
};

string nomHistorique(int instance, int compteur) {
    return "historique" + to_string(instance) + "_" + to_string(compteur) + ".txt";
}

string nomFichierComplet(int instance) {
    return "commandes_instance" + to_string(instance) + ".txt";
}

void executerCommandes(int instance, int totalCommandes, int freqHistorique) {
    ofstream toutesCommandes(nomFichierComplet(instance));
    vector<string> historique;
    srand(time(0) + instance);  // pour éviter mêmes séquences

    for (int i = 1; i <= totalCommandes; ++i) {
        string commande = commandes[rand() % commandes.size()];
        historique.push_back(commande);
        toutesCommandes << i << " " << commande << endl;

        // Split commande
        vector<string> args;
        string arg;
        istringstream iss(commande);
        while (iss >> arg) args.push_back(arg);

        // Prepare argv
        char* argv[10];
        for (size_t j = 0; j < args.size(); ++j)
            argv[j] = strdup(args[j].c_str());
        argv[args.size()] = NULL;

        pid_t pid = fork();
        if (pid == 0) {
            execvp(argv[0], argv);
            perror("Erreur execvp");
            exit(1);
        } else {
            waitpid(pid, NULL, 0);
        }

        for (size_t j = 0; j < args.size(); ++j)
            free(argv[j]);

        // Sauvegarde de l’historique tous les freqHistorique
        if (i % freqHistorique == 0) {
            ofstream histo(nomHistorique(instance, i));
            for (int j = i - freqHistorique + 1; j <= i && j <= historique.size(); ++j) {
                histo << j << " " << historique[j - 1] << endl;
            }
            histo.close();
        }
    }

    toutesCommandes.close();
}

int main() {
    clock_t start1 = clock();
    executerCommandes(1, 100, 50);  // Instance 1 : 100 commandes
    clock_t end1 = clock();
    double temps1 = double(end1 - start1) / CLOCKS_PER_SEC;

    clock_t start2 = clock();
    executerCommandes(2, 500, 100); // Instance 2 : 500 commandes
    clock_t end2 = clock();
    double temps2 = double(end2 - start2) / CLOCKS_PER_SEC;

    cout << "Temps d'exécution instance 1 : " << temps1 << " secondes" << endl;
    cout << "Temps d'exécution instance 2 : " << temps2 << " secondes" << endl;

    return 0;
}
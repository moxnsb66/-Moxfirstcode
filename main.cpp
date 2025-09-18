
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

int main() {
    pid_t pid;
    pid = fork();

    if (pid == 0) {
        // Code exécuté par l’enfant
        printf("Je suis l’enfant (PID=%d)\n", getpid());
    } else {
        // Code exécuté par le parent
        printf("Je suis le parent (PID=%d)\n", getpid());
    }
    return 0;
}
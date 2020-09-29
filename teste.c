#include <stdio.h> 
#include <pthread.h> 
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

int main () {
    time_t start, agora;
    double tempo;

    time (&start);

    while (1) {
        time (&agora);
        tempo = difftime (agora, start);

        if (tempo == floor (tempo)) {
            printf ("Tempo decorrido: %lf\n", tempo);
            usleep (1000000);
        }
    }

    return 0;
}
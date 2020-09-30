#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>

#define quantum 50000

pthread_mutex_t semaforo;
bool finished = false;
bool finishedDef = false;

void * threadFunc (void * arguments) {
    int aux = 0;
    while (finishedDef == false && !pthread_mutex_lock (&semaforo)) {
        printf ("entrei!\n");
        finished = false;
        while (finished == false) {
            aux ++;
            aux --;
        }
        printf ("SAI!\n");
    }
    
    printf ("terminei");
    return NULL;
}

void * escFunc (void * arguments) {
    pthread_t thread;
    pthread_mutex_init (&semaforo, NULL);
    pthread_mutex_lock (&semaforo);
    pthread_create (&thread, NULL, threadFunc, NULL);
    
    int aux = 0;
    int count = 0;

    while (aux < 1) {
        pthread_mutex_unlock (&semaforo);
        while (count < 5) {
            usleep (quantum);
            count ++;
        }
        
        finished = true;
        count = 0;
        aux++;
    }

    finishedDef = true;
    return NULL;
}

int main(void) {
    pthread_t escalonador;
    
    /* pthread_create (&escalonador, NULL, escFunc, NULL);
    pthread_join (escalonador, NULL);
    
    pthread_mutex_destroy (&semaforo);

    printf ("%f\n", (float) quantum / 1000000); */

    float a = 1.5, relogio = 0;
    int b = 1;

    if (b == floor(a))
        printf ("são iguais\n");
    else 
        printf ("são diferentes\n");
    
    while (1) {
        relogio += 0.05;
        if (relogio == floor (relogio))
            printf ("aaaaa\n");
        else
        {
            printf("shiiiiiiiiiiiit\n");
        }
        
    }

    return 0;
}
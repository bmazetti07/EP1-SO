#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

#include "header.h"

#define maxProcessos 50 + 7
#define clockTime 1
#define sleep_time 0.1

// VARIÁVEIS GLOBAIS
Processo processos[maxProcessos];
bool finished[maxProcessos] = { 0 };
int tempos[maxProcessos];
int nProc = 0;
int relogio = 0;

// SEMÁFOROS
pthread_mutex_t sem[maxProcessos];
pthread_mutex_t semEscalonador;

// LÊ O ARQUIVO DE TRACE
void readFile (char * filename) {
    FILE * trace;
    trace = fopen (filename, "r");
               
    while (!feof (trace)) {
        if (fscanf (trace, "%s %d %d %d", processos[nProc].nome, &processos[nProc].start, &processos[nProc].dt, &processos[nProc].deadline) != 4)
            continue;
        printf ("%s %d %d %d\n", processos[nProc].nome, processos[nProc].start, processos[nProc].dt, processos[nProc].deadline);
        tempos[processos[nProc].start] ++;
        nProc ++;
    }

    fclose (trace);
}

// FUNÇÃO PARA CADA UM DOS PROCESSOS
void * work (void * parameters) {
    Arguments parameter = *( (Arguments * ) parameters);
    int id = parameter.id;
    int type = parameter.escType;

    int cont = 0;
    int aux;
    float time = processos[id].dt;
    
    while (time > 0) {
        aux = 10000 * 10000;
        cont ++;
        time = time - sleep_time;
        usleep (sleep_time * 1000000);

        if (type == 1) {
            if (cont % 10 == 0) {
                pthread_mutex_unlock (&semEscalonador);
                pthread_mutex_lock (&sem[id]);
                relogio ++;
                cont = 0;
            }
        }
    }

    pthread_mutex_unlock (&semEscalonador);
    finished[id] = true;
      
    return NULL;
}

// ESCALONADOR FIRST-COME FIRST-SERVED
void * fcfs (void * arguments) {
    pthread_mutex_lock (&semEscalonador);
    pthread_t threads[nProc];
    Arguments parameters[nProc];

    for (int i = 0; i < nProc; i ++) {
        parameters[i].id = i;
        parameters[i].escType = 1;

        if (pthread_create (&threads[i], NULL, work, (void *) &parameters[i])){
            printf ("ERRO ao criar a thread %d!\n", i);
            exit (1);
        }
    
        while (finished[i] == false) {
            pthread_mutex_lock (&semEscalonador);
            pthread_mutex_unlock (&sem[i]);
        }

        if (pthread_join (threads[i], NULL)) {
            printf ("ERRO ao esperar o término da thread %d!\n", i);
            exit (1);
        }

        pthread_detach (threads[i]);
    }

    return NULL;
}

// ESCALONADOR SHORTEST REMAINING TIME NEXT
void * srtn (void * arguments) {
    pthread_mutex_lock (&semEscalonador);

    createLista ();
    
    
    



}

// INICIALIZA A THREAD DO ESCALONADOR
void escInit (int type) {
    pthread_t escalonador;

    // incializa os semáforos das threads lockados, menos o do escalonador
    pthread_mutex_init (&semEscalonador, NULL);
    for (int i = 0; i < nProc; i ++) {
        pthread_mutex_init (&sem[i], NULL);
        pthread_mutex_lock (&sem[i]);
    }

    // cria a thread do escalonador
    if (type == 1)
        pthread_create (&escalonador, NULL, fcfs, NULL);
    else if (type == 2)
        pthread_create (&escalonador, NULL, srtn, NULL);
 
    // espera até o fim da execução da thread do escalonador
    if (pthread_join (escalonador, NULL)) {
        printf ("ERRO ao entrar na thread do escalonador!\n");
        exit (1);
    };

    printf ("acabou em %d\n", relogio);

    // destrói os semáforos
    pthread_mutex_destroy (&semEscalonador);
    for (int i = 0; i < nProc; i ++) 
        pthread_mutex_destroy (&sem[i]);
}

int main (int argc, char **argv) {
    int type = atoi (argv[1]);
    readFile (argv[2]);

    escInit (type);
  
    return 0;
}
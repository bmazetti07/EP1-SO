#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#include "header.h"

#define maxProcessos 50 + 7
#define clockTime 1
#define sleep_time 0.1

// VARIÁVEIS GLOBAIS
Processo processos[maxProcessos];
bool finished = false;
bool created = false;
bool finishedLastProc = false;
int tempos[maxProcessos];
int nProc = 0;
int relogio = 0;
int procAtual = 0;

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
    // Arguments parameter = *( (Arguments * ) parameters);
    // int id = parameter.id;
    // int type = parameter.escType;
    int aux = 0;

    created = true;

    while (finished == false) {
        aux ++;
        aux --;
    }

    return NULL;
}

// THREAD PARA TIMER GLOBAL
void * timer (void * arguments) {
    time_t start, agora;
    double tempo;
    int segundo;

    time (&start);

    while (!finishedLastProc) {
        time (&agora);
        tempo = difftime (agora, start);

        if (tempo == floor (tempo)) {
            pthread_mutex_unlock (&semEscalonador);
            usleep (1000000);
            relogio ++;
        }
    }

    return NULL;
}

// ESCALONADOR FIRST-COME FIRST-SERVED
void * fcfs (void * arguments) {
    pthread_t thread;
    Processo aux;
    int fimTimeProc = 0;

    createFila ();
    while (procAtual <= nProc) {
        if (finishedLastProc)
            break;

        pthread_mutex_lock (&semEscalonador);
    
        while (tempos[relogio]) {
            queue (processos[procAtual]);
            procAtual ++;
            tempos[relogio] --;
        }

        // printf ("relogio = %d\n", relogio);
        // print ();

        if (emptyFila () == 0) {
            if (created == true && relogio == fimTimeProc) {
                finished = true;
                created = false;
                aux = dequeue ();

                if (strcmp (aux.nome, processos[nProc - 1].nome) == 0)
                    finishedLastProc = true;

                if (pthread_join (thread, NULL)) {
                    printf ("ERRO ao esperar o término da thread!");
                    exit (1);
                }
                pthread_detach (thread);
            }

            if (created == false) {
                finished = false;

                if (pthread_create (&thread, NULL, work, NULL)) {
                    printf ("ERRO ao criar a thread!");
                    exit (1);
                }

                aux = getIni ();
                fimTimeProc = aux.dt + relogio;
            }
        }
    }

    freeFila ();
    return NULL;
}

// ESCALONADOR SHORTEST REMAINING TIME NEXT
void * srtn (void * arguments) {
    pthread_mutex_lock (&semEscalonador);

    createLista ();
}

// INICIALIZA A THREAD DO ESCALONADOR E DO TIMER
void escInit (int type) {
    pthread_t escalonador;
    pthread_t timerThread;

    // incializa os semáforos das threads lockados
    pthread_mutex_init (&semEscalonador, NULL);
    pthread_mutex_lock (&semEscalonador);
    for (int i = 0; i < nProc; i ++) {
        pthread_mutex_init (&sem[i], NULL);
        pthread_mutex_lock (&sem[i]);
    }

    // cria a thread do escalonador e para o timer
    pthread_create (&timerThread, NULL, timer, NULL);

    if (type == 1)
        pthread_create (&escalonador, NULL, fcfs, NULL);
    else if (type == 2)
        pthread_create (&escalonador, NULL, srtn, NULL);
 
    // espera até o fim da execução da thread do escalonador e do timer
    if (pthread_join (escalonador, NULL)) {
        printf ("ERRO ao entrar na thread do escalonador!\n");
        exit (1);
    };

    if (pthread_join (timerThread, NULL)) {
        printf ("ERRO ao entrar na thread do timer!\n");
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
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#include "header.h"

#define maxProcessos 1000 + 7
#define clockTime 1
#define sleep_time 0.1

// VARIÁVEIS GLOBAIS DESTINADOS
Processo processos[maxProcessos];                   // Processos lidos do arquivo de trace
int tempos[100000];                                 // Número de processos existentes em cada t0
int nProc = 0;                                      // Número de processos totais
int procAtual = 0;                                  // Processo atuando no momento
int type = 0;                                       // Tipo do escalonador

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
        processos[nProc].id = nProc;
        processos[nProc].created = false;
        processos[nProc].finishedDef = false;
        processos[nProc].finishedOp = false;
        processos[nProc].runCount = 0.0;
        tempos[processos[nProc].start] ++;
        nProc ++;
    }

    fclose (trace);
}

// FUNÇÃO PARA CADA UM DOS PROCESSOS
void * work (void * parameters) {
    int index = *( (int *) parameters);
    int op = 0;

    if (type == 1) {
        while (processos[index].finishedDef == false) {
            op ++;
            op --;
        }
    }

    else if (type == 2) {
        while (processos[index].finishedDef == false && !pthread_mutex_lock (&sem[index])) {
            processos[index].finishedOp = false;
            while (processos[index].finishedOp == false) {
                op ++;
                op --;
            }
        }
    }

    else if (type == 3) {
        while (processos[index].finishedDef == false && !pthread_mutex_lock (&sem[index])) {
            processos[index].finishedOp = false;
            while (processos[index].finishedOp == false) {
                op ++;
                op --;
            }
        }
    }
    
    return NULL;
}

// ESCALONADOR FIRST-COME FIRST-SERVED
void * fcfs (void * arguments) {
    pthread_t thread;
    Processo aux;
    int relogio = 0;
    int procFinalizados = 0;
    int id;

    createFila ();
    while (procFinalizados != nProc) {
        while (procAtual < nProc && tempos[relogio]) {
            queue (processos[procAtual]);
            procAtual ++;
            tempos[relogio] --;
        }

        if (!emptyFila ()) {
            aux = getIniFila ();
            id = aux.id;

            if (processos[id].created == true && (int) processos[id].runCount >= processos[id].dt) {
                processos[id].finishedDef = true;

                if (pthread_join (thread, NULL)) {
                    printf ("ERRO ao esperar o término da thread!");
                    exit (1);
                }

                pthread_detach (thread);
                procFinalizados ++;
            
                dequeue ();
                aux = getIniFila ();
                id = aux.id;

                if (procFinalizados == nProc)
                    break;
            }

            if (processos[id].created == false) {
                if (pthread_create (&thread, NULL, work, (void *) &id)) {
                    printf ("ERRO ao criar a thread!");
                    exit (1);
                }

                processos[id].created = true;
            }

            usleep (1000000);
            relogio ++;
            processos[id].runCount ++;
        }

        else {
            usleep (1000000);
            relogio ++;
        }
    }

    freeFila ();
    return NULL;
}

// ESCALONADOR SHORTEST REMAINING TIME NEXT
void * srtn (void * arguments) {
    pthread_t threads[nProc];
    Processo aux, running;
    bool newEntry = false;
    bool firstExec = true;
    bool changed = true;
    int relogio = 0;
    int procFinalizados = 0;
    int id;

    createFila ();
    while (procFinalizados != nProc) {
        while (procAtual < nProc && tempos[relogio]) {
            queue (processos[procAtual]);
            procAtual ++;
            tempos[relogio] --;
            newEntry = true;
        }
        printf ("Relogio == %d\n", relogio);
        printf ("Antes do sort: ");
        printFila ();

        if (newEntry) 
            sortFila (processos);
        if (firstExec && newEntry) {
            running = getIniFila ();
            firstExec = false;
        }

        printf ("Depois do sort: ");
        printFila ();

        if (!emptyFila ()) {
            aux = getIniFila ();
            id = aux.id;

            if (newEntry == false)
                changed = false;

            else if (newEntry == true && strcmp (aux.nome, running.nome) != 0) {
                processos[running.id].finishedOp = true;

                running = aux;
                changed = true;
            }
            
            if (processos[id].created == false) {
                if (pthread_create (&threads[id], NULL, work, (void *) &id)) {
                    printf ("ERRO ao criar a thread!");
                    exit (1);
                }

                processos[id].created = true;
                changed = true;
            }

            if (changed == true)
                pthread_mutex_unlock (&sem[id]);
            
            usleep (1000000);
            relogio ++;
            processos[id].runCount ++;

            if ((int) processos[id].runCount >= processos[id].dt) {
                processos[id].finishedDef = true;
                processos[id].finishedOp = true;

                if (pthread_join (threads[id], NULL)) {
                    printf ("ERRO ao esperar o término da thread %d!\n", id);
                    exit (1);
                }

                pthread_detach (threads[id]);
                procFinalizados ++;

                changed = true;
                dequeue ();
            }
        }

        else {
            usleep (1000000);
            relogio ++;
        }

        newEntry = false;
    }

    freeFila ();
    return NULL;
}

// ESCALONADOR ROUND-ROBIN
void * rr (void * arguments) {
    pthread_t threads[nProc];
    Processo aux;
    int quantum = 50000;
    int procFinalizados = 0;
    int relogio = 0;
    int cont = 0;
    int interval = 1000000 / quantum;

    createFila ();
    while (procFinalizados != nProc) {
        if (cont % interval == 0) {
            while (procAtual < nProc && tempos[relogio] != 0) {
                queue (processos[procAtual]);
                procAtual ++;
                tempos[relogio] --;
            }
        }

        if (!emptyFila ()) {
            aux = getIniFila ();

            if (processos[aux.id].created == false) {
                if (pthread_create (&threads[aux.id], NULL, work, (void *) &aux.id)) {
                    printf ("ERRO ao criar a thread %d!\n", aux.id);
                    exit (1);
                }
                
                processos[aux.id].created = true;
            }

            pthread_mutex_unlock (&sem[aux.id]);
        
            usleep (quantum);
            cont ++;
            if (cont % interval == 0)
                relogio ++;

            processos[aux.id].runCount += 0.05;

            if ((int) processos[aux.id].runCount >= processos[aux.id].dt) {
                processos[aux.id].finishedDef = true;
                processos[aux.id].finishedOp = true;

                if (pthread_join (threads[processos[aux.id].id], NULL)) {
                    printf ("ERRO ao esperar o término da thread %d!\n", processos[aux.id].id);
                    exit (1);
                }

                pthread_detach (threads[processos[aux.id].id]);
                procFinalizados ++;
            }

            processos[aux.id].finishedOp = true;
            if (processos[aux.id].finishedDef == false)
                queue (dequeue ());
            else
                dequeue ();
        }

        else {
            usleep (quantum);
            cont ++;
            if (cont % interval == 0)
                relogio ++;
        }
    }

    freeFila ();
    return NULL;
}

// INICIALIZA A THREAD DO ESCALONADOR
void escInit (int type) {
    pthread_t escalonador;

    // incializa os semáforos das threads lockados
    pthread_mutex_init (&semEscalonador, NULL);
    pthread_mutex_lock (&semEscalonador);
    for (int i = 0; i < nProc; i ++) {
        pthread_mutex_init (&sem[i], NULL);
        pthread_mutex_lock (&sem[i]);
    }

    if (type == 1)
        pthread_create (&escalonador, NULL, fcfs, NULL);
    else if (type == 2)
        pthread_create (&escalonador, NULL, srtn, NULL);
    else if (type == 3)
        pthread_create (&escalonador, NULL, rr, NULL);
 
    // espera até o fim da execução da thread do escalonador
    if (pthread_join (escalonador, NULL)) {
        printf ("ERRO ao aguardar o término da thread do escalonador!\n");
        exit (1);
    };

    // destrói os semáforos
    pthread_mutex_destroy (&semEscalonador);
    for (int i = 0; i < nProc; i ++) 
        pthread_mutex_destroy (&sem[i]);
}

int main (int argc, char **argv) {
    type = atoi (argv[1]);
    readFile (argv[2]);

    escInit (type);
  
    return 0;
}
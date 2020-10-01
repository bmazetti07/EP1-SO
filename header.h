#ifndef HEADER_H
#define HEADER_H

// REPRESENTAÇÃO DE CADA PROCESSO
typedef struct processo {
    char nome[30];
    int id;                 // Nome do processo
    int start;              // t0
    int dt;                 // Tempo que o processo deve rodar
    int deadline;           // Tempo final suposto do processo
    float runCount;         // Tempo de execução do processo no momento
    bool created;           // Indica se o processo já foi criado
    bool finishedDef;       // Indica se o processo já foi finalizado
    bool finishedOp;        // Indica se as operações do processo devem cessar ou não
} Processo;

/* Fila usada nos escalonadores Round Robin, SRTN e FCFS*/

Processo * fila;

int iniFila;

int fimFila;

int tamFila;

int nFila;

void createFila ();

void freeFila ();

void resizeFila ();

void queue (Processo proc);

Processo dequeue ();

Processo getIniFila ();

int emptyFila ();

void printFila ();

int compara (const void * a, const void * b);

void sortFila (Processo *v);

void diminuiDt (int id, int val);

#endif
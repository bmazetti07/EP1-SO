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

// ARGUMENTOS FORNECIDOS À FUNÇÃO WORK()
typedef struct arguments{
    int id;
    int escType;
} Arguments;

/* Fila usada nos escalonadores Round Robin e FCFS*/

Processo * fila;

int ini;

int fim;

int tamFila;

int nFila;

void createFila ();

void freeFila ();

void resizeFila ();

void queue (Processo proc);

Processo dequeue ();

Processo getIni ();

int emptyFila ();

void printFila ();


/* Estrutura usada no escalonador SRTN */

Processo * lista;

int tamLista;

int nLista;

void createLista ();

void freeLista ();

void resizeLista ();

void put (Processo proc);

void take ();

void sortLista ();

int emptyLista ();



#endif
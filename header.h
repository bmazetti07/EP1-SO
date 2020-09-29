#ifndef HEADER_H
#define HEADER_H

// REPRESENTAÇÃO DE CADA PROCESSO
typedef struct processo {
    char nome[30];
    int start;
    int dt;
    int deadline;
} Processo;

// ARGUMENTOS FORNECIDOS À FUNÇÃO WORK()
typedef struct arguments{
    int id;
    int escType;
} Arguments;

/* Fila usada no escalonador Round Robin*/

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

int emptyFila ();


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
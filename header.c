#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "header.h"

/* Implementação fila usada no Round Robin */

void createFila () {
    fila = malloc (57 * sizeof (Processo));
    
    tamFila = 57;
    iniFila = fimFila = 0;
    nFila = 0;
}

void freeFila () {
    free (fila);
}

void resizeFila () {
    Processo * novo = malloc (5 * tamFila);

    for (int i = 0; i < nFila; i ++)
        novo[i] = fila[(iniFila + i)%tamFila];
    
    freeFila ();

    iniFila = 0;
    fimFila = nFila;

    tamFila *= 5;

    fila = novo;
}

void queue (Processo proc) {
    if ((fimFila + 1) % tamFila == iniFila)
        resizeFila ();
    
    fila[fimFila] = proc;

    fimFila = (fimFila + 1) % tamFila;

    nFila ++;
}

Processo dequeue () {
    Processo aux;

    nFila --;
    aux = fila[iniFila];
    iniFila = (iniFila + 1) % tamFila;

    return aux;
}

Processo getIniFila () {
    return (fila[iniFila]);
}

int emptyFila () {
    if (iniFila == fimFila)
        return 1;
    
    return 0;
}

void printFila () {
    printf ("Fila: ");
    for (int i = iniFila; i != fimFila; i = (i + 1) % tamFila) {
        printf (" %s ", fila[i].nome);
    }
    printf ("\n");
}

int compara (const void * a, const void * b) {
    Processo * x = (Processo *) a;
    Processo * y = (Processo *) b; 
    return ((x -> dt) - (y -> dt));
}


void sortFila (Processo *v) {
    //Processo aux;

    /* for (int i = iniFila + 1; i < nFila; i = (i + 1) % tamFila) {
        for (int j = iniFila; j < nFila - i; j = (j + 1) % tamFila) {
            if ((fila[j].dt - v[fila[j].id].runCount) > (fila[j + 1].dt - v[fila[j + 1].id].runCount)) {
                aux = fila[j];
                fila[j] = fila[j + 1];
                fila[j + 1] = aux;
            }
        }
    } */
    qsort (fila + iniFila, fimFila - iniFila, sizeof (Processo), compara);
}

void diminuiDt (int id, int val) {
    fila[id].dt -= val;
}

/* Implementação da estrutura usada no SRTN */

/* void createLista () {
    lista = malloc (57 * sizeof (Processo));

    tamLista = 57;
}

void freeLista () {
    free (lista);
}

void resizeLista () {
    lista = (Processo *) realloc (lista, tamLista * 5);
    tamLista *= 5;
}

void put (Processo proc) {
    if ((fimLista + 1) % tamLista)
        resizeLista ();

    lista[nLista + 1] = proc;

    nLista ++;
}

void take () {
    for (int i = 0; i < nLista; i ++)
        lista[i] = lista[i+1];
    
    nLista --;
}

Processo getIniLista () {
    return (lista[0]);
} */
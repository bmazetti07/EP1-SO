#include <stdlib.h>
#include <stdio.h>
#include "header.h"

/* Implementação fila usada no Round Robin */

void createFila () {
    fila = malloc (57 * sizeof (Processo));
    
    tamFila = 57;
    ini = fim = 0;
    nLista = 0;
}

void freeFila () {
    free (fila);
}

void resizeFila () {
    /* int * novo = malloc (5 * tamFila);

    for (int i = 0; i < tamFila; i ++)
        novo[i] = fila[i];
    
    freeFila ();

    tamFila *= 5;

    fila = novo; */
    fila = (Processo *) realloc (fila, tamFila * 5);

    tamFila *= 5;
}

void queue (Processo proc) {
    if (fim == tamFila)
        resizeFila ();
    
    fila[fim++] = proc;

    nFila ++;
}

Processo dequeue () {
    nFila --;

    return fila[ini++];
}

int emptyFila () {
    if (ini == fim)
        return 1;
    
    return 0;
}


/* Implementação da estrutura usada no SRTN */

void createLista () {
    lista = malloc (57 * sizeof (Processo));

    tamLista = 57;
}

void freeLista () {
    free (lista);
}

void resizeLista () {
    lista = (Processo *) realloc (lista, tamLista * 5);
}

void put (Processo proc) {
    lista[nLista + 1] = proc;

    nLista ++;
}

void take () {
    for (int i = 0; i < nLista; i ++)
        lista[i] = lista[i+1];
    
    nLista --;
}

void sortLista () {
    Processo aux;

    for (int i = 2; i < nLista; i++)
      for (int j = 1; j < nLista - i; j++) {
         if (lista[j].dt > lista[j + 1].dt) {
            aux = lista[j];
            lista[j] = lista[j + 1];
            lista[j + 1] = aux;
         }
      }
}
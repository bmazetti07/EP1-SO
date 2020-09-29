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
    Processo * novo = malloc (5 * tamFila);

    for (int i = 0; i < nFila; i ++)
        novo[i] = fila[(ini + i)%tamFila];
    
    freeFila ();

    ini = 0;
    fim = nFila;

    tamFila *= 5;

    fila = novo;
}

void queue (Processo proc) {
    if ((fim + 1) % tamFila == ini)
        resizeFila ();
    
    fila[fim] = proc;

    fim = (fim + 1) % tamFila;

    nFila ++;
}

Processo dequeue () {
    Processo aux;

    nFila --;
    aux = fila[ini];
    ini = (ini + 1) % tamFila;

    return aux;
}

Processo getIni () {
    return fila[ini];
}

int emptyFila () {
    if (ini == fim)
        return 1;
    
    return 0;
}

void printFila () {
    printf ("Fila: ");
    for (int i = ini; i != fim; i = (i + 1) % tamFila) {
        printf (" %s ", fila[i].nome);
    }
    printf ("\n");
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
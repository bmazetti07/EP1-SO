#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int time_geral = 0;
int tempos[50], dts[50], deadlines[50], starts[50], nProc = 0;
char processos[10000][30];

void * work (void * arguments) {
    int index = *( (int *) arguments);
    // printf ("index = %d\n", index);
    float time = dts[index];
    int aux;
    
    while (time > 0) {
        aux = 10000 * 10000;
        aux ++;
        time = time - 0.1;
        usleep (100000);
    }
      
    return NULL;
}

void readFile (char * filename) {
    FILE * trace;
    trace = fopen (filename, "r");
               
    while (!feof (trace)) {
        if (fscanf (trace, "%s %d %d %d", processos[nProc], &starts[nProc], &dts[nProc], &deadlines[nProc]) != 4)
            continue;
        printf ("%s %d %d %d\n", processos[nProc], starts[nProc], dts[nProc], deadlines[nProc]);
        tempos[starts[nProc]] ++;
        nProc ++;
    }

    fclose (trace);
}

void fcfs () {
    for (int i = 0; i < nProc; i ++) {
        pthread_t thread;
        int thread_args[nProc];
        thread_args[i] = i;
        
        if (pthread_create (&thread, NULL, work, &thread_args[i])) {
            printf ("\n ERRO na criação da thread!");
            exit (1);
        }

        if (pthread_join (thread, NULL)) {
            printf ("\n ERRO ao entrar na thread!");
            exit (1);
        }
    }
}

void srtn () {
    



}

int main (int argc, char **argv) {
    int type = atoi (argv[1]);
    readFile (argv[2]);
  
    if (type == 1) {
        fcfs ();
    }

    else if (type == 2) {
        srtn ();
    }
  
    return 0;
}
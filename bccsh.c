#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void type_prompt () {
    printf ("{Hirotroxa@diretório} ");
}

int main (int argc, char **argv) {
    char *command, *parameters;
    while (1) {
        type_prompt ();
        scanf ("%s", command);

        if (strcmp (command, "mkdir") == 0) {

        }

        else if (strcmp (command, "kill") == 0) {

        }

        else if (strcmp (command, "ln") == 0) {

        }

        else {
            /* if (fork() != 0) {
                waitpid(-1,&status,0);
            } 
            
            else {
                execve(command,parameters,0);
            } */
            printf ("lalaalalal\n");
        }


    }
}
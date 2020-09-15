#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>

void type_prompt () {
    printf ("{Hirotroxa@diretório} ");
}

int main (int argc, char **argv) {
    char command[50], parameters[50];

    while (1) {
        type_prompt ();
        scanf ("%s %s", command, parameters);

        if (strcmp (command, "mkdir") == 0)
            mkdir (parameters, 0777);

        else if (strcmp (command, "kill") == 0) {
            pid_t pid;
            scanf ("%d", &pid);

            kill (pid, SIGKILL);
        }

        else if (strcmp (command, "ln") == 0) {
            char file[20], shortcut[20];
            scanf ("%s %s", file, shortcut);

            symlink (file, shortcut);
        }

        else {
            char * argumentos[] = { " ", parameters, NULL };
            char * envp[] = { NULL };

            if (fork() != 0) {
                waitpid(-1, NULL, 0);
            }
            
            else {
                execve (command, argumentos, envp);
            }
        }
    }
}
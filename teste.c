#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

void type_prompt (char * prompt) {
    char * username = NULL;
    char * path = NULL;

    username = getlogin ();
    path = getcwd (path, 0);

    sprintf(prompt, "{%s@%s} ", username, path);
}

int main (int argc, char **argv) {
    char prompt[150];

    type_prompt (prompt);
    printf("%s\n", prompt);
}
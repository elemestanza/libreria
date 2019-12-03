#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "parser.h"

int main(){

    	//Declaración de variables
    	char buf[1024];
    	int control = 1;
   	tline * line;

    	//Obtención de línea
    	while (control != 0){
        	printf("msh> ");
        	scanf("%s", buf);
        	line = tokenize(buf);

        	if (line == NULL) control = 0;
        	else printf("\n");
    	}

	//Segunda declaración de variables
	int i, j;
	int *pipes[2];
	pid_t pid, input, output, error;
	int numcomandos = line->ncommands;

	pipes[i] = malloc ((numcomandos - 1) * sizeof(int[2]));
	for (i = 0; i < numcomandos-1; i++) pipe(pipes[i]);

	//Diferenciar caso de 1 comando a más de 1

	if (numcomandos == 1){ //Un comando

		pid = fork();
		if (pid == 0){
			execvp(line->commands[0].argv[0], line->commands[0].argv + 1);
			exit(1);
		} else{
			wait(NULL);
			exit(0);
		}

	} else { //Más de un comando
		pid = fork();
		if (pid == 0){ //primero
			close(pipes[0][0]);
			dup2(pipes[0][1], 1);
			for (i = 1; i < numcomandos-2; i++){
				close(pipes[i][0]);
				close(pipes[i][1]);
			}

			//input = open(line->redirect_input);
			//if (line->redirect_input != NULL) dup2(input, 0); //Redirección de entrada

			execvp(line->commands[0].argv[0], line->commands[0].argv + 1);
			exit(1);
		} else {
			for (i = 0; i < numcomandos-2; i++){ //los hijos del medio
				pid = fork();
				if (pid == 0){
					for (j = 0; j < i; j++){
						close(pipes[j][0]);
						close(pipes[j][1]);
					}
					dup2(pipes[i][0], 0);
					close(pipes[i][1]);
					close(pipes[i+1][0]);
					dup2(pipes[i+1][1], 1);
					for (j = i+2; j < numcomandos-2; j++){
						close(pipes[j][0]);
						close(pipes[j][1]);
					}

					execvp(line->commands[i+1].argv[0], line->commands[i+1].argv + 1);
					exit(1);
				}
			}
			pid = fork();
			if (pid == 0){ //último
				for (i = 0; i < numcomandos-2; i++){
					close(pipes[i][0]);
					close(pipes[i][1]);
				}
				dup2(pipes[numcomandos-2][0], 0);
				close(pipes[numcomandos-2][1]);

				execvp(line->commands[numcomandos-1].argv[0], line->commands[numcomandos-1].argv +1);
				exit(1);

				//output = open(line->redirect_output);
				//error = open(line->redirect_error);

				//if (line->redirect_output != NULL) dup2(output, 1); //Redirección de salida
				//if (line->redirect_error != NULL) dup2(error, 2); //Redirección de error
			} else { //padre
				for (i =0; i> numcomandos-1; i++){
					close(pipes[i][0]);
					close(pipes[i][1]);
				}

				for (i = 0; i < numcomandos; i++) wait(NULL); //Espera a todos los hijos
				exit(0);
			}
		}
	}
}

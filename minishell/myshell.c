#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "parser.h"

void cd(char *dir){
	if (dir == NULL) chdir(getenv("HOME"));
	else chdir(dir);
}

int main(void){

    	//Declaración de variables
    	char buf[1024];
   	tline * line;
	int i, j, numcomandos;
        pid_t pidunico, input, output, error;
        int **pipes;
        pid_t *pid;

    	printf("msh> ");
    	while (fgets(buf, 1024, stdin)){
        	line = tokenize(buf);

        	if (line == NULL) printf("\n");

		//Asignación de variables
		numcomandos = line->ncommands;
		input = open(line->redirect_input, O_RDONLY);
		output = open(line->redirect_output, O_WRONLY | O_CREAT | O_TRUNC, 0600);
		error = open(line->redirect_error, O_WRONLY | O_CREAT | O_TRUNC, 0600);

		//CD
		if (strcmp(line->commands[0].argv[0],"cd") == 0) cd(line->commands[0].argv[1]);

		// UN COMANDO
		if (numcomandos == 1){

			pidunico = fork();
			if (pidunico == 0){
				if (line->redirect_input != NULL) dup2(input, 0); //Redirección de entrada
				if (line->redirect_output != NULL) dup2(output, 1); //Redirección de salida
				if (line->redirect_error != NULL) dup2(error, 2); //Redirección de error

				execv(line->commands[0].filename, line->commands[0].argv);
				exit(1);
			} else waitpid(pidunico, NULL, 0);

		//MÁS DE UN COMANDO
		} else {

			//Asignación de memoria dinámica
			pipes = (int **) malloc ((numcomandos-1) * sizeof(int *));
			for (i = 0; i < numcomandos-1; i++){
				pipes[i] = (int *) malloc (2 * sizeof(int));
			}
			pid = (pid_t *) malloc ((numcomandos-1) * sizeof(pid_t));

			//Creación de pipes
			for (i = 0; i < numcomandos-1; i++) pipe(pipes[i]);

			pid[0] = fork();
			if (pid[0] == 0){ //Primer hijo
				close(pipes[0][0]);
				dup2(pipes[0][1], 1);
				for (i = 1; i < numcomandos-1; i++){
					close(pipes[i][0]);
					close(pipes[i][1]);
				}
				if (line->redirect_input != NULL) dup2(input, 0); //Redirección de entrada

				execv(line->commands[0].filename, line->commands[0].argv);
				exit(1);
			} else {
				if (numcomandos > 2){
				for (i = 0; i < numcomandos-2; i++){ //Hijos del medio
					pid[i+1] = fork();
					if (pid[i+1] == 0){
						for (j = 0; j < i; j++){
							close(pipes[j][0]);
							close(pipes[j][1]);
						}
						dup2(pipes[i][0], 0);
						close(pipes[i][1]);
						close(pipes[i+1][0]);
						dup2(pipes[i+1][1], 1);
						for (j = i+2; j <= numcomandos-2; j++){ //Borrar el = si no funciona
							close(pipes[j][0]);
							close(pipes[j][1]);
						}

						execv(line->commands[i+1].filename, line->commands[i+1].argv);
						exit(1);
					}
				}}
				pid[numcomandos-2] = fork();
				if (pid[numcomandos-2] == 0){ //Último hijo
					for (i = 0; i < numcomandos-2; i++){
						close(pipes[i][0]);
						close(pipes[i][1]);
					}
					dup2(pipes[numcomandos-2][0], 0);
					close(pipes[numcomandos-2][1]);

					if (line->redirect_output != NULL) dup2(output, 1); //Redirección de salida
					if (line->redirect_error != NULL) dup2(error, 2); //Redirección de error

					execv(line->commands[numcomandos-1].filename, line->commands[numcomandos-1].argv);
					exit(1);
				} else { //padre
					for (i = 0; i < numcomandos-1; i++){
						close(pipes[i][0]);
						close(pipes[i][1]);
					}
					for (i = 0; i < numcomandos; i++) waitpid(pid[i], NULL, 0); //Espera a todos los hijos
				}
			}
		}
		printf("msh> ");
	}
	for (i = 0; i < numcomandos-1; i++) free(pipes[i]);
	free(pipes);
}

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "parser.h"

int cd(char *dir){
	int err;

	if (dir == NULL) chdir(getenv("HOME"));
	else {
		err = chdir(dir);
		if (err == -1) return 1;
	}

	return 0;
}

int main(void){

    	//Declaración de variables
    	char buf[1024];
   	tline * line;
	int i, j, numcomandos, err;
        pid_t pidunico, input, output, error;
        int **pipes;
        pid_t *pid;

	//utilizamos  sigaction handler para controlar los procesos en background
	struct sigaction action;
	action.sa_handler = SIG_IGN; // definir el handler de SIGCHLD como SIG_IGN elimina el proceso zombie
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	if (sigaction( SIGCHLD, &action, 0) == -1){
	perror(0);
	exit(1);
	}

    	printf("msh> "); //Prompt
    	while (fgets(buf, 1024, stdin)){
        	line = tokenize(buf);

		//Si no se ha escrito nada, muestra el prompt y vuelta al bucle
		if (strcmp(buf,"\n") == 0){
		printf("msh>");
		continue;
		}

		//Asignación de variables
		numcomandos = line->ncommands;
		if (line->redirect_input != NULL){
			input = open(line->redirect_input, O_RDONLY);
			if (input == -1){
				fprintf(stderr, "Error al abrir la redirección de entrada. \n");
				exit(1);
			}
		}
		if (line->redirect_output != NULL){
			output = open(line->redirect_output, O_WRONLY | O_CREAT | O_TRUNC, 0600);
			if (output == -1){
				fprintf(stderr, "Error al abrir la redirección de salida. \n");
				exit(1);
			}
		}
		if (line->redirect_error != NULL){
			error = open(line->redirect_error, O_WRONLY | O_CREAT | O_TRUNC, 0600);
			if (error == -1){
				fprintf(stderr, "Error al abrir la redirección de error. \n");
				exit(1);
			}
		}

		//CD
		if (strcmp(line->commands[0].argv[0],"cd") == 0) err = cd(line->commands[0].argv[1]);
		if (err == 1){
			fprintf(stderr, "El directorio \"%s\" no existe. \n", line->commands[0].argv[1]);
			printf("msh>");
			continue;
		}

		// UN COMANDO
		if (numcomandos == 1){

			pidunico = fork();
			if (pidunico == 0){
				if (line->redirect_input != NULL) dup2(input, 0); //Redirección de entrada
				if (line->redirect_output != NULL) dup2(output, 1); //Redirección de salida
				if (line->redirect_error != NULL) dup2(error, 2); //Redirección de error

				if (line->background == 0){
					sigaction(SIGCHLD, &action,  0);
				}
				execvp(line->commands[0].filename, line->commands[0].argv);
				exit(1);
			} else{
				if(line->background == 0) waitpid(pidunico,NULL,0);
			}
		//MÁS DE UN COMANDO
		} else {

			//Asignación de memoria dinámica
			pipes = (int **) malloc ((numcomandos-1) * sizeof(int *));
			for (i = 0; i < numcomandos-1; i++){
				pipes[i] = (int *) malloc (2 * sizeof(int));
			}
			pid = (pid_t *) malloc ((numcomandos-1) * sizeof(pid_t));

			//Creación de pipes
			for (i = 0; i < numcomandos-1; i++){
				err = pipe(pipes[i]);
				if (err < 0){
					fprintf(stderr, "Error al crear el pipe %s. \n", strerror(errno));
				}
			}

			pid[0] = fork();
			if (pid[0] == 0){ //Primer hijo
				close(pipes[0][0]);
				dup2(pipes[0][1], 1);
				for (i = 1; i < numcomandos-1; i++){
					close(pipes[i][0]);
					close(pipes[i][1]);
				}
				if (line->redirect_input != NULL) dup2(input, 0); //Redirección de entrada

				if (line->background == 0){
	                                sigaction(SIGCHLD, &action,  0);
                                }
				execv(line->commands[0].filename, line->commands[0].argv);
				exit(1);
			} else {
				if (numcomandos > 2){
					for (i = 0; i < numcomandos-2; i++){ //Hijos de en medio
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
							for (j = i+2; j <= numcomandos-2; j++){
								close(pipes[j][0]);
								close(pipes[j][1]);
							}

							if (line->background == 0){
                                        			sigaction(SIGCHLD, &action,  0);
                                			}
							execv(line->commands[i+1].filename, line->commands[i+1].argv);
							exit(1);
						}
					}
				}
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

					if (line->background == 0){
                	                        sigaction(SIGCHLD, &action,  0);
                                        }
					execv(line->commands[numcomandos-1].filename, line->commands[numcomandos-1].argv);
					exit(1);
				} else { //Padre
					for (i = 0; i < numcomandos-1; i++){
						close(pipes[i][0]);
						close(pipes[i][1]);
					}

					if(line->background == 0){
						for (i = 0; i < numcomandos; i++) waitpid(pid[i], NULL, 0); //Espera a todos los hijos
					}
				}
			}
		 }
		printf("msh> ");
	}
	for (i = 0; i < numcomandos-1; i++) free(pipes[i]);
	free(pipes);
}

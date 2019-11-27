#import <signal.h>
#import <stdio.h>
#import <stdlib.h>
#import <sys/types.h>
#import <sys/wait.h>
#import <unistd.h>
#import "parser.h"

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
	int i, p1[2], p2[2];
	pid_t pid;
	int numcomandos = line->ncommands;

	pipe(p1);
	pipe(p2); //Si la salida de la 2 se conecta a la entrada de la 1, todos los hijos (excepto el primero) recibirán información de la tubería 1

	pid = fork();
	if (pid == 0){ //primero
		close(p1[0]);
		close(p1[1]);
		close(p2[0]);
		dup2(p2[1], 1);
		//if (line->redirect_input != NULL) dup2(0, line->redirect_input); //Redirección de entrada

		execvp(line->commands[0].argv[0], line->commands[i+1].argv + 1);
	} else {
		for (i = 0; i < numcomandos-2; i++){ //los hijos del medio
			pid = fork(); //Tú vas a leer de p2[0] y vas a escribir en p1[1];
			if (pid == 0){
				close(p1[0]);
				close(p2[1]);
				dup2(p2[0], 0);
				dup2(p1[1], 1);

				execvp(line->commands[i+1].argv[0], line->commands[i+1].argv + 1);
				dup2(p2[1], p1[1]);
			}
		}
		pid = fork();
		if (pid == 0){ //último: lee de p2[0]
			close(p1[0]);
			close(p1[1]);
			close(p2[1]);
			dup2(p2[0], 0);

			execvp(line->commands[numcomandos-1].argv[0], line->commands[numcomandos-1].argv + 1);

			//if (line->redirect_output != NULL) dup2(1, line->redirect_output); //Redirección de salida
			//if (line->redirect_error != NULL) dup2(2, line->redirect_error); //Redirección de error
		} else { //padre
			close(p1[0]);
			close(p1[1]);
			close(p2[0]);
			close(p2[1]);

			for (i = 0; i < numcomandos; i++) wait(NULL); //Espera a todos los hijos
			exit(0);
		}
	}
}

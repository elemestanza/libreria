#import <signal.h>
#import <stdio.h>
#import <stdlib.h>
#import <sys/types.h>
#import <sys/wait.h>
#import <unistd.h> //execvp

#import "parser.h"

#define PROMPT = "msh> "

pid_t *hijos; //Para almacenar los pids de los hijos

void primero (tcommand command){

}

void medio (tcommand command){

}

void ultimo (tcommand command){

}


int main(){

    	//Declaración de variables
    	char buf[1024];
    	int control = 1;
   	tline * line;

    	//Obtención de línea
    	while (control != 0){
        	printf(PROMPT);
        	scanf("%s", buf);
        	line = tokenize(buf);

        	if (line == NULL) control = 0;
        	else printf("\n");
    	}

	//Segunda declaración de variables
	int i, p1[2], p2[2];
	pid_t pid;
	int numcomandos = line->ncommands;

	hijos = malloc(numcomandos * sizeof(pid_t)); //Crea el espacio para guardar los pids de los hijos
	for (i = 0, i < numcomandos, i++) hijos[i] = 0;

	//¿Cómo sabemos donde hay que hacer la redirección?

	pipe(p1);
	pipe(p2); //Si la salida de la 2 se conecta a la entrada de la 1, todos los hijos (excepto el primero) recibirán información de la tubería 1

	pid = fork();
	if (pid == 0){ //primero
		close(p1
	}
}

#import <signal.h>
#import <stdio.h>
#import <stdlib.h>
#import <sys/types.h>
#import <sys/wait.h>
#import <unistd.h> //execvp

#import "parser.h"

#define PROMPT = "msh> "

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
	int p1[2], p2[2], pid;

	pipe(p1);
	pipe(p2);

	pid = fork();
	if (pid == 0){ //primero
		close(p1
	}
}

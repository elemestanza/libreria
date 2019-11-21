/*
* Dudas:
* qué ocurre en line si tenemos más de una redirección del mismo tipo?
* las redirecciones a partir de aquí se hacen con execlp, hijos y pipes, no?
*
*/



#import <stdio.h>
#import <stdlib.h>

#import "parser.h"

#define PROMPT = "msh> "

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
    
    

}

#include <stdio.h>
#include <stdlib.h>
#include "libreria.h"

int myhead (int argc, char *arg){
	//Función que realizará el HEAD
	if (argc == 0) head(10) ;
	if (argc == 1) head(atoi(arg)) ;
	return 0;
}

int mytail (int argc, char *arg){
	//Función que realizará el TAIL
	return 0;
}

int mylonglines (int numerolineas){
	//Función que realizará el LONGLINES
	return 0;
}

int msgerror(char *arg){
	//Mensaje de error cuando no se tienen suficientes argumentos.
	fprintf(stderr, "USO: %s opción \n OPCIONES \n 1 [n_líneas]: head \n 2 [n_líneas]: tail \n 3 n_líneas: longlines \n", arg);
	return 1;
}

int main(int argc, char *argv[])
{
        //Si no tiene suficientes argumentos o son erróneos, dará error y mostrará el mensaje.
        if (argc == 1) { msgerror(argv[0]);
        } else if (argc >= 4){
                fprintf(stderr, "TOO MANY ARGUMENTS; ");
                msgerror(argv[0]); //En este caso, se tienen demasiados argumentos.
        } else {
                //Elegimos a qué función acceder, a la cual le pasamos los parámetros necesarios.
                switch (atoi(argv[1])) {
                        case 1: myhead(argc-2, argv[2]);
                                break;
                        case 2: mytail(argc-2, argv[2]);
                                break;
                        case 3: if (argc != 3){
                                        fprintf(stderr, "USO: %s 3 n_líneas \n", argv[0]);
                                        return 3; //Faltan argumentos.
                                } else mylonglines(atoi(argv[2]));
                                break;
                        default: msgerror(argv[0]);
                }
        }
        return 0;
}


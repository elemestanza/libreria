#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libreria.h"

int head (int N){

	//Declaración de variables
	char buf[1024];
	int cuenta = 0;

	//Imprime las n primeras líneas
	while ((cuenta < N) && (fgets(buf, 1024, stdin) != NULL)){
		printf("%s", buf);
		cuenta++;
	}

	return 0;
}

int tail (int N){

	//Declaración de variables
	int i;
	char buf[1024];
	char **lineas = (char **) malloc (N * sizeof(char *));

	//Creación de memoria dinámica
	for (i = 0; i < N; i++){
		lineas[i] = (char *) malloc (1024 * sizeof(char));
	}

	//Obtiene cuáles son las n últimas líneas
	while (fgets(buf, 1024, stdin) != NULL){
		for (i = 0; i < N-2; i++) strcpy(lineas[i], lineas[i+1]);
		strcpy(lineas[N-1], fgets(buf, 1024, stdin));
	}

	//Imprime las n últimas líneas
	for (i = 0; i < N; i++) printf("%s", lineas[i]);

	free (lineas);
	return 0;
}

int longlines (int N){
}

#include <stdio.h>
#include <stdlib.h>
#include "libreria.h"

int head (int N){

	//Declaración de variables
	char buf[1024];
	int cuenta = 0;

	//
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

	//
	for (i = 0; i < N; i++){
		lineas[i] = fgets(buf, 1024, stdin);
	}

	//
	while (fgets(buf, 1024, stdin) != NULL)){
		for (i = 0; i < N-2; i++){
			free(lineas[i]);
			lineas[i] = lineas[i+1];
		}
		free(lineas[N-1]);
		lineas[N-1] = fgets(buf, 1024, stdin);
	}

	return 0;
}

int longlines (int N){
}

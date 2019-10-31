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

	//Sale del programa
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

	//Obtiene cuáles son las N últimas líneas
	while (fgets(buf, 1024, stdin) != NULL){
		for (i = 0; i < N-1; i++) strcpy(lineas[i], lineas[i+1]);
		strcpy(lineas[N-1], buf);
	}

	//Imprime las N últimas líneas
	for (i = 0; i < N; i++) printf("%s", lineas[i]);

	//Libera la memoria dinámica y sale del programa
	for (i = 0; i < N; i++) free(lineas[i]);
	free (lineas);
	return 0;
}

int longlines (int N){

	//Decralación de variables
	int i;
	char buf[1024];
	char **lineas = (char **) malloc (N * sizeof(char *));
	char linea[1024];
	char cambio[1024];

	//Creación de memoria dinámica
	for (i = 0; i < N; i++){
		lineas[i] = (char *) malloc (1024 * sizeof(char));
	}

	//Obtiene cuáles son las N líneas más largas
	while (fgets(buf, 1024, stdin) != NULL){
		strcpy(linea, buf);
		for (i = N-1; i >= 0; i--){
			if(strlen(linea) >= strlen(lineas[i])){
				strcpy(cambio, lineas[i]);
				strcpy(lineas[i], linea);
				if (i != N-1) strcpy(lineas[i+1], cambio);
			}
		}
	}

	//Imprime las N líneas más largas
	for (i = 0; i < N; i++) printf("%s", lineas[i]);

	//Libera la memoria dinámica y sale del programa
	for (i = 0; i < N; i++) free(lineas[i]);
	free (lineas);
	return 0;
}

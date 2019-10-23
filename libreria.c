#include <stdio.h>
#include <stdlib.h>

#include "libreria.h"

int head(int N){

	char buf[1024];
	int cuenta = 0;

	while ((cuenta < N) && (fgets(buf, 1024, stdin) != NULL)){
		printf("%s", buf);
		cuenta++;
	}
	return 0;
}

int tail(int N){
}

int longlines(int N){
}

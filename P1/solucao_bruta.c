#include <stdio.h>
#include <stdlib.h>

void imprimeVetor(int* vetor, int tamanho){
	int i;
	printf("Vetor: ");
	for (i=0; i<tamanho; i++) printf("%i ", vetor[i]);
	printf("\n");
}

int** alloc_matrix(int m) {
	int **mat = malloc(sizeof(int*)*m);
	srand(m);
	
	int in = 0;
	// aloca matriz ordenada
	for (int i = 0; i < m; i++) {
		mat[i] = malloc(sizeof(int)*m);
	    	for (int j = 0; j < m; j++) {
			if (i == j) in+= (rand()%3)+1;
			else in += rand()%3;
			mat[i][j] = in;
		}
	}
	// desordena matriz triangular superior
	for (int i = 0; i < m-2; i++) {
	    	for (int j = i+1; j < m-1; j++) {
			int swaps = (int)((m-j)/2.0)+1;
			for (int t = 1; t <= swaps; t++) {
				int a = (rand()%(m-j))+j;
				int b = (rand()%(m-j))+j;
				int tmp = mat[i][a];
				mat[i][a] = mat[i][b];
				mat[i][b] = tmp;
			}
		}

	}
	return mat;
}

int main (int argc, char* argv[]){
	int m;
	scanf("%d", &m);
	
	int vetor[m*m];
	int** matriz = alloc_matrix(m);
	
	int i, j, k = 0;
	for (i=0; i<m; i++)
		for (j=0; j<m; j++)
			vetor[k++] = matriz[i][j];			

	for(i = 0; i < m*m-1; i++){
		for(j = 0; j < m*m-1-i; j++){
			if (vetor[j] > vetor[j+1]){
				int aux = vetor[j];
				vetor[j] = vetor[j+1];
				vetor[j+1] = aux;		
			}
		}
	}

	k = 0;
	for (i=0; i<m; i++){
		for (j=0; j<m; j++){
			matriz[i][j] = vetor[k++];
			printf("%i ", matriz[i][j]);
		}
		printf("\n");
	}

	free(matriz);

	return 0;
}

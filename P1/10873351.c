/*
* Nome: Gabriel Bacarin #10873351
* Prova 1 A-K
* SCC0201 - ICC2
*/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

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

void insertionSort(int *vetor, int tamVetor) {
	int i, key, j;
	for(i = 1; i < tamVetor; i++) {
		key = vetor[i];
		j = i - 1;

		while(j >= 0 && vetor[j] > key) {
			vetor[j + 1] = vetor[j];
			j--;
		}
		vetor[j + 1] = key;
	}
}

void imprimirMatriz(int **mat, int m) {
	for(int i = 0; i < m; i++) {
		for(int j = 0; j < m; j++) {
			printf("%d", mat[i][j]);
			if(j != m - 1 || i != m - 1)
				printf(" ");
		}
		if(i != m - 1)
			printf("\n");		
	}
}

int main() {
	int m;
	scanf("%d", &m);

	int **mat = alloc_matrix(m);

	//para cada linha da matriz
	for(int i = 0; i < m; i++) {
		//existe um vetor que vai da diagonal até o fim da linha
		int tamVetor = (m - i - 1);
		int *vetor = malloc((tamVetor) * sizeof(int));

		//esse vetor vai receber os valores desse pedaço da matriz
		int k = 0;
		for(int j = i + 1; k < tamVetor; j++, k++) {
			vetor[k] = mat[i][j]; 
		}

		//será ordenado
		insertionSort(vetor, k);

		//e então a matriz receberá os valores ordenados desse vetor
		for(int j = i + 1, k = 0; k < tamVetor; j++, k++) {
			mat[i][j] = vetor[k]; 
		}

		//e ele é desalocado
		free(vetor);
	}

	imprimirMatriz(mat, m);
	
	//libera a matriz
	for(int i = 0; i < m; i++) {
		free(mat[i]);
	}
	free(mat);


	return 0;
}
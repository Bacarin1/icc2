/*Nome: Gabriel Bacarin, N 10873351
Revisando vetores e funções

um vetor, que na verdade é uma matriz, guarda os números lidos e quantas vezes eles aparecem
*/

#include<stdio.h>
#include<stdlib.h>

int buscarVetor(int vetor[][2], int posVetor, int buscado) {
	for(int i = 0; i < posVetor; i++) {
		if(vetor[i][0] == buscado) {
			return i;
		}
	}
	return -1;
}

int main() {
	int vetor[100][2];
	char isEOF, numero[10];
	int posVetor = 0;	

	do {
		isEOF = scanf("%s", numero);
		if(isEOF != EOF) {
			int buscado = atoi(numero);
			int index = buscarVetor(vetor, posVetor, buscado);
			
			if(index != -1) { //se o numero ainda não tiver sido adicionado ao vetor
				vetor[index][1] = vetor[index][1] + 1;
			}
			else { //caso já exista ele soma
				vetor[posVetor][0] = buscado;
				vetor[posVetor][1] = 1;
				posVetor++;
			}
		}
	} while(isEOF != EOF);

	for(int i = 0; i < posVetor; i++) {
		printf("%d (%d)", vetor[i][0], vetor[i][1]);
		if(i != posVetor - 1)
			printf("\n");
	}

	return 0;
}
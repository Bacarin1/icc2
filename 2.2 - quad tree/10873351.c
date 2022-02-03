/*
	Nome: Gabriel Bacarin #10873351
	SCC0201 - ICC 2
	Exercício 2 - Compressão de Imagens com Quad-Tree
*/
#include<stdio.h>
#include<stdlib.h>
 
/*função que lê a matriz de entrada e armazena ela na matriz alocada na main*/
void lerEntrada(unsigned char **matrix, int hi) {
	char entrada[4]; //cada número pode ter até 3 dígitos, até 255
	int i = 0; //variáveis de endereço da matriz
	int j = 0;

	while (1) {
		scanf("%s", entrada);
		fgetc(stdin); // consome o espaço após o número

		matrix[i][j] = atoi(entrada); //converte a entrada para número e guarda na matriz
		j++;

		if(j == hi) {
			if(i == hi - 1)
				break; //caso tenha acabado de ler
			i++;
			j = 0;
			fgetc(stdin); //consome o \n de cada linha
		}
	}
}

/*
	função recursiva que vai dividindo a matriz em quatro partes iguais a cada vez
	entradas: matriz, largura da matriz, altura da matriz, arvore, tamanho da árvore, x do corner e y do corner
*/
void dividirMatrix(unsigned char **matrix, int *wdt, int *hi, int *arvore, int *tamArvore, int xC, int yC) {
	// encontrando a altura e largura de cada pedaço da matriz que será analisado
	int newWdt = *wdt / 2;
	int newHi = *hi / 2;
	unsigned char numRamo;

	// ramo superior esquerdo
	numRamo = matrix[yC][xC]; // num ramo recebe o primeiro número desse ramo, e será comparado com os demais números desse ramo

	for(int i = yC; i < yC + newHi; i++) {
		for(int j = xC; j < xC + newWdt; j++) {
			if(matrix[i][j] != numRamo) { // caso algum dos númmeros do ramo seja diferente	//vai ter que quebrar novamente
				*tamArvore = *tamArvore + 1;
				arvore = realloc(arvore, *tamArvore * sizeof(int));
				arvore[*tamArvore - 1] = -1; // adicionando mais uma raíz na arvore

				dividirMatrix(matrix, &newWdt, &newHi, arvore, tamArvore, xC, yC); //chamada recursiva para o ramo sup esquerdo
				i = yC + newHi;
				break;
			}
			if(i == yC + newHi - 1 && j == xC + newWdt - 1) { //percorreu todo o ramo e tudo é igual
				*tamArvore = *tamArvore + 1;
				arvore = realloc(arvore, *tamArvore * sizeof(int));
				arvore[*tamArvore - 1] = numRamo; // adiciona ele na árvore
			}
		}
	}

	// ramo superior direito
	numRamo = matrix[yC][xC + newWdt];

	for(int i = yC; i < yC + newHi; i++) {
		for(int j = xC + newWdt; j < (xC + *wdt); j++) {
			if(matrix[i][j] != numRamo) {
				//vai ter que quebrar novamente
				*tamArvore = *tamArvore + 1;
				arvore = realloc(arvore, *tamArvore * sizeof(int));
				arvore[*tamArvore - 1] = -1;

				dividirMatrix(matrix, &newWdt, &newHi, arvore, tamArvore, xC + newWdt, yC); // o novo corner é o canto sup esq do ramo superior direito
				i = yC + newHi;
				break;
			}
			if(i == yC + newHi - 1 && j == xC + *wdt - 1) { //percorreu todo o ramo e tudo é igual
				*tamArvore = *tamArvore + 1;
				arvore = realloc(arvore, *tamArvore * sizeof(int));
				arvore[*tamArvore - 1] = numRamo;
			}
		}
	}

	// ramo inferior esquerdo
	numRamo = matrix[yC + newHi][xC];

	for(int i = yC + newHi; i < yC + *hi; i++) {
		for(int j = xC; j < xC + newWdt; j++) {
			if(matrix[i][j] != numRamo) {
				//vai ter que quebrar novamente
				*tamArvore = *tamArvore + 1;
				arvore = realloc(arvore, *tamArvore * sizeof(int));
				arvore[*tamArvore - 1] = -1;

				dividirMatrix(matrix, &newWdt, &newHi, arvore, tamArvore, xC, yC + newHi); // o novo corner é o canto sup esquerdo do ramo inferior esquerdo
				i = yC + *hi;
				break;
			}
			if(i == yC + *hi - 1 && j == xC + newWdt - 1) { //percorreu todo o ramo e tudo é igual
				*tamArvore = *tamArvore + 1;
				arvore = realloc(arvore, *tamArvore * sizeof(int));
				arvore[*tamArvore - 1] = numRamo;
			}
		}
	}

	// ramo inferior direito
	numRamo = matrix[yC + newHi][xC + newWdt];

	for(int i = yC + newHi; i < yC + *hi; i++) {
		for(int j = xC + newWdt; j < xC + *wdt; j++) {
			if(matrix[i][j] != numRamo) {
				//vai ter que quebrar novamente
				*tamArvore = *tamArvore + 1;
				arvore = realloc(arvore, *tamArvore * sizeof(int));
				arvore[*tamArvore - 1] = -1;

				dividirMatrix(matrix, &newWdt, &newHi, arvore, tamArvore, xC + newWdt, yC + newHi); // o novo corner é o canto superior esquerdo do ramo inferior direito
				i = yC + *hi;
				break;
			}
			if(i == yC + *hi - 1 && j == xC + *wdt - 1) { //percorreu todo o ramo e tudo é igual
				*tamArvore = *tamArvore + 1;
				arvore = realloc(arvore, *tamArvore * sizeof(int));
				arvore[*tamArvore - 1] = numRamo;
			}
		}
	}
}

int main() {
	int wdt, hi; // dimensões da matriz
	scanf("%d %d  ", &wdt, &hi);

	//criando a matriz usando unsigned char porque é o range pedido pela especificação [0, 255]
	unsigned char **matrix = malloc(hi * sizeof(void *));
	for(int i = 0; i < hi; i++) {
		matrix[i] = malloc(wdt * sizeof(unsigned char));
	}

	lerEntrada(matrix, hi);

	//planta a árvore
	int *arvore = malloc(sizeof(int));
	int tamArvore = 1;
	arvore[0] = -1;

	// essas variáveis são o x do corner e o y do corner, o corner sendo a posição mais à esquerda e acima de cada parte
	// analisada da matriz. a primeira posição que assume é a 0, 0 - o canto superior esquerdo
	int xC = 0, yC = 0;

	dividirMatrix(matrix, &wdt, &hi, arvore, &tamArvore, xC, yC);

	//imprime a árvore
	for(int i = 0; i < tamArvore; i++) {
		printf("%d ", arvore[i]);
	}
	printf("\n");


	//desaloca memória
	for(int i = 0; i < hi; i++) {
		free(matrix[i]);
	}
	free(matrix);
	free(arvore);

	return 0;
}
/*Nome: Gabriel Bacarin N 10873351
Revisando Alocação Dinâmica Recursão e Arquivos*/

#include<stdio.h>
#include<stdlib.h>

char **lerArquivo(FILE *arquivo, int *linhas, int *colunas, int *xIni, int *yIni) {
	//a partir do arquivo lê as informações sobre o labirinto de pessoas e o labirinto em si
	fscanf(arquivo, "%d %d %d %d", linhas, colunas, xIni, yIni);
	fgetc(arquivo);
	//alocando dinamicamente a matriz do labirinto
	char **matriz = realloc(NULL, *linhas * sizeof(void *));
	
	for(int i = 0; i < *linhas; i++) {
		matriz[i] = realloc(NULL, *colunas * sizeof(char));
	}

	char entrada;

	for(int i = 0; i < *linhas; i++) {
		for(int j = 0; j <= *colunas; j++) {
			entrada = fgetc(arquivo);
			if(entrada != '\n') {
				matriz[i][j] = entrada;
			}
		}
	}

	return matriz;
}

void imprimirMatriz(char **matriz, int linhas, int colunas) {
	for(int i = 0; i < linhas; i++) {
		for(int j = 0; j < colunas; j++) {
			printf("%c", matriz[i][j]);
		}
		printf("\n");
	}
}

void contarMatriz(char **matriz, int linhas, int colunas, int *qtd1, char tipo1, int *qtd2, char tipo2) {
	//conta a quantidade de caracteres de determinado tipo presentes na matriz
	//conta dois tipos de caractere por vez
	for(int i = 0; i < linhas; i++) {
		for(int j = 0; j < colunas; j++) {
			if(matriz[i][j] == tipo1)
				*qtd1 = *qtd1 + 1;
			else if(matriz[i][j] == tipo2)
				*qtd2 = *qtd2 + 1;
		}
	}
}

int encontrarSaida(char **matriz, int *linhas, int *colunas, int x, int y) {
	//função recursiva cujo caso base é chegar nas bordas do labirinto
	if((x == 0 || x == (*linhas - 1)) || (y == 0 || y == (*colunas - 1))) {
		return 1;
	}
	else {
		//vai por cima
		if(matriz[x - 1][y] == '.') {
			matriz[x - 1][y] = '*';
			if(encontrarSaida(matriz, linhas, colunas, x - 1, y) == 1)
				return 1;
		}
		//vai pela direita
		if(matriz[x][y + 1] == '.') {
			matriz[x][y + 1] = '*';
			if(encontrarSaida(matriz, linhas, colunas, x, y + 1) == 1)
				return 1;
		}
		//vai por baixo
		if(matriz[x + 1][y] == '.') {
			matriz[x + 1][y] = '*';
			if(encontrarSaida(matriz, linhas, colunas, x + 1, y) == 1)
				return 1;
		}
		//vai pela esquerda
		if(matriz[x][y - 1] == '.') {
			matriz[x][y - 1] = '*';
			if(encontrarSaida(matriz, linhas, colunas, x, y-1) == 1)
				return 1;
		}		
		return 0;
	}
}

int main() {
	char nomeArquivo[26];
	scanf("%s", nomeArquivo);

	FILE *arquivo = fopen(nomeArquivo, "r");

	int linhas = 0, colunas = 0, xIni = 0, yIni = 0;
	char **matriz = lerArquivo(arquivo, &linhas, &colunas, &xIni, &yIni);

	fclose(arquivo);

	int qtdPessoas = 0, qtdPontos = 0;
	contarMatriz(matriz, linhas, colunas, &qtdPessoas, '#', &qtdPontos, '.');

	matriz[xIni][yIni] = '*';
	encontrarSaida(matriz, &linhas, &colunas, xIni, yIni);

	int qtdVisitados = 0;
	qtdPessoas = 0;
	contarMatriz(matriz, linhas, colunas, &qtdVisitados, '*', &qtdPessoas, '#');
	int porcentagemExploracao = (((float)qtdVisitados / (float)qtdPontos) * 100);

	imprimirMatriz(matriz, linhas, colunas);

	printf("\nVoce escapou de todos! Ninguem conseguiu te segurar!\n");
	printf("Veja abaixo os detalhes da sua fuga:\n");
	printf("----Pessoas te procurando: %d\n", qtdPessoas);
	printf("----Numero total de caminhos validos:   %d\n", qtdPontos);
	printf("----Numero total de caminhos visitados: %d\n", qtdVisitados);
	printf("----Exploracao total do labirinto: %d.0%%\n", porcentagemExploracao);

	for(int i = 0; i < linhas; i++) {
		free(matriz[i]);		
	}
	free(matriz);

	return 0;
}
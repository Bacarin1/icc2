/*
*	Nome: Gabriel Bacarin
*	N: 10873351
*	BCC - SSC0201
*	Trabalho 01: Campo Minado
*
*	No trabalho, me refiro as posições adjacentes a cada unidade do campo da seguinte forma
*
*	1	2	3
*	
*	4	X   6
*
*	7	8	9
*
*/

#include<stdio.h>
#include<stdlib.h>

char **lerBoard(char nomeBoard[], int *hi, int *len);
void imprimirBoard(char **board, int hi, int len);
int contarMinas(char **board, int *hi, int *len, int x, int y);
void adicionarDicas(char **board, int hi, int len);
char **criarConcealedBoard(int hi, int len);
void revelarBoard(char **board, char **concealedBoard, int x, int y, int *hi, int *len);
void analizarSelecao(char **board, int hi, int len, int x, int y);


int main() {
	int op;
	char nomeBoard[10];

	scanf("%d ", &op);
	scanf("%s", nomeBoard);

	int hi = 0, len = 0;
	char **board = lerBoard(nomeBoard, &hi, &len);

	if(op == 1) {
		imprimirBoard(board, hi, len);
	}
	if(op == 2) {
		adicionarDicas(board, hi, len);
		imprimirBoard(board, hi, len);
	}
	if(op == 3) {
		int x, y;
		scanf("%d %d", &x, &y);
		adicionarDicas(board, hi, len);
		analizarSelecao(board, hi, len, x, y);
	}

	for(int i = 0; i <= hi; i++) {
		free(board[i]);
	}
	free(board);

	return 0;
}


char **lerBoard(char nomeBoard[], int *hi, int *len) {
	/*funcao que abre o arquivo a ser lido e preenche uma matriz com ele
	recebe: o nome do arquivo, a altura e o comprimento
	retorna: uma matriz preenchida*/
	
	int i = 0, j = 0;
	char **board = malloc(sizeof(void *));
	board[0] = malloc(sizeof(char));

	FILE *arquivoBoard = fopen(nomeBoard, "r");
	if(arquivoBoard == NULL) {
		printf("ERRO ao abrir arquivo\n");
		exit(1);	
	}

	char entrada;

	do {
		entrada = fgetc(arquivoBoard);

		if(entrada == '.' || entrada == '*') {
			board[i] = realloc(board[i], (j + 1)*sizeof(char));
			board[i][j] = entrada;
			j++;
		}
		if(entrada == '\n') {
			i++;
			*len = j;
			j = 0;
			board = realloc(board, (i + 1) * sizeof(void *));
		}

	} while(entrada != EOF);

	//desaloca o espaço a mais que o último \n do arquivo faz alocar
	board = realloc(board, (i) * sizeof(void *));

	*hi = i;

	fclose(arquivoBoard);

	return board;
}

void imprimirBoard(char **board, int hi, int len) {
	for(int i = 0; i < hi; i++) {
		for(int j = 0; j < len; j++) {
			printf("%c", board[i][j]);
		}
		printf("\n");
	}
}

int contarMinas(char **board, int *hi, int *len, int x, int y) {
	//conta minas adjacentes a uma posição
	//retorna a quantidade acumulada de minas

	int minas = 0;
	//x é linha, y é coluna
	//verificar se posição 1 é válida
	if((y - 1 != -1) && (x - 1 != -1)) {
		if(board[x - 1][y - 1] == '*')
			minas++;
	}
	//verificar se posição 2 é válida
	if(x - 1 != -1) {
		if(board[x - 1][y] == '*')
			minas++;
	}
	//verificar se posição 3 é válida
	if((y + 1 != *len) && (x - 1 != -1)) {
		if(board[x - 1][y + 1] == '*')
			minas++;
	}
	//verificar se posição 4 é válida
	if(y - 1 != -1) {
		if(board[x][y - 1] == '*')
			minas++;
	}
	//verificar se posição 6 é válida
	if(y + 1 != *len) {
		if(board[x][y + 1] == '*')
			minas++;
	}
	//verificar se posição 7 é válida
	if((y - 1 != -1) && (x + 1 != *hi)) {
		if(board[x + 1][y - 1] == '*')
			minas++;
	}
	//verificar se posição 8 é válida
	if(x + 1 != *hi) {
		if(board[x + 1][y] == '*')
			minas++;
	}
	//verificar se posição 9 é válida
	if((x + 1 != *hi) && (y + 1 != *len)) {
		if(board[x + 1][y + 1] == '*')
			minas++;
	}

	return minas;
}

void adicionarDicas(char **board, int hi, int len) {
	//preenche o tabuleiro com as dicas
	int minas = 0;
	for(int x = 0; x < hi; x++) {
		for(int y = 0; y < len; y++) {
			if(board[x][y] == '.') {
				minas = contarMinas(board, &hi, &len, x, y);
				if(minas != 0) {
					board[x][y] = minas + '0'; // "itoa"
				}				
			}
		}
	}
}

char **criarConcealedBoard(int hi, int len) {
	//cria um tabuleiro com altura e largura especificadas
	//e preenche ele inteiro com caracteres X
	char **concealedBoard = malloc(hi * sizeof(void *));
	for(int i = 0; i < hi; i++) {
		concealedBoard[i] = malloc(len * sizeof(char));
		for(int j = 0; j < len; j++) {
			concealedBoard[i][j] = 'X';
		}
	}
	return concealedBoard;
}

void revelarBoard(char **board, char **concealedBoard, int x, int y, int *hi, int *len) {
	//função recursiva que verifica se a posição adjacente é válida, e caso seja,
	//preenche ela ou não a depender do que ela contem
	//ao final, o concealed board acaba preenchido com algumas posições reveladas, conforme indicado pelo usuário
	// caso base - o x, y atual é uma dica, logo, uma borda
	if(board[x][y] != '*' && board[x][y] != '.') {
		concealedBoard[x][y] = board[x][y];
		return;
	}
	else {
		//ele vai expandir até encontrar uma dica
		concealedBoard[x][y] = '.';
		//verificar se posição 1 é válida
		if((y - 1 != -1) && (x - 1 != -1)) {
			//procurar na posição 1
			if(concealedBoard[x-1][y-1] == 'X') {
				revelarBoard(board, concealedBoard, (x-1), (y-1), hi, len);
			}
		}
		//verificar se posição 2 é válida
		if(x - 1 != -1) {
			//procurar na posição 2
			if(concealedBoard[x-1][y] == 'X') {
				revelarBoard(board, concealedBoard, (x-1), y, hi, len);
			}	
		}
		//verificar se posição 3 é válida
		if((y + 1 != *len) && (x - 1 != -1)) {
			//procurar na posição 3
			if(concealedBoard[x-1][y+1] == 'X') {
				revelarBoard(board, concealedBoard, (x-1), (y+1), hi, len);
			}
		}
		//verificar se posição 4 é válida
		if(y - 1 != -1) {
			//procurar na posição 4
			if(concealedBoard[x][y-1] == 'X') {
				revelarBoard(board, concealedBoard, (x), (y-1), hi, len);
			}
		}
		//verificar se posição 6 é válida
		if(y + 1 != *len) {
			//procurar na posição 6
			if(concealedBoard[x][y+1] == 'X') {
				revelarBoard(board, concealedBoard, (x), (y+1), hi, len);
			}
		}
		//verificar se posição 7 é válida
		if((y - 1 != -1) && (x + 1 != *hi)) {
			//procurar na posição 7
			if(concealedBoard[x+1][y-1] == 'X') {
				revelarBoard(board, concealedBoard, (x+1), (y-1), hi, len);
			}
		}
		//verificar se posição 8 é válida
		if(x + 1 != *hi) {
			//procurar na posição 8
			if(concealedBoard[x+1][y] == 'X') {
				revelarBoard(board, concealedBoard, (x+1), (y), hi, len);
			}
		}
		//verificar se posição 9 é válida
		if((x + 1 != *hi) && (y + 1 != *len)) {
			//procurar na posição 9
			if(concealedBoard[x+1][y+1] == 'X') {
				revelarBoard(board, concealedBoard, (x+1), (y+1), hi, len);
			}
		}		
	}
	return;
}

void analizarSelecao(char **board, int hi, int len, int x, int y) {
	//a partir do x e y informados pelo usuário, decide o que fazer para mostrar o tabuleiro resultante da ação
	
	//cria um tabuleiro com apenas caracteres X para usar para a seleção
	char **concealedBoard = criarConcealedBoard(hi, len);
	
	//se uma mina for selecionada
	if(board[x][y] == '*') {
		imprimirBoard(board, hi, len);
	}
	//se uma dica for selecionada
	if(board[x][y] != '*' && board[x][y] != '.') {
		concealedBoard[x][y] = board[x][y];
		imprimirBoard(concealedBoard, hi, len);
	}
	//se um espaço vazio é selecionado
	if(board[x][y] == '.') {
		revelarBoard(board, concealedBoard, x, y, &hi, &len);
		imprimirBoard(concealedBoard, hi, len);
	}

	//desaloca o concealed board
	for(int i = 0; i < hi; i++) {
		free(concealedBoard[i]);
	}
	free(concealedBoard);

}
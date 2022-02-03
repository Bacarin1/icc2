/*
    Nome: Gabriel Bacarin #10873351
    SCC0201 - ICC2
    Trabalho 2 - Compressão de Áudio
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<complex.h>
#include<math.h>

/*
    essa estrutura audiot é usada para guardar todas as informações relevantes de cada sample: seu índice, aka a ordem original de cada um no
    arquivo, o coeficiente complexo obtido através da DFT e a magnitude obtida nessa mesma função
*/
typedef struct transformedAudio {
    int index;
    double complex coef;
    double mag;
} audiot;

/*
    função do Ku para ler os samples do arquivo original, modificada para guardar também a quantidade de samples lida
    entradas: nome do arquivo original, quantidade de samples
    saída: vetor com samples
*/
unsigned char* read_wav_data(char* fname, int *length) {
    FILE* fp = fopen(fname, "rb");
    unsigned char buf4[4];

    fseek(fp, 40, SEEK_SET);
    fread(buf4, sizeof(buf4), 1, fp);
    int dataSize = buf4[0] | buf4[1]<<8 | buf4[2]<<16 | buf4[3]<<24;

    *length = dataSize;
    unsigned char* data = malloc(sizeof(*data) * (dataSize));
    
    int i = 0;
    while (i < dataSize) {
        fread(&data[i++], sizeof(unsigned char), 1, fp);
    }
    fclose(fp);
    return data;
}

/*
    função que escreve um novo arquivo new_fname, binário, que contém o header do arquivo original e os samples modificados pelo programa
    entradas: nome do arquivo, vetor de samples e tamanho do vetor
*/
void writeWav(char* fname, unsigned char *audio, int length) {
    FILE* fp = fopen(fname, "rb");
    unsigned char header[44];

    fread(header, sizeof(unsigned char), 44, fp);
    fclose(fp);

    char nfname[26];
    strcpy(nfname, "new_");
    strcat(nfname, fname);

    fp = fopen(nfname, "wb");
    fwrite(header, sizeof(header), 1, fp);

    fwrite(audio, sizeof(unsigned char), length, fp);

    fclose(fp);
}

/*
    Função do Ku modificada para armazenar os coeficientes obtidos na transformada discreta de fourier em uma estrutura audiot
    além de calcular os coeficientes, já calcula também as magnitudes, além de contar quantos coeficientes são negativos, o que é usado
    na saída do programa
    entradas: vetor de audio, tamanho do vetor e quantidade de negativos
    saída: vetor vec de estruturas audiot contendo seus coeficientes, magnitudes e índices
*/
audiot *DFT(unsigned char *audio, int length, int *neg) {
    //double complex *coef = (double complex *) calloc(length, sizeof(double complex));
    audiot *vec = calloc(length, sizeof(audiot));

    for (int k = 0; k < length; k++) {
        vec[k].index = k;
        vec[k].coef = 0;
        for (int n = 0; n < length; n++) {
            vec[k].coef += audio[n] * cexp((-2.0 * M_PI * (((k+1) * n * 1.0) / (length * 1.0))) * _Complex_I);
        }
        // printf("%.1lf + %.1lfi\n", creal(coef[k]), cimag(coef[k]));
        vec[k].mag = sqrt(pow(creal(vec[k].coef), 2) + pow(cimag(vec[k].coef), 2));
        
        if(creal(vec[k].coef) <= 0 && cimag(vec[k].coef) <= 0) {
            *neg = *neg + 1;
        }
    }
    return vec;
}

/*Inverse Discrete Fourier Transform
a partir do vetor que contém os coeficientes complexos, vai formar novamente um vetor de audio com unsigned chars,
que será escrito no arquivo. Faz isso através da inversa da transformada discreta de fourier
entradas: vetor da estrutura audiot, quantidade de samples length
saída: vetor audio de unsigned char*/
unsigned char *IDFT(audiot *vecCoef, int length) {

    unsigned char *audio = calloc(length, sizeof(unsigned char));
    //printf("length %d\n", length);

    for (int n = 0; n < length; n++) {
        double complex temp = 0;
        for (int k = 0; k < length; k++) {            //n e k invertidos
            temp = temp + vecCoef[k].coef * cexp((2.0 * M_PI * (((n+1) * k * 1.0) / (length * 1.0))) * _Complex_I);
        }       

        temp = temp / length;

       // printf("%.1lf + %.1lfi ", creal(temp), cimag(temp));

        audio[n] = (unsigned char)creal(temp);
        //printf("%d ", audio[n]);
    }

    return audio;
}

/*insertion sort que usa como parâmetro as magnitudes de cada sample, ordenando da maior para a menor
entradas: vetor a ser ordenado e quantidade de samples*/
void sortMag(audiot *vecCoef, int length) {
    int i, j;
    audiot key;

    for(i = 1; i < length; i++) {
        key = vecCoef[i];
        j = i - 1;

        while(j >= 0 && vecCoef[j].mag < key.mag) {
            vecCoef[j + 1] = vecCoef[j];
            j--;
        }
        vecCoef[j + 1] = key;
    }
}

/*função que zera os coeficientes e magnitudes das posições maiores que as t primeiras
entrada: vetor de estruturas, tamanho dele e t, que representa os primeiros elementos*/
void zeroOut(audiot *vecCoef, int length, int t) {
    for(int i = (t); i < length; i++) {
        vecCoef[i].coef = 0;
        vecCoef[i].mag = 0;
    }

    for(int i = 0; i < t; i++) {
        printf("%d\n", (int)vecCoef[i].mag); //debug mode
    }
}

/*é um insertion sort que usa como parâmetro o campo index da estrutura audiot, ordenando o vetor de volta as posições originais
entradas: vetor a ser ordenado, tamanho do vetor (quantidade de samples)*/
void sortIndex(audiot *vecCoef, int length) {
    int i, j;
    audiot key;

    for(i = 1; i < length; i++) {
        key = vecCoef[i];
        j = i - 1;

        while(j >= 0 && vecCoef[j].index > key.index) {
            vecCoef[j + 1] = vecCoef[j];
            j--;
        }
        vecCoef[j + 1] = key;
    }
}



int main() {
    char fname[20];
    int t;
    // lê as entradas, nome do arquivo e quantidade de coeficientes que serão usados
    scanf("%s", fname);
    scanf("%d", &t);

    int length = 0; // variável que até o fim do programa armazena quantos samples foram lidos
    unsigned char *audio = read_wav_data(fname, &length);
    printf("%d\n", length); //imprimindo a quantidade de samples

    int neg = 0; // variável que armazena a quantidade de coeficientes cujas partes reais e imaginárias são negativas
    audiot *vecCoef = DFT(audio, length, &neg);
    free(audio);
    printf("%d\n", neg);

    sortMag(vecCoef, length); //ordena por magnitude
    zeroOut(vecCoef, length,t); //zera os coeficientes maiores que t
    sortIndex(vecCoef, length); //retorna a ordem original ordenando pelo índice

    audio = IDFT(vecCoef, length); //aplica a transformada inversa

    writeWav(fname, audio, length); //escrevee o novo arquivo

    //desaloca a memória usada
    free(audio);
    free(vecCoef);    

    return 0;
}
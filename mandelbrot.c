#include <stdio.h>
#include <stdlib.h>
#include <errno.h> 
#include <limits.h>

typedef struct mandelbrot{
    int largura;
    int altura;
    int max_interacoes;
    int numeroDeThreads;
} Mandelbrot;

int main(int argc, char *argv[]){

    if (argc != 5){
        fprintf(stderr, "Erro: entrada ou argumento(s) errado.\n");
        exit(1);
    }

    Mandelbrot dados;

    char *fim;
    long valor;

    errno = 0;

    //long int strtol(const char *nptr, char **endptr, int base);
    //const char *nptr ponteiro para string p o número convertido. ignora espaços em branco e lê digitos até encontrar um caractere não numérico
    //char **endptr contem um ponteiro para ponteiro de caractere, se não for NULL a função guarda o endereço do primeiro caractere não interpretado como parte do número. Isso permite analisar strings com múltiplos valores e detectar erros de conversão
    //int base é a base numérica para a conversão
    //a função retorna o valor inteiro convertido, se estourar retorna LONG_MAX ou LONG_MIN, e define a variavel como errno para ERANGE.
    //se nenhuma conversão foi feita retorna 0 e define a variável global errno para EINVAL

    valor = strtol(argv[1], &fim, 10); //strtol converte uma string pra long

    if (fim == argv[1]){ //fim == argv[1] não conseguiu ler nem o primeiro caractere
        fprintf(stderr, "Largura invalida.\n");
        exit(1);
        //não conseguiu converter
    }

    if (*fim != '\0'){
        fprintf(stderr, "Largura invalida.\n");
        exit(1);
        //sobrou lixo na memória
    }

    if (errno == ERANGE){
        fprintf(stderr, "Largura fora do limite permitido.\n");
        exit(1);
        //estourou passou do limite
    }

    if (valor <= 0 || valor > INT_MAX){
        fprintf(stderr, "Largura inválida.\n");
        exit(1);
        //os parametros nao podem ser negativos nem passar do maximo
    }

    dados.largura = (int) valor;

    errno = 0;
    valor = strtol(argv[2], &fim, 10);

    if (fim == argv[2]){ //fim == argv[2] não conseguiu ler nem o primeiro caractere
        fprintf(stderr, "Altura invalida.\n");
        exit(1);
        //não conseguiu converter
    }

    if (*fim != '\0'){
        fprintf(stderr, "Altura invalida.\n");
        exit(1);
        //sobraram caracteres que nao foram convertidos na leitura
    }

    if (errno == ERANGE){
        fprintf(stderr, "Altura fora do limite permitido.\n");
        exit(1);
        //estourou passou do limite
    }

    if (valor <= 0 || valor > INT_MAX){
        fprintf(stderr, "Altura inválida.\n");
        exit(1);
        //atingiu o máximo
    }

    dados.altura = (int) valor;

    errno = 0;
    valor = strtol(argv[3], &fim, 10);

    if (fim == argv[3]){ //fim == argv[3] não conseguiu ler nem o primeiro caractere
        fprintf(stderr, "Altura invalida.\n");
        exit(1);
        //não conseguiu converter
    }

    if (*fim != '\0'){
        fprintf(stderr, "Altura invalida.\n");
        exit(1);
        //sobraram caracteres que nao foram convertidos na leitura
    }

    if (errno == ERANGE){
        fprintf(stderr, "Altura fora do limite permitido.\n");
        exit(1);
        //estourou passou do limite
    }

    if (valor <= 0 || valor > INT_MAX){
        fprintf(stderr, "ALtura inválida.\n");
        exit(1);
        //atingiu o máximo
    }

    dados.max_interacoes = (int) valor;

    errno = 0;

    strtol(argv[4], &fim, 10);

    if (fim == argv[4]){ //fim == argv[4] não conseguiu ler nem o primeiro caractere
        fprintf(stderr, "Altura invalida.\n");
        exit(1);
        //não conseguiu converter
    }

    if (*fim != '\0'){
        fprintf(stderr, "Altura invalida.\n");
        exit(1);
        //sobraram caracteres que nao foram convertidos na leitura
    }

    if (errno == ERANGE){
        fprintf(stderr, "Altura fora do limite permitido.\n");
        exit(1);
        //estourou passou do limite
    }

    if (valor <= 0 || valor > INT_MAX){
        fprintf(stderr, "ALtura inválida.\n");
        exit(1);
        //atingiu o máximo
    }

    dados.numeroDeThreads = (int) valor;

    return 0;
}
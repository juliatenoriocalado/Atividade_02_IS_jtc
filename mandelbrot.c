#include <stdio.h>
#include <stdlib.h>
#include <errno.h> 
#include <limits.h>
#include <time.h>
#include <stdint.h>
#include <omp.h>
#include <pthread.h>

typedef struct mandelbrot{
    int largura;
    int altura;
    int max_interacoes;
    int numeroDeThreads;
} Mandelbrot;

typedef struct DadosThread {
    int *imagem;
    Mandelbrot *dados;
    int inicio_linha;
    int fim_linha;
    int id_thread;
} DadosThread;

void *funcao_thread1(void *arg){

    DadosThread *dt = (DadosThread *) arg;
    
    for (int linha = dt->inicio_linha; linha < dt->fim_linha; linha++){
        for (int coluna=0; coluna<dt->dados->largura; coluna++){
            
            int indice = linha * dt->dados->largura + coluna;

            double c_real = -2.0 + ((double) coluna/ (dt->dados->largura - 1)) * 3.0;
            double c_imagem = -1.5 + ((double) linha / (dt->dados->altura - 1)) * 3.0;
            double z_real = 0.0;
            double z_imagem = 0.0;
            int iteracoes = 0;
            
            while (iteracoes < dt->dados->max_interacoes){
                double novo_real = (z_real * z_real) - (z_imagem * z_imagem) + c_real;
                double novo_imagem = (2 * z_real * z_imagem) + c_imagem;

                z_real = novo_real;
                z_imagem = novo_imagem;

                iteracoes++;

                if ((z_real * z_real) + (z_imagem * z_imagem) > 4.0){ //se explodiu
                    break;
                }
            }

            int intensidade = (int)(((double) iteracoes / dt->dados->max_interacoes) * 255.0);
            dt->imagem[indice] = intensidade;

        }    
    }
    return NULL;
}

void *funcao_thread2(void *arg){
    DadosThread *dt = (DadosThread *) arg;
    for (int linha = dt->id_thread; linha < dt->dados->altura; linha += dt->dados->numeroDeThreads){
        for (int coluna=0; coluna<dt->dados->largura; coluna++){
            int indice = linha * dt->dados->largura + coluna;
            double c_real = -2.0 + ((double) coluna/ (dt->dados->largura - 1)) * 3.0;
            double c_imagem = -1.5 + ((double) linha / (dt->dados->altura - 1)) * 3.0;
            double z_real = 0.0;
            double z_imagem = 0.0;
            int iteracoes = 0;
            
            while (iteracoes < dt->dados->max_interacoes){
                double novo_real = (z_real * z_real) - (z_imagem * z_imagem) + c_real;
                double novo_imagem = (2 * z_real * z_imagem) + c_imagem;

                z_real = novo_real;
                z_imagem = novo_imagem;

                iteracoes++;

                if ((z_real * z_real) + (z_imagem * z_imagem) > 4.0){ //se explodiu
                    break;
                }
            }

            int intensidade = (int)(((double) iteracoes / dt->dados->max_interacoes) * 255.0);
            dt->imagem[indice] = intensidade;
        }    
    }
    return NULL;
}

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

    if (valor <= 1 || valor > INT_MAX){
        fprintf(stderr, "Largura invalida.\n");
        exit(1);
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

    if (valor <= 1 || valor > INT_MAX){
        fprintf(stderr, "Altura inválida.\n");
        exit(1);
    }

    dados.altura = (int) valor;

    errno = 0;
    valor = strtol(argv[3], &fim, 10);

    if (fim == argv[3]){ //fim == argv[3] não conseguiu ler nem o primeiro caractere
        fprintf(stderr, "Numero maximo de iteracoes invalido.\n");
        exit(1);
        //não conseguiu converter
    }

    if (*fim != '\0'){
        fprintf(stderr, "maximo de iteracoes invalido.\n");
        exit(1);
        //sobraram caracteres que nao foram convertidos na leitura
    }

    if (errno == ERANGE){
        fprintf(stderr, "maximo de iteracoes invalido.\n");
        exit(1);
        //estourou passou do limite
    }

    if (valor <= 0 || valor > INT_MAX){
        fprintf(stderr, "maximo de iteracoes.\n");
        exit(1);
        //atingiu o máximo
    }

    dados.max_interacoes = (int) valor;

    errno = 0;

    valor = strtol(argv[4], &fim, 10);

    if (fim == argv[4]){ //fim == argv[4] não conseguiu ler nem o primeiro caractere
        fprintf(stderr, "Numero de threads invalido.\n");
        exit(1);
        //não conseguiu converter
    }

    if (*fim != '\0'){
        fprintf(stderr, "Numero de threads invalido.\n");
        exit(1);
    }

    if (errno == ERANGE){
        fprintf(stderr, "Numero de threads invalido.\n");
        exit(1);
        //estourou passou do limite
    }

    if (valor <= 0 || valor > INT_MAX){
        fprintf(stderr, "Numero de threads invalido.\n");
        exit(1);
    }

    dados.numeroDeThreads = (int) valor;

    if ((size_t) dados.largura > SIZE_MAX / (size_t) dados.altura){
        fprintf(stderr, "A imagem ficou muito grande.\n");
        exit(1);
    }

    size_t quantidade_pixels = (size_t) dados.largura * dados.altura;

    if (quantidade_pixels > SIZE_MAX / sizeof(int)){
        fprintf(stderr, "Imagem grande demais para alocar o espaco.\n");
        exit(1);
    }

    int *imagem = malloc(quantidade_pixels * sizeof(int));

    if (imagem == NULL){
        fprintf(stderr, "erro ao alocar memoria.\n");
        exit(1);
    }

    struct timespec inicio_serial;
    struct timespec fim_serial;
    struct timespec inicio_openmp;
    struct timespec fim_openmp;

    clock_gettime(CLOCK_MONOTONIC, &inicio_serial);

    for (int linha=0; linha<dados.altura; linha++){
        for (int coluna=0; coluna<dados.largura; coluna++){
            
            int indice = linha * dados.largura + coluna;

            double c_real = -2.0 + ((double) coluna/ (dados.largura - 1)) * 3.0;
            double c_imagem = -1.5 + ((double) linha / (dados.altura - 1)) * 3.0;
            double z_real = 0.0;
            double z_imagem = 0.0;
            int iteracoes = 0;
            
            while (iteracoes < dados.max_interacoes){
                double novo_real = (z_real * z_real) - (z_imagem * z_imagem) + c_real;
                double novo_imagem = (2 * z_real * z_imagem) + c_imagem;

                z_real = novo_real;
                z_imagem = novo_imagem;

                iteracoes++;

                if ((z_real * z_real) + (z_imagem * z_imagem) > 4.0){ //se explodiu
                    break;
                }
            }

            int intensidade = (int)(((double) iteracoes / dados.max_interacoes) * 255.0);
            imagem[indice] = intensidade;

        }    
    }

    clock_gettime(CLOCK_MONOTONIC, &fim_serial);

    double tempo_serial =
        (fim_serial.tv_sec - inicio_serial.tv_sec) +
        (fim_serial.tv_nsec - inicio_serial.tv_nsec) / 1000000000.0;

    FILE *arquivo;
    arquivo = fopen("mandelbrot_jtc_serial.pgm", "w");

    if (arquivo == NULL){
        fprintf(stderr, "Erro ao abrir ou criar arquivo serial.\n");
        free(imagem);
        exit(1);
    }

    for (int linha = 0; linha < dados.altura; linha++){
        for (int coluna = 0; coluna < dados.largura; coluna++){
            int indice = linha * dados.largura + coluna;
            fprintf(arquivo, "%d ", imagem[indice]);
        }
        fprintf(arquivo, "\n");
    }

    fclose(arquivo);

    clock_gettime(CLOCK_MONOTONIC, &inicio_openmp);
    #pragma omp parallel for num_threads(dados.numeroDeThreads)

    for (int linha=0; linha<dados.altura; linha++){
        for (int coluna=0; coluna<dados.largura; coluna++){
            
            int indice = linha * dados.largura + coluna;

            double c_real = -2.0 + ((double) coluna/ (dados.largura - 1)) * 3.0;
            double c_imagem = -1.5 + ((double) linha / (dados.altura - 1)) * 3.0;
            double z_real = 0.0;
            double z_imagem = 0.0;
            int iteracoes = 0;
            
            while (iteracoes < dados.max_interacoes){
                double novo_real = (z_real * z_real) - (z_imagem * z_imagem) + c_real;
                double novo_imagem = (2 * z_real * z_imagem) + c_imagem;

                z_real = novo_real;
                z_imagem = novo_imagem;

                iteracoes++;

                if ((z_real * z_real) + (z_imagem * z_imagem) > 4.0){ //se explodiu
                    break;
                }
            }

            int intensidade = (int)(((double) iteracoes / dados.max_interacoes) * 255.0);
            imagem[indice] = intensidade;

        }    
    }

    clock_gettime(CLOCK_MONOTONIC, &fim_openmp);

    double tempo_openmp = (fim_openmp.tv_sec - inicio_openmp.tv_sec) + (fim_openmp.tv_nsec - inicio_openmp.tv_nsec) / 1000000000.0;
 
    arquivo = fopen("mandelbrot_jtc_openmp.pgm", "w");

    if (arquivo == NULL){
        fprintf(stderr, "Erro ao abrir ou criar arquivo PGM.\n");
        free(imagem);
        exit(1);
    }

    for (int linha = 0; linha < dados.altura; linha++){
        for (int coluna = 0; coluna < dados.largura; coluna++){
            int indice = linha * dados.largura + coluna;
            fprintf(arquivo, "%d ", imagem[indice]);
        }
        fprintf(arquivo, "\n");
    }

    fclose(arquivo);

    pthread_t *threads = malloc(dados.numeroDeThreads * sizeof(pthread_t));
    DadosThread *args = malloc(dados.numeroDeThreads * sizeof(DadosThread));

    if (threads == NULL || args == NULL){
        fprintf(stderr, "Erro ao alocar memoria para as threads.\n");
        free(threads);
        free(args);
        free(imagem);
        exit(1);
    }

    struct timespec inicio_thread;
    struct timespec fim_thread;

    clock_gettime(CLOCK_MONOTONIC, &inicio_thread);

    int threads_criadas = 0;

    for (int i=0; i<dados.numeroDeThreads; i++){

        args[i].imagem = imagem;
        args[i].dados = &dados;

        args[i].inicio_linha = i * dados.altura / dados.numeroDeThreads;

        args[i].fim_linha = (i + 1) * dados.altura / dados.numeroDeThreads;

        int resultado = pthread_create(&threads[i], NULL, funcao_thread1, &args[i]);

        if (resultado != 0){
            fprintf(stderr, "Erro ao criar thread.\n");
            break;
        }

        threads_criadas++;
            
    }

    for (int i = 0; i < threads_criadas; i++){
        pthread_join(threads[i], NULL);
    }

    if (threads_criadas != dados.numeroDeThreads)
    {free(imagem);
    free(threads);
    free(args);
    exit(1);
    }

    clock_gettime(CLOCK_MONOTONIC, &fim_thread);

    double tempo_pthreads1 =
        (fim_thread.tv_sec - inicio_thread.tv_sec) +
        (fim_thread.tv_nsec - inicio_thread.tv_nsec) / 1000000000.0;

    arquivo = fopen("mandelbrot_jtc_pthreads1.pgm", "w");

    if (arquivo == NULL){
        fprintf(stderr, "Erro ao abrir ou criar arquivo Pthreads1.\n");
        free(imagem);
        free(threads);
        free(args);
        exit(1);
    }

    for (int linha = 0; linha < dados.altura; linha++){
        for (int coluna = 0; coluna < dados.largura; coluna++){
            int indice = linha * dados.largura + coluna;
            fprintf(arquivo, "%d ", imagem[indice]);
        }

        fprintf(arquivo, "\n");
    }

    fclose(arquivo);

    //aqui dm baio

    struct timespec inicio_thread2;
    struct timespec fim_thread2;

    clock_gettime(CLOCK_MONOTONIC, &inicio_thread2);

    threads_criadas = 0;

    for (int i=0; i<dados.numeroDeThreads; i++){

        args[i].imagem = imagem;
        args[i].dados = &dados;
        args[i].id_thread = i;

        int resultado = pthread_create(&threads[i], NULL, funcao_thread2, &args[i]);

        if (resultado != 0){
            fprintf(stderr, "Erro ao criar thread2.\n");
            break;
        }

        threads_criadas++;
            
    }

    for (int i = 0; i < threads_criadas; i++){
        pthread_join(threads[i], NULL);
    }

    if (threads_criadas != dados.numeroDeThreads)
    {free(imagem);
    free(threads);
    free(args);
    exit(1);
    }

    clock_gettime(CLOCK_MONOTONIC, &fim_thread2);

    double tempo_pthreads2 =
        (fim_thread2.tv_sec - inicio_thread2.tv_sec) +
        (fim_thread2.tv_nsec - inicio_thread2.tv_nsec) / 1000000000.0;

    arquivo = fopen("mandelbrot_jtc_pthreads2.pgm", "w");

        if (arquivo == NULL){
        fprintf(stderr, "Erro ao abrir ou criar arquivo Pthreads2.\n");
        free(imagem);
        free(threads);
        free(args);
        exit(1);
    }

    for (int linha = 0; linha < dados.altura; linha++){
        for (int coluna = 0; coluna < dados.largura; coluna++){
            int indice = linha * dados.largura + coluna;
            fprintf(arquivo, "%d ", imagem[indice]);
        }

        fprintf(arquivo, "\n");
    }

    fclose(arquivo);

    // antes daqui
    
    FILE *arquivo_tempos;
    arquivo_tempos = fopen("times.txt", "w");

    if (arquivo_tempos == NULL){
        fprintf(stderr, "Erro ao abrir ou criar times.txt.\n");
        free(imagem);
        free(threads);
        free(args);
        exit(1);
    }

    fprintf(arquivo_tempos, "Serial: %f\n", tempo_serial);
    fprintf(arquivo_tempos, "OpenMP: %f\n", tempo_openmp);
    fprintf(arquivo_tempos, "Pthreads1: %f\n", tempo_pthreads1);
    fprintf(arquivo_tempos, "Pthreads2: %f\n", tempo_pthreads2);
    fclose(arquivo_tempos);
    free(imagem);
    free(threads);
    free(args);

    return 0;
} 
# Mandelbrot - Implementação 2

Implementação do conjunto de Mandelbrot em C utilizando:

- Execução serial
- OpenMP
- Pthreads com divisão por blocos de linhas
- Pthreads com divisão intercalada de linhas

## Compilação

O projeto deve ser compilado em ambiente Linux, Unix ou macOS.

Para limpar arquivos compilados anteriormente:

```bash
make clean
```

Para compilar o programa:

```bash
make
```

Após a compilação, será gerado o executável:

```text
mandelbrot
```

## Execução

O programa recebe quatro argumentos:

```bash
./mandelbrot [largura] [altura] [max_iteracoes] [num_threads]
```

Exemplo:

```bash
./mandelbrot 800 600 1000 4
```

Os argumentos correspondem a:

- `largura`: largura da imagem
- `altura`: altura da imagem
- `max_iteracoes`: número máximo de iterações por ponto
- `num_threads`: número de threads utilizado nas implementações paralelas

## Arquivos gerados

Após uma execução válida, o programa gera:

```text
mandelbrot_jtc_serial.pgm
mandelbrot_jtc_openmp.pgm
mandelbrot_jtc_pthreads1.pgm
mandelbrot_jtc_pthreads2.pgm
times.txt
```

Os quatro arquivos `.pgm` contêm os valores de intensidade dos pixels calculados por cada implementação.

O arquivo `times.txt` registra os tempos das quatro abordagens.

## Verificação dos resultados

Para verificar os arquivos gerados:

```bash
ls -l mandelbrot_jtc_*.pgm times.txt
```

As quatro implementações devem produzir arquivos idênticos.

Para comparar os resultados:

```bash
cmp mandelbrot_jtc_serial.pgm mandelbrot_jtc_openmp.pgm
cmp mandelbrot_jtc_serial.pgm mandelbrot_jtc_pthreads1.pgm
cmp mandelbrot_jtc_serial.pgm mandelbrot_jtc_pthreads2.pgm
```

Caso os arquivos sejam idênticos, os comandos `cmp` não apresentam nenhuma saída.

## Tempos de execução

Para visualizar os tempos:

```bash
cat times.txt
```

## Estratégias de paralelização

### OpenMP

A implementação com OpenMP paraleliza o cálculo das linhas da imagem utilizando o número de threads informado na execução.

### Pthreads 1

A primeira implementação com Pthreads divide as linhas da imagem em blocos contínuos entre as threads.

### Pthreads 2

A segunda implementação com Pthreads distribui as linhas de forma intercalada entre as threads.

## Tratamento de erros

O programa verifica situações como:

- Quantidade incorreta de argumentos
- Argumentos inválidos ou não numéricos
- Valores fora dos limites permitidos
- Falha na alocação de memória
- Falha na criação dos arquivos
- Falha na criação das threads

As mensagens de erro são enviadas para `stderr`.

Durante a execução normal, o programa não imprime informações em `stdout`.
